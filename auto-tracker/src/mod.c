#include "wwhd_guest.h"
#include "wwhd/functions.h"

#include "tracker.h"

WWHD_HOOK(WWHD_ADDR_daPy_Execute, auto_tracker_update, (void* link)) {
    (void)link;

    tracker_update();
}
