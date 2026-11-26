
#include "file_writer.h"
#include "wwhd_guest.h"

#define JSON_BUFFER_SIZE 768

static bool append_text(
    char* buffer,
    unsigned int capacity,
    unsigned int* length,
    const char* text
) {
    unsigned int i = 0;

    while (text[i] != '\0') {
        if (*length + 1 >= capacity) {
            return false;
        }

        buffer[*length] = text[i];
        (*length)++;
        i++;
    }

    buffer[*length] = '\0';
    return true;
}

static bool append_bool(
    char* buffer,
    unsigned int capacity,
    unsigned int* length,
    bool value
) {
    return append_text(
        buffer,
        capacity,
        length,
        value ? "true" : "false"
    );
}

static bool append_inventory_value(
    char* buffer,
    unsigned int capacity,
    unsigned int* length,
    const char* name,
    bool value,
    bool inventory_valid,
    bool comma
) {
    if (!append_text(buffer, capacity, length, "\"")) return false;
    if (!append_text(buffer, capacity, length, name)) return false;
    if (!append_text(buffer, capacity, length, "\":")) return false;

    if (inventory_valid) {
        if (!append_bool(buffer, capacity, length, value)) return false;
    } else {
        if (!append_text(buffer, capacity, length, "null")) return false;
    }

    if (comma) {
        if (!append_text(buffer, capacity, length, ",")) return false;
    }

    return true;
}

int file_writer_write(
    const TrackerGameState* state,
    const TrackerInventory* inventory,
    bool inventory_valid
) {
    char json[JSON_BUFFER_SIZE];
    unsigned int length = 0;

    if (state == 0 || inventory == 0) {
        return 0;
    }

    json[0] = '\0';

    if (!append_text(json, sizeof(json), &length,
        "{"
        "\"connected\":true,"
        "\"current_health\":")) return 0;

    /* Current health */
    {
        char digits[6];
        unsigned int value = state->current_health;
        unsigned int count = 0;

        do {
            digits[count++] = (char)('0' + (value % 10));
            value /= 10;
        } while (value > 0 && count < sizeof(digits));

        while (count > 0) {
            char character[2] = { digits[--count], '\0' };
            if (!append_text(json, sizeof(json), &length, character)) return 0;
        }
    }

    if (!append_text(json, sizeof(json), &length,
        ",\"maximum_health\":")) return 0;

    /* Maximum health */
    {
        char digits[6];
        unsigned int value = state->maximum_health;
        unsigned int count = 0;

        do {
            digits[count++] = (char)('0' + (value % 10));
            value /= 10;
        } while (value > 0 && count < sizeof(digits));

        while (count > 0) {
            char character[2] = { digits[--count], '\0' };
            if (!append_text(json, sizeof(json), &length, character)) return 0;
        }
    }

    if (!append_text(json, sizeof(json), &length,
        ",\"rupees\":")) return 0;

    /* Rupees */
    {
        char digits[6];
        unsigned int value = state->rupees;
        unsigned int count = 0;

        do {
            digits[count++] = (char)('0' + (value % 10));
            value /= 10;
        } while (value > 0 && count < sizeof(digits));

        while (count > 0) {
            char character[2] = { digits[--count], '\0' };
            if (!append_text(json, sizeof(json), &length, character)) return 0;
        }
    }

    if (!append_text(json, sizeof(json), &length,
        ",\"inventory_available\":")) return 0;

    if (!append_bool(json, sizeof(json), &length, inventory_valid)) return 0;

    if (!append_text(json, sizeof(json), &length, ",\"inventory\":{")) return 0;

    if (!append_inventory_value(json, sizeof(json), &length,
        "telescope", inventory->telescope, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "sail", inventory->sail, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "wind_waker", inventory->wind_waker, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "grappling_hook", inventory->grappling_hook, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "spoils_bag", inventory->spoils_bag, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "boomerang", inventory->boomerang, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "deku_leaf", inventory->deku_leaf, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "tingle_bottle", inventory->tingle_bottle, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "picto_box", inventory->picto_box, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "iron_boots", inventory->iron_boots, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "magic_armor", inventory->magic_armor, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "bait_bag", inventory->bait_bag, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "heros_bow", inventory->heros_bow, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "bombs", inventory->bombs, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "delivery_bag", inventory->delivery_bag, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "hookshot", inventory->hookshot, inventory_valid, true)) return 0;
    if (!append_inventory_value(json, sizeof(json), &length,
        "skull_hammer", inventory->skull_hammer, inventory_valid, false)) return 0;

    if (!append_text(json, sizeof(json), &length, "}}")) return 0;

    return wwhd_file_write("tracker_state.json", json, length) > 0;
}

int file_writer_write_disconnected(void) {
    static const char json[] = "{\"connected\":false}";

    return wwhd_file_write(
        "tracker_state.json",
        json,
        sizeof(json) - 1
    ) > 0;
}
