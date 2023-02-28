#pragma once
#include "Offsets.hpp"
#include "../Utils/Memory.hpp"

class Weapon
{
private:
    int _index;
    ulong _basePointer;

    bool _isValid;
    float _projectileSpeed;
    float _projectileScale;
    int _ammoInClip;

    ulong readBasePointer()
    {
        ulong ptr = Offsets::getInstance().region + Offsets::getInstance().entityList + (_index << 5);
        return Memory::getInstance().read<ulong>(ptr);  
    }

    float readProjectileSpeed() const {
        ulong ptr = _basePointer + Offsets::getInstance().projectileSpeed;
        return Memory::getInstance().read<float>(ptr);  
    }

    float readProjectileScale() const {
        ulong ptr = _basePointer + Offsets::getInstance().projectileScale;
        return Memory::getInstance().read<float>(ptr);  
    }

    int readAmmoInClip() const {
        ulong ptr = _basePointer + Offsets::getInstance().ammoInClip;
        return Memory::getInstance().read<int>(ptr);  
    }

    void setAsInvalid() {
        _basePointer = 0;
        _isValid = false;
    }

public:

    Weapon() : _index(0), _basePointer(0), _isValid(0), _projectileSpeed(0), _projectileScale(0), _ammoInClip(0) {}

    void update(int index) {
        _index = index;
        _basePointer = readBasePointer();
        
        _isValid = Memory::isValidPointer(_basePointer);
        if(!_isValid) {
            setAsInvalid();
            return;
        }

        _projectileSpeed = readProjectileSpeed();
        _projectileScale = readProjectileScale();
        _ammoInClip = readAmmoInClip();
    }

    bool isValid() const {
        return _isValid;
    }

    float getProjectileSpeed() const {
        return _projectileSpeed;
    }

    float getProjectileScale() const {
        return _projectileScale;
    }

    int getAmmoInClip() const {
        return _ammoInClip;
    }
};
