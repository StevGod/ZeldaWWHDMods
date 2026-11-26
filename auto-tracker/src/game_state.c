#include "game_state.h"

#include "wwhd/save.h"

bool game_state_read(TrackerGameState* state) {
    if (state == 0) {
        return false;
    }

    uint16_t max_life = dComIfGs_player_status_a.max_life;

    // Reject values that indicate no valid save is loaded.
    if (max_life < 12 || max_life > 80) {
        state->valid = false;
        return false;
    }

    state->current_health = dComIfGs_player_status_a.life;
    state->maximum_health = max_life;
    state->rupees = dComIfGs_player_status_a.rupees;
    state->valid = true;

    return true;
}
