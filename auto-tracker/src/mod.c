#include "wwhd_guest.h"
#include "wwhd/functions.h"

static wwhd_hud_element tracker_text;
static int hud_registered = 0;
static int state_written = 0;

static const char status_message[] = "AutoTracker Connected";
static const char test_json[] =
    "{\"version\":1,\"connected\":true,\"test\":\"file_write_success\"}";

static void draw_tracker_hud(u32 list) {
    tracker_text = (wwhd_hud_element) {
        .kind = WWHD_HUD_TEXT,
        .anchor = WWHD_HUD_TOP_LEFT,
        .x = 24,
        .y = 40,
        .size = 24,
        .rgba = 0xFFFFFFFF,
        .text = status_message,
        .text_bytes = sizeof(status_message) - 1,
        .thickness = 1
    };

    wwhd_hud_emit(list, &tracker_text);
}

WWHD_HOOK(WWHD_ADDR_daPy_Execute, auto_tracker_update, (void* link)) {
    (void)link;

    if (!hud_registered) {
        wwhd_hud_register(draw_tracker_hud, WWHD_HUD_BOTH);
        hud_registered = 1;
    }

    if (!state_written) {
        s32 result = wwhd_file_write(
            "tracker_state.json",
            test_json,
            sizeof(test_json) - 1
        );

        if (result >= 0) {
            state_written = 1;
        }
    }
}
