#ifndef AUTO_TRACKER_FILE_WRITER_H
#define AUTO_TRACKER_FILE_WRITER_H

#include <stdbool.h>

#include "game_state.h"
#include "inventory.h"
#include "equipment.h"

int file_writer_write(
    const TrackerGameState* state,
    const TrackerInventory* inventory,
    bool inventory_valid,
    const TrackerEquipment* equipment,
    bool equipment_valid
);

int file_writer_write_disconnected(void);

#endif
