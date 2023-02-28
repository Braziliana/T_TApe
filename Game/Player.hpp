#pragma once
#include "../Math/Vector3d.hpp"
#include "../Utils/Memory.hpp"
#include "Offsets.hpp"
#include "LocalPlayer.hpp"
#include "Enums/HitboxType.hpp"
#include "../Math/Matrix.hpp"
#include "../Math/Color.hpp"
#include "GlowMode.hpp"
#include "../Utils/Types.hpp"
#include "../Utils/Logger.hpp"
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
    Vector3d _vecAbsVelocity;
    Vector3d _aimBonePosition;
    Vector3d _headPosition;

    GlowMode _glowMode;
    Color _glowColor;
    byte _glowEnabled;
    byte _glowThroughWall;

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

    Vector3d readVecAbsVelocity() const {
        ulong ptr = _basePointer + Offsets::getInstance().vecAbsVelocity;
        return Memory::getInstance().read<Vector3d>(ptr);  
    }

    int readBoneFromHitBox(HitboxType hitBox) const {

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
        auto bonePtr = hitboxIndex + hitboxArray + (static_cast<int>(hitBox) * 0x20);
        if(!Memory::isValidPointer(bonePtr)) {
            return -1;
        }

        return Memory::getInstance().read<uint16_t>(bonePtr);
    }
    
    Vector3d readBonePosition(HitboxType hitBox) const {
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

    Vector3d readHeadPosition() {
        return readBonePosition(HitboxType::Head);
    }

    GlowMode readGlowMode() const {
        ulong ptr = _basePointer + Offsets::getInstance().glowMode;
        return Memory::getInstance().read<GlowMode>(ptr);  
    }

    Color readGlowColor() const {
        ulong ptr = _basePointer + Offsets::getInstance().glowColor;
        return Memory::getInstance().read<Color>(ptr);  
    }

    byte readGlowEnabled() const {
        ulong ptr = _basePointer + Offsets::getInstance().glowEnable;
        return Memory::getInstance().read<byte>(ptr);  
    }

    byte readGlowThroughWall() const {
        ulong ptr = _basePointer + Offsets::getInstance().glowThroughWall;
        return Memory::getInstance().read<byte>(ptr);  
    }

    int readNamePtr() {
        ulong ptr = _basePointer + Offsets::getInstance().name;
        return Memory::getInstance().read<int>(ptr);  
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
        _vecAbsVelocity = Vector3d::zero();
        _aimBonePosition = Vector3d::zero();
        _headPosition = Vector3d::zero();
        _glowMode = GlowMode(HighlightFill::FillNone, HighlightOutline::FillNone, 0, 0);
        _glowColor = Color(0, 0, 0);
        _glowEnabled = 2;
        _glowThroughWall = 5;
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

        if(readNamePtr() == 0) {
            setAsInvalid();
            return;
        }

        _isKnocked = readIsKnocked();

        _health = readHealth();
        if(_health <= 0 || _health > 300 ) {
            setAsInvalid();
            return;
        }
        _maxHealth = readMaxHealth();
        _shield = readShield();

        if(_health <= 0 || _health > 1000 ) {
            setAsInvalid();
            return;
        }
        
        _maxShield = readMaxShield();

        _isAlive = !_isDead && !_isKnocked && _health > 0 && _health <= 100;

        _team = readTeam();

        _previouslastVisibleTime = _lastVisibleTime;
        _lastVisibleTime = readLastVisibleTime(); 
        _isVisible = _lastVisibleTime > _previouslastVisibleTime;

        _position = readPosition();

        if(_position == Vector3d::zero()) {
            setAsInvalid();
            return;
        }

        _vecAbsOrigin = readVecAbsOrigin();
        _vecAbsVelocity = readVecAbsVelocity();

        _glowMode = readGlowMode();
        _glowColor = readGlowColor();
        _glowEnabled = readGlowEnabled();

        if(_glowEnabled == 0 || _glowEnabled == 255) {
            setAsInvalid();
            return;
        }

        _glowThroughWall = readGlowThroughWall();

        HitboxType hitbox = Settings::getInstance().getAimbotSettings().getHitbox();
        _aimBonePosition = readBonePosition(hitbox);
        _headPosition = readHeadPosition();
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

    Vector3d getVecAbsOrigin() const {
        return _vecAbsOrigin;
    }

    Vector3d getVecAbsVelocity() const {
        return _vecAbsVelocity;
    }

    Vector3d getAimBonePosition() const {
        return _aimBonePosition;
    }

    Vector3d getHeadPosition() const {
        return _headPosition;
    }

    void setGlowState(byte enabled, byte throughWalls) {
        if(_glowEnabled == enabled && _glowThroughWall == throughWalls) {
            return;
        }

        ulong glowEnablePtr = _basePointer + Offsets::getInstance().glowEnable;
        ulong glowThroughWallPtr = _basePointer + Offsets::getInstance().glowThroughWall;

        if(!Memory::isValidPointer(glowEnablePtr) || !Memory::isValidPointer(glowThroughWallPtr)) {
            return;
        }

        Memory::getInstance().write(glowEnablePtr, enabled);
        Memory::getInstance().write(glowThroughWallPtr, throughWalls);
        _glowEnabled = enabled;
        _glowThroughWall = throughWalls;
    }

    void setGlowColor(Color color) {
        if(_glowColor == color) {
            return;
        }

        ulong ptr = _basePointer + Offsets::getInstance().glowColor;

        if(!Memory::isValidPointer(ptr)) {
            return;
        }

        Memory::getInstance().write(ptr, color);
        _glowColor = color;
    }

    void setGlowMode(GlowMode glowMode) {
        if(_glowMode == glowMode) {
            return;
        }

        ulong ptr = _basePointer + Offsets::getInstance().glowMode;

        if(!Memory::isValidPointer(ptr)) {
            return;
        }
        
        Memory::getInstance().write(ptr, glowMode);
        _glowMode = glowMode;
    }

    static std::string getClientClassName(int index) {
        ulong entity_ptr = Offsets::getInstance().region + Offsets::getInstance().entityList + ((index + 1) << 5);
        entity_ptr = Memory::getInstance().read<ulong>(entity_ptr);
        if(!Memory::isValidPointer(entity_ptr)){
            return "";
        }
        long client_networkable_vtable;
        long get_client_entity;
        int offset;
        long network_name_ptr;
        char buffer[32];

        try {
        // Read the ClientClass's network name for to given entity
        client_networkable_vtable = Memory::getInstance().read<long>(entity_ptr + 3 * 8);
        get_client_entity = Memory::getInstance().read<long>(client_networkable_vtable + 3 * 8);
        offset = Memory::getInstance().read<long>(get_client_entity + 3);
        network_name_ptr = Memory::getInstance().read<long>(get_client_entity + offset + 7 + 16);
        
        return Memory::getInstance().readString(network_name_ptr, 32);
        }
        catch(...){
            return "";
        }
    }
};