//
// Created by koelion on 05.02.24.
//

#ifndef APEX_ENTITYMANAGER_H
#define APEX_ENTITYMANAGER_H

#include "Player.h"
#include "LocalPlayer.h"

namespace SDK
{
    struct EntityManager
    {
    private:
        int maxPlayers;
        Player players[60];

    public:

        EntityManager()
        {
            maxPlayers = 60;
            for (auto i = 0; i < maxPlayers; i++) {
                players[i] = Player();
            }
        }

        bool Update()
        {
            for (auto i = 0; i < maxPlayers; i++)
            {
                players[i].Update(i);
            }

            return true;
        }

        inline PPlayer operator[](int index)
        {
            if (index < 0 || index >= maxPlayers)
            {
                return nullptr;
            }

            return &players[index];
        }

        inline PPlayer GetPlayer(int index)
        {
            if (index < 0 || index >= maxPlayers)
            {
                return nullptr;
            }

            return &players[index];
        }

        inline int PlayersCount() const
        {
            return maxPlayers;
        }
    };

    typedef EntityManager* PEntityManager;
}

#endif //APEX_ENTITYMANAGER_H
