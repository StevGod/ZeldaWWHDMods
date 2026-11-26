
#ifndef AUTO_TRACKER_INVENTORY_H
#define AUTO_TRACKER_INVENTORY_H

#include <stdbool.h>

typedef struct {
    bool telescope;
    bool sail;
    bool wind_waker;
    bool grappling_hook;
    bool spoils_bag;
    bool boomerang;
    bool deku_leaf;
    bool tingle_bottle;
    bool picto_box;
    bool iron_boots;
    bool magic_armor;
    bool bait_bag;
    bool heros_bow;
    bool bombs;
    bool delivery_bag;
    bool hookshot;
    bool skull_hammer;
} TrackerInventory;

bool inventory_read(TrackerInventory* inventory);

#endif
