
#include "tracker.h"
#include "game_state.h"
#include "inventory.h"
#include "file_writer.h"
#include "wwhd_guest.h"

static bool inventory_changed(
    const TrackerInventory* current,
    const TrackerInventory* previous
) {
    return
        current->telescope != previous->telescope ||
        current->sail != previous->sail ||
        current->wind_waker != previous->wind_waker ||
        current->grappling_hook != previous->grappling_hook ||
        current->spoils_bag != previous->spoils_bag ||
        current->boomerang != previous->boomerang ||
        current->deku_leaf != previous->deku_leaf ||
        current->tingle_bottle != previous->tingle_bottle ||
        current->picto_box != previous->picto_box ||
        current->iron_boots != previous->iron_boots ||
        current->magic_armor != previous->magic_armor ||
        current->bait_bag != previous->bait_bag ||
        current->heros_bow != previous->heros_bow ||
        current->bombs != previous->bombs ||
        current->delivery_bag != previous->delivery_bag ||
        current->hookshot != previous->hookshot ||
        current->skull_hammer != previous->skull_hammer;
}

void tracker_update(void) {
    static TrackerGameState state;
    static TrackerInventory inventory;
    static TrackerInventory last_exported_inventory;

    static unsigned int last_logged_health = 0xFFFF;
    static unsigned int last_logged_max_health = 0xFFFF;
    static unsigned int last_logged_rupees = 0xFFFF;

    static unsigned int last_exported_health = 0xFFFF;
    static unsigned int last_exported_max_health = 0xFFFF;
    static unsigned int last_exported_rupees = 0xFFFF;

    static int last_grappling_hook = -1;
    static int last_boomerang = -1;
    static int last_deku_leaf = -1;
    static int last_iron_boots = -1;
    static int last_magic_armor = -1;
    static int last_heros_bow = -1;
    static int last_bombs = -1;
    static int last_hookshot = -1;
    static int last_skull_hammer = -1;

    static int was_connected = 0;
    static int has_exported_state = 0;
    static int last_exported_inventory_valid = 0;

    /* Read the current game state. */
    if (!game_state_read(&state)) {
        if (was_connected) {
            file_writer_write_disconnected();
            was_connected = 0;
            has_exported_state = 0;
        }

        return;
    }

    /* Read inventory exactly once. */
    bool inventory_valid = inventory_read(&inventory);

    /* Log health changes. */
    if (state.current_health != last_logged_health) {
        wwhd_log_int("Current health", state.current_health);
        last_logged_health = state.current_health;
    }

    if (state.maximum_health != last_logged_max_health) {
        wwhd_log_int("Maximum health", state.maximum_health);
        last_logged_max_health = state.maximum_health;
    }

    /* Log rupee changes. */
    if (state.rupees != last_logged_rupees) {
        wwhd_log_int("Rupees", state.rupees);
        last_logged_rupees = state.rupees;
    }

    /* Log inventory changes. */
    if (inventory_valid) {
        if ((int)inventory.grappling_hook != last_grappling_hook) {
            wwhd_log_int("Grappling Hook", inventory.grappling_hook);
            last_grappling_hook = inventory.grappling_hook;
        }

        if ((int)inventory.boomerang != last_boomerang) {
            wwhd_log_int("Boomerang", inventory.boomerang);
            last_boomerang = inventory.boomerang;
        }

        if ((int)inventory.deku_leaf != last_deku_leaf) {
            wwhd_log_int("Deku Leaf", inventory.deku_leaf);
            last_deku_leaf = inventory.deku_leaf;
        }

        if ((int)inventory.iron_boots != last_iron_boots) {
            wwhd_log_int("Iron Boots", inventory.iron_boots);
            last_iron_boots = inventory.iron_boots;
        }

        if ((int)inventory.magic_armor != last_magic_armor) {
            wwhd_log_int("Magic Armor", inventory.magic_armor);
            last_magic_armor = inventory.magic_armor;
        }

        if ((int)inventory.heros_bow != last_heros_bow) {
            wwhd_log_int("Hero's Bow", inventory.heros_bow);
            last_heros_bow = inventory.heros_bow;
        }

        if ((int)inventory.bombs != last_bombs) {
            wwhd_log_int("Bombs", inventory.bombs);
            last_bombs = inventory.bombs;
        }

        if ((int)inventory.hookshot != last_hookshot) {
            wwhd_log_int("Hookshot", inventory.hookshot);
            last_hookshot = inventory.hookshot;
        }

        if ((int)inventory.skull_hammer != last_skull_hammer) {
            wwhd_log_int("Skull Hammer", inventory.skull_hammer);
            last_skull_hammer = inventory.skull_hammer;
        }
    }

        /* Check whether the game state has changed since the last export. */
    bool state_changed =
        !has_exported_state ||
        state.current_health != last_exported_health ||
        state.maximum_health != last_exported_max_health ||
        state.rupees != last_exported_rupees;

    bool inventory_state_changed =
        !has_exported_state ||
        inventory_valid != last_exported_inventory_valid ||
        (inventory_valid &&
         inventory_changed(&inventory, &last_exported_inventory));

    /* Write JSON whenever something changes. */
    if (state_changed || inventory_state_changed) {
        if (file_writer_write(&state, &inventory, inventory_valid)) {
            was_connected = 1;
            has_exported_state = 1;

            last_exported_health = state.current_health;
            last_exported_max_health = state.maximum_health;
            last_exported_rupees = state.rupees;

            last_exported_inventory_valid = inventory_valid;

            if (inventory_valid) {
                last_exported_inventory = inventory;
            }
        }
    }
}
