#pragma once
#include "../Math/Vector3d.hpp"
#include "../Utils/Memory.hpp"
#include "Offsets.hpp"
#include "LocalPlayer.hpp"
#include "../Math/Matrix.hpp"
#include "../Math/Color.hpp"
#include "GlowMode.hpp"
#include "../Utils/Types.hpp"
#include "../Utils/Logger.hpp"

#include "../Utils/Types.hpp"
#include "../Features/Settings.hpp" 

class Player
{
private:

    int _index;
    ulong _basePointer;
    
    bool _isValid;
    bool _isDead;
    bool _isKnocked;
    bool _isAlive;
    int _health;
    int _maxHealth;
    int _shield;
    int _maxShield;
    int _team;
    float _previouslastVisibleTime;
    float _lastVisibleTime;
    bool _isVisible;
    Vector3d _position;
    Vector3d _vecAbsOrigin;
    Vector3d _aimBonePosition;

    ulong readBasePointer() const {
        ulong ptr = Offsets::getInstance().region + Offsets::getInstance().entityList + ((_index + 1) << 5);
        return Memory::getInstance().read<ulong>(ptr);  
    }

    bool readIsDead() const {
        ulong ptr = _basePointer + Offsets::getInstance().lifeState;
        return Memory::getInstance().read<short>(ptr) > 0;
    }

    bool readIsKnocked() const {
        ulong ptr = _basePointer + Offsets::getInstance().bleedOutState;
        return Memory::getInstance().read<short>(ptr) > 0;
    }

    int readHealth() const {
        ulong ptr = _basePointer + Offsets::getInstance().health;
        return Memory::getInstance().read<int>(ptr);  
    }

    int readMaxHealth() const {
        ulong ptr = _basePointer + Offsets::getInstance().maxHealth;
        return Memory::getInstance().read<int>(ptr);  
    }

    int readShield() const {
        ulong ptr = _basePointer + Offsets::getInstance().shield;
        return Memory::getInstance().read<int>(ptr);  
    }

    int readMaxShield() const {
        ulong ptr = _basePointer + Offsets::getInstance().maxShield;
        return Memory::getInstance().read<int>(ptr);  
    }

    int readTeam() const {
        ulong ptr = _basePointer + Offsets::getInstance().team;
        return Memory::getInstance().read<int>(ptr);  
    }

    float readLastVisibleTime() const {
        ulong ptr = _basePointer + Offsets::getInstance().lastVisibleTime;
        return Memory::getInstance().read<float>(ptr);  
    }

    Vector3d readPosition() const {
        ulong ptr = _basePointer + Offsets::getInstance().localOrigin;
        return Memory::getInstance().read<Vector3d>(ptr);  
    }

    Vector3d readVecAbsOrigin() const {
        ulong ptr = _basePointer + Offsets::getInstance().vecAbsOrigin;
        return Memory::getInstance().read<Vector3d>(ptr);  
    }
    

    int readBoneFromHitBox(int hitBox) const {

        ulong modelPtr = Memory::getInstance().read<ulong>(_basePointer + Offsets::getInstance().studioHdr);
		if (!Memory::isValidPointer(modelPtr)) {
            return -1;
        }

		ulong studioHdr =  Memory::getInstance().read<ulong>(modelPtr + 0x8);
        if(!Memory::isValidPointer(studioHdr + 0x34)) {
            return -1;
        }

        auto hitboxCache = Memory::getInstance().read<uint16_t>(studioHdr + 0x34);
        auto hitboxArray = studioHdr +((uint16_t)(hitboxCache & 0xFFFE) << (4 * (hitboxCache & 1))); 
        if(!Memory::isValidPointer(hitboxArray + 0x4)) {
            return -1;
        }

        auto indexCache = Memory::getInstance().read<uint16_t>(hitboxArray + 0x4);
        auto hitboxIndex = ((uint16_t)(indexCache & 0xFFFE) << (4 * (indexCache & 1)));
        auto bonePtr = hitboxIndex + hitboxArray + (hitBox * 0x20);
        if(!Memory::isValidPointer(bonePtr)) {
            return -1;
        }

        return Memory::getInstance().read<uint16_t>(bonePtr);
    }
    
