#include "wwhd_guest.h"
#include "wwhd/functions.h"
#include "wwhd/save.h"

static u16 last_life = 0xFFFF;
static u16 last_max_life = 0xFFFF;
static u16 last_rupees = 0xFFFF;

WWHD_HOOK(WWHD_ADDR_daPy_Execute, auto_tracker_update, (void* link)) {
    (void)link;

    u16 max_life = dComIfGs_player_status_a.max_life;
    u16 life = dComIfGs_player_status_a.life;
    u16 rupees = dComIfGs_player_status_a.rupees;

    /* Ignore the game until a valid save is loaded. */
    if (max_life < 12 || max_life > 80) {
        return;
    }

    /* Only log values when they change. */
    if (life != last_life) {
        wwhd_log_int("Current health", life);
        last_life = life;
    }

    if (max_life != last_max_life) {
        wwhd_log_int("Maximum health", max_life);
        last_max_life = max_life;
    }

    if (rupees != last_rupees) {
        wwhd_log_int("Rupees", rupees);
        last_rupees = rupees;
    }
}