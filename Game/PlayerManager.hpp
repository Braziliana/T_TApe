#pragma once
#include "../Utils/Memory.hpp"
#include "Player.hpp"

class PlayerManager
{
private:
    int _size;
    Player** _players;

    PlayerManager() : _size(60) {
        _players = new Player*[_size];

        for(auto i = 0; i < _size; i++) {
            _players[i] = new Player(i);
        }
    }

    ~PlayerManager() {
        for(auto i = 0; i < _size; i++) {
            delete _players[i];
        }

        delete _players;
    }

public:

    static PlayerManager& getInstance() {
        static PlayerManager instance;
        return instance;
    }

    PlayerManager(const PlayerManager&) = delete;
    PlayerManager& operator=(const PlayerManager&) = delete;
    
    Player** begin() const {
        return _players;
    }
    
    Player** end() const {
        return _players + _size;
    }

    Player* operator[](int index) const {
        if(index < 0 || index >= _size)
        {
            return nullptr;
        }

        return _players[index];
    }
    
    void update() {
        for(auto p : *this) {
            if(p != nullptr) {
                p->update();
            }
        }
    }

};
