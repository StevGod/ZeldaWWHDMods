#include "tracker.h"
#include "game_state.h"

#include "wwhd_guest.h"

void tracker_update(void) {
    static TrackerGameState state;

    static uint16_t last_health = 0xFFFF;
    static uint16_t last_max_health = 0xFFFF;
    static uint16_t last_rupees = 0xFFFF;

    if (!game_state_read(&state)) {
        return;
    }

    if (state.current_health != last_health) {
        wwhd_log_int("Current health", state.current_health);
        last_health = state.current_health;
    }

    if (state.maximum_health != last_max_health) {
        wwhd_log_int("Maximum health", state.maximum_health);
        last_max_health = state.maximum_health;
    }

    if (state.rupees != last_rupees) {
        wwhd_log_int("Rupees", state.rupees);
        last_rupees = state.rupees;
    }
}
