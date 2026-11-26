#include "inventory.h"

#include "wwhd_guest.h"
#include "wwhd/save.h"

/*
 * The item array starts at offset 0x3C from the
 * player-status structure.
 *
 * There are 21 inventory slots, each one byte.
 */
#define INVENTORY_ITEM_OFFSET 0x3C

/* Inventory slot positions */
#define SLOT_TELESCOPE       0
#define SLOT_SAIL            1
#define SLOT_WIND_WAKER      2
#define SLOT_GRAPPLING_HOOK  3
#define SLOT_SPOILS_BAG      4
#define SLOT_BOOMERANG       5
#define SLOT_DEKU_LEAF       6
#define SLOT_TINGLE_BOTTLE   7
#define SLOT_PICTO_BOX       8
#define SLOT_IRON_BOOTS      9
#define SLOT_MAGIC_ARMOR     10
#define SLOT_BAIT_BAG        11
#define SLOT_HEROS_BOW       12
#define SLOT_BOMBS           13
#define SLOT_DELIVERY_BAG    18
#define SLOT_HOOKSHOT        19
#define SLOT_SKULL_HAMMER    20

/* Item IDs */
#define ITEM_TELESCOPE       0x20
#define ITEM_SAIL            0x78
#define ITEM_SWIFT_SAIL      0x77
#define ITEM_WIND_WAKER      0x22
#define ITEM_GRAPPLING_HOOK  0x25
#define ITEM_SPOILS_BAG      0x24
#define ITEM_BOOMERANG       0x2D
#define ITEM_DEKU_LEAF       0x34
#define ITEM_TINGLE_BOTTLE   0x21
#define ITEM_PICTO_BOX       0x23
#define ITEM_DELUXE_PICTO    0x26
#define ITEM_IRON_BOOTS      0x29
#define ITEM_MAGIC_ARMOR     0x2A
#define ITEM_BAIT_BAG        0x2C
#define ITEM_HEROS_BOW       0x27
#define ITEM_BOMBS           0x31
#define ITEM_DELIVERY_BAG    0x30
#define ITEM_HOOKSHOT        0x2F
#define ITEM_SKULL_HAMMER    0x33

static bool item_in_slot(
    volatile const unsigned char* items,
    unsigned int slot,
    unsigned char item_id
) {
    return items[slot] == item_id;
}

static bool item_in_slot_either(
    volatile const unsigned char* items,
    unsigned int slot,
    unsigned char item_a,
    unsigned char item_b
) {
    return items[slot] == item_a || items[slot] == item_b;
}

bool inventory_read(TrackerInventory* inventory) {
    if (inventory == 0) {
        return false;
    }

    /*
     * Check that the save-data pointer exists before
     * attempting to read the inventory.
     */
    unsigned int save_base =
        WWHD_GAME_DATA(WWHD_ADDR_save_info_pointer, u32);

    if (save_base == 0) {
        return false;
    }

    /*
     * Avoid reading inventory when no valid save is loaded.
     * This uses the same health guard as game_state.c.
     */
    unsigned int max_life = dComIfGs_player_status_a.max_life;

    if (max_life < 12 || max_life > 80) {
        return false;
    }

    /*
     * Locate the 21 inventory slots.
     */
    volatile const unsigned char* items =
        (volatile const unsigned char*)(
            save_base +
            WWHD_OFFSET_save_info +
            INVENTORY_ITEM_OFFSET
        );

    inventory->telescope =
        item_in_slot(items, SLOT_TELESCOPE, ITEM_TELESCOPE);

    inventory->sail =
        item_in_slot_either(
            items, SLOT_SAIL, ITEM_SAIL, ITEM_SWIFT_SAIL
        );

    inventory->wind_waker =
        item_in_slot(items, SLOT_WIND_WAKER, ITEM_WIND_WAKER);

    inventory->grappling_hook =
        item_in_slot(items, SLOT_GRAPPLING_HOOK, ITEM_GRAPPLING_HOOK);

    inventory->spoils_bag =
        item_in_slot(items, SLOT_SPOILS_BAG, ITEM_SPOILS_BAG);

    inventory->boomerang =
        item_in_slot(items, SLOT_BOOMERANG, ITEM_BOOMERANG);

    inventory->deku_leaf =
        item_in_slot(items, SLOT_DEKU_LEAF, ITEM_DEKU_LEAF);

    inventory->tingle_bottle =
        item_in_slot(items, SLOT_TINGLE_BOTTLE, ITEM_TINGLE_BOTTLE);

    inventory->picto_box =
        item_in_slot_either(
            items, SLOT_PICTO_BOX, ITEM_PICTO_BOX, ITEM_DELUXE_PICTO
        );

    inventory->iron_boots =
        item_in_slot(items, SLOT_IRON_BOOTS, ITEM_IRON_BOOTS);

    inventory->magic_armor =
        item_in_slot(items, SLOT_MAGIC_ARMOR, ITEM_MAGIC_ARMOR);

    inventory->bait_bag =
        item_in_slot(items, SLOT_BAIT_BAG, ITEM_BAIT_BAG);

    inventory->heros_bow =
        item_in_slot(items, SLOT_HEROS_BOW, ITEM_HEROS_BOW);

    inventory->bombs =
        item_in_slot(items, SLOT_BOMBS, ITEM_BOMBS);

    inventory->delivery_bag =
        item_in_slot(items, SLOT_DELIVERY_BAG, ITEM_DELIVERY_BAG);

    inventory->hookshot =
        item_in_slot(items, SLOT_HOOKSHOT, ITEM_HOOKSHOT);

    inventory->skull_hammer =
        item_in_slot(items, SLOT_SKULL_HAMMER, ITEM_SKULL_HAMMER);

    return true;
}
