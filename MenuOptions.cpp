#include "MenuOptions.h"
#include "MinHook.h"

bool __fastcall MenuOptions::initHook(void* self)
{
    bool result = MenuOptions::init(self);
    MenuOptions::addToggle(
        self,
        "Show Level Info",
        "1000",
        "Show info about the level while you're playing it, useful for level request streams! (Added by Figment's Utilities)");

    MenuOptions::addToggle(
        self,
        "\"In Practice\" text",
        "1001",
        "Puts the text \"In Practice\" on your screen while you're in practice mode! (Added by Figment's Utilities)");

    MenuOptions::addToggle(
        self,
        "Lock Mouse Cursor",
        "1003",
        "Keep the mouse cursor in GD while the window is selected and alt is not pressed! (Added by Figment's Utilities)");

    return result;
}

void MenuOptions::mem_init() {
    size_t base = reinterpret_cast<size_t>(GetModuleHandle(0));
    MH_CreateHook(
        (PVOID)(base + 0x1DE8F0),
        MenuOptions::initHook,
        (PVOID*)&MenuOptions::init);

    MenuOptions::addToggle = reinterpret_cast<decltype(MenuOptions::addToggle)>(base + 0x1DF6B0);
}