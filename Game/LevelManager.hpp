#pragma once
#include "../Utils/Memory.hpp"
#include "../Game/Offsets.hpp"


class LevelManager
{
private:

    ulong _basePointer;
    std::string _levelName;
    bool _isPlayable;
    bool _isTrainingArea;

    ulong readBasePointer() {
        return Offsets::getInstance().region + Offsets::getInstance().level;
    }

    LevelManager() {}
    ~LevelManager() {}

    std::string readLevelName() {
        return Memory::getInstance().readString(_basePointer, 64);
    }

    bool checkIfIsPlayable() {
        return !_levelName.empty() && _levelName.compare("mp_lobby") != 0;
    }

    void setAsInvalid() {
        _levelName.clear();
        _isPlayable = false;
        _isTrainingArea = false;
    }

public:

    static LevelManager& getInstance() {
        static LevelManager instance;
        return instance;
    }

    LevelManager(const LevelManager&) = delete;
    LevelManager& operator=(const LevelManager&) = delete;

    void update() {
        _basePointer = readBasePointer();
        if(!Memory::isValidPointer(_basePointer)) {
            _isPlayable = false;
            return;
        }

        _levelName = readLevelName();
        _isPlayable = checkIfIsPlayable();
        _isTrainingArea = _isPlayable && _levelName.compare("mp_rr_canyonlands_staging") == 0;
    }

    std::string getLevelName() const {
        return _levelName;
    }

    bool isPlayable() const {
        return _isPlayable;
    }

    bool isTrainingArea() const {
        return _isTrainingArea;
    }
};

