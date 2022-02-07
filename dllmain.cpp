#include "pch.h"
#include <cstdio>
#include <iostream>
#include "MinHook.h"

uintptr_t gunz_base = (uintptr_t)GetModuleHandleA("Gunz.exe");

typedef void(__cdecl* _mlog)(const char* message);
_mlog mlog;

typedef void(__cdecl* _lawnmower)();
_lawnmower lawnmower;

typedef void(__cdecl* _esp)();
_esp esp;

typedef void(__cdecl* _shotbot)();
_shotbot shotbot;

typedef void(__cdecl* _quest)();
_quest quest;

typedef void(__cdecl* _godmode)();
_godmode godmode;

void __cdecl godmode_func()
{
    return godmode();
}
void __cdecl quest_func()
{
    return quest();
}

void __cdecl shotbot_func()
{
    return shotbot();
}

void __cdecl esp_func()
{
    return esp();
}

void __cdecl lawnmower_func()
{
    return lawnmower();
}

void __cdecl mlog_func(const char* message)
{
    std::cout << message << std::endl;
    std::cout << std::endl;
    return mlog(message);
}

uintptr_t WINAPI Boof(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << std::hex << gunz_base << std::endl;

    MH_Initialize();

    auto mlog_hook = MH_CreateHook((LPVOID)(gunz_base + 0x4e0430), &mlog_func, (LPVOID*)&mlog);
    if (mlog_hook != 0)
        printf("mlog hook boom\n");

    auto mlog_enable = MH_EnableHook((void*)mlog_hook);
    if (mlog_enable != 0)
        printf("mlog hook not enable wtf\n");
    
    auto lawnmower_hook = MH_CreateHook((LPVOID)(gunz_base + 0x0010f3a0), &lawnmower_func, (LPVOID*)&lawnmower);
    if (lawnmower_hook != 0)
        printf("lawnmower hook boom\n");

    auto lawnmower_enable = MH_EnableHook((void*)lawnmower_hook);
    if (lawnmower_enable != 0)
        printf("lawnmower_enable not work..\n");

    auto esp_hook = MH_CreateHook((LPVOID)(gunz_base + 0x000f8260), &esp_func, (LPVOID*)&esp);
    if (esp_hook != 0)
        printf("Esp hook boom\n");

    auto esp_enable = MH_EnableHook((void*)esp_hook);
    if (esp_enable != 0)
        printf("Esp no enable wtf men\n");

    auto shotbot_hook = MH_CreateHook((LPVOID)(gunz_base + 0x00111cc0), &shotbot_func, (LPVOID*)&shotbot);
    if (shotbot_hook != 0)
        printf("Shotbot hook no work\n");
    
    auto shotbot_enable = MH_EnableHook((void*)shotbot_hook);
    if (shotbot_enable != 0)
        printf("?XDshotbot enable booM~!\n");
    
    auto quest_hook = MH_CreateHook((LPVOID)(gunz_base + 0x0010edf0), &quest_func, (LPVOID*)&quest);
    if (quest_hook != 0)
        printf("?XSDquest boom\n");

    auto quest_enable = MH_EnableHook((void*)quest_hook);
    if (quest_enable != 0)
        printf("?XD quest no enable\n");

    auto godmode_hook = MH_CreateHook((LPVOID)(gunz_base + 0x000ffb70), &godmode_func, (LPVOID*)&godmode);
    if (godmode_hook != 0)
        printf("Godmode hook boom W T F\n");

    auto godmode_enable = MH_EnableHook((void*)godmode_hook);
    if (godmode_enable != 0)
        printf("Godmode no enable FUCK\n");

    for ( ;; )
    {
        if (GetAsyncKeyState(VK_F9) & 1)
            esp_func();

        // This isn't actually shotbot, it's some admin function that just fucking one shots the whole lobby?
        if (GetAsyncKeyState(VK_F8) & 1)
            shotbot_func();

        // This is insane massive. The admin_quest function's broken and I don't care enough to rename it
        if (GetAsyncKeyState(VK_F7) & 1)
        {
            CreateThread(nullptr,
                0,
                (LPTHREAD_START_ROUTINE)quest_func,
                0,
                0,
                nullptr
            );
        }

        if (GetAsyncKeyState(VK_F10) & 1)
            CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)godmode_func, 0, 0, nullptr);

        // EPIC ADMIN COMMAND GETS ABUSED BY HUNG (LARGE) MALE PLAYER
        if (GetAsyncKeyState(VK_F2) & 1)
        {
            // Overrwite PUSH 0xa (10) so that 1ms is passed into the sleep call. Need moar slashes.
            int a = WriteProcessMemory(GetCurrentProcess(), (LPVOID)(gunz_base + 0x0010fa40), "\x6A\x01", 2, NULL);
            std::cout << a << std::endl;
            lawnmower_func();
        }
        
        // Killswitch
        if (GetAsyncKeyState(VK_F1) & 1)
            break;

        Sleep(20);
    }

    MH_DisableHook((void*)mlog_hook);
    MH_DisableHook((void*)lawnmower_hook);
    MH_DisableHook((void*)esp_hook);
    MH_DisableHook((void*)shotbot_hook);
    MH_DisableHook((void*)quest_hook);
    MH_DisableHook((void*)godmode_hook);
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(
            CreateThread(nullptr,
                0,
                (LPTHREAD_START_ROUTINE)Boof,
                hModule,
                0,
                nullptr
            )
        );
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
