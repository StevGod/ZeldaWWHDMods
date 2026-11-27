#include "chest_tracker.h"
#include "wwhd_guest.h"

#define CHEST_VTABLE_ADDRESS 0x1003FE44u

#define CHEST_HOME_X_OFFSET       0x2ECu
#define CHEST_HOME_Y_OFFSET       0x2F0u
#define CHEST_HOME_Z_OFFSET       0x2F4u
#define CHEST_HOME_ROT_Z_OFFSET   0x2FCu
#define CHEST_HOME_ROOM_OFFSET    0x2FEu
#define CHEST_PARAMETERS_OFFSET   0x0B0u
#define CHEST_VTABLE_OFFSET       0x0B4u
#define CHEST_PROCESS_ID_OFFSET   0x004u

#define INVALID_REWARD_ID 0xFFu

static u32 read_u32(const void* address)
{
    return *(const volatile u32*)address;
}

static u16 read_u16(const void* address)
{
    return *(const volatile u16*)address;
}

static f32 read_f32(const void* address)
{
    return *(const volatile f32*)address;
}

void chest_tracker_on_open(void* chest)
{
    volatile u8* base;
    u32 vtable;
    u32 parameters;
    u32 process_id;
    u16 home_rotation_z;
    u8 reward_id;
    u8 home_room;

    if (chest == 0)
        return;

    base = (volatile u8*)chest;

    vtable = read_u32((const void*)(base + CHEST_VTABLE_OFFSET));

    /* Only process confirmed treasure chest actors. */
    if (vtable != CHEST_VTABLE_ADDRESS)
        return;

    parameters = read_u32((const void*)(base + CHEST_PARAMETERS_OFFSET));
    process_id = read_u32((const void*)(base + CHEST_PROCESS_ID_OFFSET));

    home_rotation_z = read_u16((const void*)(base + CHEST_HOME_ROT_Z_OFFSET));
    reward_id = (u8)(home_rotation_z >> 8);
    home_room = *(volatile u8*)(base + CHEST_HOME_ROOM_OFFSET);

    wwhd_log("Chest opened");
    wwhd_log_hex("Chest vtable", vtable);
    wwhd_log_hex("Chest parameters", parameters);
    wwhd_log_hex("Chest process ID", process_id);

    wwhd_log_float("Chest home X",
        read_f32((const void*)(base + CHEST_HOME_X_OFFSET)));

    wwhd_log_float("Chest home Y",
        read_f32((const void*)(base + CHEST_HOME_Y_OFFSET)));

    wwhd_log_float("Chest home Z",
        read_f32((const void*)(base + CHEST_HOME_Z_OFFSET)));

    wwhd_log_hex("Chest home rotation Z", home_rotation_z);
    wwhd_log_hex("Chest home room", home_room);
    wwhd_log_hex("Chest reward ID", reward_id);

    if (reward_id == INVALID_REWARD_ID)
        wwhd_log("Chest reward ID is invalid");
}
