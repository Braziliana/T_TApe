#pragma once
#include "../Utils/Memory.hpp"
#include "../Math/QAngle.hpp"
#include "Offsets.hpp"
#include "Weapon.hpp"

class LocalPlayer
{
private:

    LocalPlayer() {}
    ~LocalPlayer() {}

    ulong _basePointer;
    bool _isValid;
    bool _isDead;
    bool _isInAttack;
    bool _isKnocked;
    int _team;
    Vector3d _position;
    Vector3d _cameraPosition;

    QAngle _weaponPunch;
    QAngle _breathAngles;
    QAngle _viewAngle;
    Weapon _weapon;

    ulong readBasePointer() const {
        long ptr = Offsets::getInstance().region + Offsets::getInstance().localPlayer;
        return Memory::getInstance().read<ulong>(ptr);
    }

    int readTeam() const {
        ulong ptr = _basePointer + Offsets::getInstance().team;
        return Memory::getInstance().read<int>(ptr);  
    }

    bool readIsKnocked() const {
        ulong ptr = _basePointer + Offsets::getInstance().bleedOutState;
        return Memory::getInstance().read<short>(ptr) > 0;
    }

    bool readIsDead() {
        ulong ptr = _basePointer + Offsets::getInstance().lifeState;
        return Memory::getInstance().read<short>(ptr) > 0;
    }

    Vector3d readPosition() const {
        ulong ptr = _basePointer + Offsets::getInstance().localOrigin;
        return Memory::getInstance().read<Vector3d>(ptr);  
    }

    bool readIsInAttack() const {
        long ptr = Offsets::getInstance().region + Offsets::getInstance().inAttack;
        return Memory::getInstance().read<short>(ptr) > 0;
    }

    QAngle readWeaponPunch() const {
        ulong ptr = _basePointer + Offsets::getInstance().weaponPunch;
        return Memory::getInstance().read<QAngle>(ptr);
    }

    QAngle readBreathAngles() const {
        ulong ptr = _basePointer + Offsets::getInstance().breathAngles;
        return Memory::getInstance().read<QAngle>(ptr);
    }

    QAngle readViewAngle() const {
        ulong ptr = _basePointer + Offsets::getInstance().viewAngle;
        return Memory::getInstance().read<QAngle>(ptr).fixAngle();
    }

    void writeViewAngle(QAngle viewAngle) const {
        ulong ptr = _basePointer + Offsets::getInstance().viewAngle;
        Memory::getInstance().write(ptr, viewAngle);
    }

    Vector3d readCameraPosition() const {
        ulong ptr = _basePointer + Offsets::getInstance().cameraPosition;
        return Memory::getInstance().read<Vector3d>(ptr);
    }

    ulong readWeaponIndex() const {
        ulong ptr = _basePointer + Offsets::getInstance().latestPrimaryWeapons;
        return Memory::getInstance().read<ulong>(ptr) & 0xFFFF;
    }

    void setAsInvalid() {
        _isValid = false;
        _position = Vector3d::zero();
        _cameraPosition = Vector3d::zero();
        _isInAttack = false;
        _isKnocked = false;
        _isDead = false;
        _team = -1;
        _weaponPunch = QAngle::zero();
        _viewAngle = QAngle::zero();
    }

public:

    static LocalPlayer& getInstance() {
        static LocalPlayer instance;
        return instance;
    }

    LocalPlayer(const LocalPlayer&) = delete;
    LocalPlayer& operator=(const LocalPlayer&) = delete;

    void update() {
        _basePointer = readBasePointer();
        _isValid = Memory::isValidPointer(_basePointer);
        if(!_isValid) {
            setAsInvalid();
            return;
        }

        _team = readTeam();
        _isKnocked = readIsKnocked();
        _isDead = readIsDead();
        _position = readPosition();
        _cameraPosition = readCameraPosition();
        _isInAttack = readIsInAttack();
        _weaponPunch = readWeaponPunch();
        _breathAngles = readBreathAngles();
        _viewAngle = readViewAngle();
        _weapon.update(readWeaponIndex());
    }

    ulong getBasePointer() const {
        return _basePointer;
    }

    bool isValid() const {
        return _isValid;
    }

    bool isKnocked() const {
        return _isKnocked;
    }

    bool isDead() const {
        return _isDead;
    }

    int getTeam() const {
        return _team;
    }

    Vector3d getPosition() const {
        return _position;
    }

    Vector3d getCameraPosition() const {
        return _cameraPosition;
    }

    bool isInAttack() const {
        return _isInAttack;
    }

    QAngle getWeaponPunch() const {
        return _weaponPunch;
    }

    QAngle getSwayAngles() {
        return _breathAngles;
    }

    QAngle getViewAngle() const {
        return _viewAngle;
    }

    void setViewAngle(QAngle viewAngle) {
        if(!viewAngle.isValid()) {
            return;
        }

        viewAngle.fixAngle();
        _viewAngle = viewAngle;

        if (_viewAngle.x > 89.0f)
            return;

        if (_viewAngle.x < -89.0f)
            return;

        if (_viewAngle.y > 180.f)
            return;

        if (_viewAngle.y < -180.f)
            return;

        writeViewAngle(viewAngle);
    }

    const Weapon& getWeapon() const {
        return _weapon;
    }
};
