#ifndef AUTO_TRACKER_EQUIPMENT_H
#define AUTO_TRACKER_EQUIPMENT_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t sword_entry;
    uint8_t shield_entry;

    uint8_t sword_level;
    uint8_t shield_level;
} TrackerEquipment;

bool equipment_read(TrackerEquipment* equipment);

#endif