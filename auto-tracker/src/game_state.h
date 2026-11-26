#ifndef AUTO_TRACKER_GAME_STATE_H
#define AUTO_TRACKER_GAME_STATE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint16_t current_health;
    uint16_t maximum_health;
    uint16_t rupees;
    bool valid;
} TrackerGameState;

bool game_state_read(TrackerGameState* state);

#endif
