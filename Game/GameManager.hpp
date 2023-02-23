#pragma once
#include "LevelManager.hpp"
#include "Camera.hpp"
#include "LocalPlayer.hpp"
#include "PlayerManager.hpp"
#include "TimeManager.hpp"

class GameManager
{
private:

    GameManager() {}
    ~GameManager() {}

public:

    static GameManager& getInstance() {
        static GameManager instance;
        return instance;
    }

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    void update() {
        TimeManager::getInstance().update();
        
        LevelManager::getInstance().update();
        if(!LevelManager::getInstance().isPlayable())
        {
            return;
        }

        Camera::getInstance().update();
        LocalPlayer::getInstance().update();
        PlayerManager::getInstance().update();
    }
};