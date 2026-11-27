#include "wwhd_guest.h"
#include "wwhd/functions.h"
#include "tracker.h"
#include "chest_tracker.h"

WWHD_HOOK(WWHD_ADDR_daPy_Execute, auto_tracker_update, (void* link)) {
    (void)link;
    tracker_update();
}

WWHD_HOOK(0x024B63A8, auto_tracker_chest_open, (void* chest)) {
    chest_tracker_on_open(chest);
}