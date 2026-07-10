#include "hack.h"
#include "log.h"
#include "il2cpp_dump.h"
#include "xdl.h"

HOOK_DEF(void, GroupCreateRoomReq_Ctor, (void* this_ptr)) {
    LOGI("=== GROK MOD: GroupCreateRoomReq Hooked ===");

    if (this_ptr) {
        // === 48 PLAYER + CUSTOM NAME ===
        set_field<uint32_t>(this_ptr, 0x30, 48);                    // max_member_num = 48
        set_field<String*>(this_ptr, 0x20, "GROK MOD ROOM");       // Room Name
        
        // Extra Features
        set_field<bool>(this_ptr, 0x38, true);                     // Death Spectate ON
        set_field<bool>(this_ptr, 0x100, true);                    // Scoreboard ON
        set_field<bool>(this_ptr, 0xc4, false);                    // Not Solo
        
        LOGI("Room Created Successfully: 48 Players | GROK MOD ROOM");
    }

    original_GroupCreateRoomReq_Ctor(this_ptr);
}

void hack_thread(void* arg) {
    LOGI("GROK MOD Waiting for libil2cpp.so...");

    while (!il2cpp::init()) {
        sleep(1);
    }

    LOGI("IL2CPP Initialized!");

    void* ctor = il2cpp::get_method("tcp", "GroupCreateRoomReq", ".ctor");
    if (ctor) {
        HOOK(GroupCreateRoomReq_Ctor, ctor);
        LOGI("48 Player Room Hook SUCCESS! ✅");
    } else {
        LOGE("Hook Failed! Class not found.");
    }
}

__attribute__((constructor))
void init_module() {
    pthread_t t;
    pthread_create(&t, nullptr, hack_thread, nullptr);
}
