#pragma once
#include "cocos2d.h"

namespace MainLayer {
    using namespace cocos2d;
    inline bool(__thiscall* init)(CCLayer* self);
    bool __fastcall initHook(CCLayer* self, int edx);

    void mem_init();
}