    Vector3d readBonePosition(int hitBox) {
        Vector3d offset = Vector3d(1.5f, 0.0f, 0.0f);

        int bone = readBoneFromHitBox(hitBox);

        if(bone < 0 || bone > 255) {
            return _vecAbsOrigin + offset;
        }

        ulong bonePtr = Memory::getInstance().read<ulong>(_basePointer + Offsets::getInstance().bones);
        bonePtr += (bone * sizeof(Matrix3x4));
        if(!Memory::isValidPointer(bonePtr)) {
            std::cout << "Invalid bone pointer " << bonePtr << std::endl;
            return _vecAbsOrigin + offset;
        }

        Matrix3x4 boneMatrix = Memory::getInstance().read<Matrix3x4>(bonePtr);

        Vector3d bonePosition = boneMatrix.getPosition();

        if(!bonePosition.isValid()) {
            return _vecAbsOrigin + Vector3d(1.5f, 0, 0);
        }

        bonePosition += _vecAbsOrigin;

        return bonePosition;
    }

public:

    Player(int index) : _index(index), _basePointer(0) {
        setAsInvalid();
    }

    void setAsInvalid() {
        _isValid = false;
        _isDead = false;
        _isKnocked = false;
        _health = 0;
        _maxHealth = 0;
        _shield = 0;
        _maxShield = 0;
        _isAlive = false;
        _team = -1;
        _previouslastVisibleTime = 0;
        _lastVisibleTime = 0;
        _isVisible = false;
        _position = Vector3d::zero();
        _vecAbsOrigin = Vector3d::zero();
        _aimBonePosition = Vector3d::zero();
    }

    void update() {
        _basePointer = readBasePointer();

        if(_basePointer == LocalPlayer::getInstance().getBasePointer()){
            setAsInvalid();
            return;
        }

        _isValid = Memory::isValidPointer(_basePointer);
        if(!_isValid) {
            setAsInvalid();
            return;
        }

        _isDead = readIsDead();
        
        if(_isDead) {
            setAsInvalid();
            return;
        }

        _isKnocked = readIsKnocked();

        _health = readHealth();
        _maxHealth = readMaxHealth();
        _shield = readShield();
        _maxShield = readMaxShield();

        _isAlive = !_isDead && !_isKnocked && _health > 0 && _health <= 100;

        _team = readTeam();

        _previouslastVisibleTime = _lastVisibleTime;
        _lastVisibleTime = readLastVisibleTime(); 
        _isVisible = _lastVisibleTime > _previouslastVisibleTime;

        _position = readPosition();
        _vecAbsOrigin = readVecAbsOrigin();

        int hitbox = Settings::getInstance().getAimbotSettings().getHitbox();
        _aimBonePosition = readBonePosition(hitbox);
    }

    int getIndex() const {
        return _index;
    }

    bool isValid() const {
        return _isValid;
    }

    bool isDead() const {
        return _isDead;
    }

    bool isKnocked() const {
        return _isKnocked;
    }

    bool isAlive() const {
        return _isAlive;
    }

    int getHealth() const {
        return _health;
    }

    int getMaxHealth() const {
        return _maxHealth;
    }

    int getShield() const {
        return _shield;
    }

    int getMaxShield() const {
        return _maxShield;
    }

    bool isVisible() const {
        return _isVisible;
    }

    int getTeam() const {
        return _team;
    }

    bool isEnemy() const {
        return _team != LocalPlayer::getInstance().getTeam();
    }

    float distance() const {
        return _position.distanceTo(LocalPlayer::getInstance().getPosition());
    }

    float distanceInMeters() const {
        const float inchesToMeter = 39.3701;
        return distance() / inchesToMeter;
    }

    bool isInRange(float rangeInMeters) const {
        return distanceInMeters() <= rangeInMeters;
    }

    Vector3d getPosition() const {
        return _position;
    }

    Vector3d getAimBonePosition() const {
        return _aimBonePosition;
    }

    void setGlowState(byte enabled, byte throughWalls) const {
        ulong glowEnablePtr = _basePointer + Offsets::getInstance().glowEnable;
        ulong glowThroughWallPtr = _basePointer + Offsets::getInstance().glowThroughWall;

        if(!Memory::isValidPointer(glowEnablePtr) || !Memory::isValidPointer(glowThroughWallPtr)) {
            return;
        }

        Memory::getInstance().write(glowEnablePtr, enabled);
        Memory::getInstance().write(glowThroughWallPtr, throughWalls);
    }

    void setGlowColor(Color color) const {
        ulong ptr = _basePointer + Offsets::getInstance().glowColor;

        if(!Memory::isValidPointer(ptr)) {
            return;
        }

        Memory::getInstance().write(ptr, color);
    }

    void setGlowMode(GlowMode glowMode) const {
        ulong ptr = _basePointer + Offsets::getInstance().glowMode;

        if(!Memory::isValidPointer(ptr)) {
            return;
        }
        
        Memory::getInstance().write(ptr, glowMode);
    }
};