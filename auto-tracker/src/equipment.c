#include "equipment.h"
#include "wwhd/save.h"
#include "wwhd_guest.h"

#define EQUIPMENT_OFFSET 0x0E

/* Sword item IDs */
#define ITEM_HEROS_SWORD             0x38
#define ITEM_MASTER_SWORD_POWERLESS  0x39
#define ITEM_MASTER_SWORD_HALF       0x3A
#define ITEM_MASTER_SWORD_FULL       0x3E

/* Shield item IDs */
#define ITEM_HEROS_SHIELD            0x3B
#define ITEM_MIRROR_SHIELD           0x3C

static uint8_t sword_level_from_item(uint8_t item_id) {
    switch (item_id) {
        case ITEM_HEROS_SWORD:
            return 1;

        case ITEM_MASTER_SWORD_POWERLESS:
            return 2;

        case ITEM_MASTER_SWORD_HALF:
            return 3;

        case ITEM_MASTER_SWORD_FULL:
            return 4;

        default:
            return 0;
    }
}

static uint8_t shield_level_from_item(uint8_t item_id) {
    switch (item_id) {
        case ITEM_HEROS_SHIELD:
            return 1;

        case ITEM_MIRROR_SHIELD:
            return 2;

        default:
            return 0;
    }
}

bool equipment_read(TrackerEquipment* equipment) {
    if (equipment == 0) {
        return false;
    }

    unsigned int save_base =
        WWHD_GAME_DATA(WWHD_ADDR_save_info_pointer, u32);

    if (save_base == 0) {
        return false;
    }

    unsigned int max_life =
        dComIfGs_player_status_a.max_life;

    if (max_life < 12 || max_life > 80) {
        return false;
    }

    volatile const uint8_t* select_equip =
        (volatile const uint8_t*)(
            save_base +
            WWHD_OFFSET_save_info +
            EQUIPMENT_OFFSET
        );

    equipment->sword_entry = select_equip[0];
    equipment->shield_entry = select_equip[1];

    equipment->sword_level =
        sword_level_from_item(equipment->sword_entry);

    equipment->shield_level =
        shield_level_from_item(equipment->shield_entry);

    return true;
}
