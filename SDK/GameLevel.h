//
// Created by koelion on 05.02.24.
//

#ifndef APEX_GAMELEVEL_H
#define APEX_GAMELEVEL_H

#include "Offsets.h"
#include "cstring"
#include "../Misc/Memory.h"

namespace SDK
{
    struct GameLevel
    {
    private:
        std::string name;
        bool playable;

    public:

        GameLevel() {
            playable = false;
            name = "";
        }

        bool Update()
        {
//            char name[64];
//            if(!Memory::getInstance().readString(OFF_REGION + OFF_LEVEL, name, 64)){
//                return false;
//            }

            name = Memory::getInstance().readString(OFF_REGION + OFF_LEVEL, 64);

            playable = !name.empty() && strcmp(name.c_str(), "mp_lobby") != 0;
            return playable;

        }

        bool IsPlayable() const
        {
            return playable;
        }

        [[nodiscard]] std::string GetName() const
        {
            return name;
        }
    };

    typedef GameLevel* PGameLevel;
}

#endif //APEX_GAMELEVEL_H
