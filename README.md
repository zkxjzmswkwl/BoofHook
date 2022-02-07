# BoofHook
admin had "admin commands" in the client that were just cheats for staff only.

Looked like this:

Data store:

![l0l](https://i.imgur.com/P5v5FvT.png)

Reference: 

![Bismillah](https://i.imgur.com/GK3JSxJ.png)

Inside each would be (most of the time) be something similar to this:

![thread](https://i.imgur.com/h4gxbo4.png)

```cpp
CreateThread((LPSECURITY_ATTRIBUTES)0x0,0,FUN_0010edf0,(LPVOID)0x0,0,(LPDWORD)0x0);
```

So all you had to do to cheat just like the admins is..

```cpp
typedef void(__cdecl* _lawnmower)();
_lawnmower lawnmower;

void __cdecl lawnmower_func() { return lawnmower(); }

auto lawnmower_hook = MH_CreateHook((LPVOID)(gunz_base + 0x0010f3a0), &lawnmower_func, (LPVOID*)&lawnmower);
if (lawnmower_hook != 0)
  printf("lawnmower hook boom\n");
 
auto lawnmower_enable = MH_EnableHook((void*)lawnmower_hook);
if (lawnmower_enable != 0)
  printf("lawnmower_enable not work..\n");

for ( ;; )
{
  if (GetAsyncKeyState(VK_F2) & 1)
    lawnmower_func();
  Sleep(10);
}
```

After some fun I felt bad and sent the developers this source code and told them the functions I was hooking.

That being said, you can do something like this:

```cpp
typedef void(__thiscall* tSetAnimationLower)(PVOID, int);
tSetAnimationLower oSetAnimationLower;


void __fastcall HSetAnimationLower(void * ecx, void * edx, int ani_enum)
{
    std::cout << "Zcharacter ptr: " << std::hex << ecx << std::endl;
    std::cout << "SetAnimationLower: " << ani_enum << std::endl;
    return oSetAnimationLower(ecx, 11);
}
```

Which sets the lower portion of your actor to one of the "DEAD" states. The result is: https://clips.twitch.tv/ExquisitePlumpSpiderPartyTime-_YWEBOvljXHbp_Lj
