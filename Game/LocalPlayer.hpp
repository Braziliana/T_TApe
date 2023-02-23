#pragma once
#include "../Utils/Memory.hpp"
#include "../Math/QAngle.hpp"
#include "Offsets.hpp"

class LocalPlayer
{
private:

    LocalPlayer() {}
    ~LocalPlayer() {}

    ulong _basePointer;
    bool _isValid;
    bool _isInAttack;
    int _team;
    Vector3d _position;
    Vector3d _cameraPosition;

    QAngle _weaponPunch;
    QAngle _breathAngles;
    QAngle _viewAngle;

    ulong ReadBasePointer()
    {
        long ptr = Offsets::getInstance().region + Offsets::getInstance().localPlayer;
        return Memory::getInstance().read<ulong>(ptr);
    }

    int readTeam() {
        ulong ptr = _basePointer + Offsets::getInstance().team;
        return Memory::getInstance().read<int>(ptr);  
    }

    Vector3d readPosition() {
        ulong ptr = _basePointer + Offsets::getInstance().localOrigin;
        return Memory::getInstance().read<Vector3d>(ptr);  
    }

    bool readIsInAttack() {
        long ptr = Offsets::getInstance().region + Offsets::getInstance().inAttack;
        return Memory::getInstance().read<short>(ptr) > 0;
    }

    QAngle readWeaponPunch() {
        ulong ptr = _basePointer + Offsets::getInstance().weaponPunch;
        return Memory::getInstance().read<QAngle>(ptr);
    }

    QAngle readBreathAngles() {
        ulong ptr = _basePointer + Offsets::getInstance().breathAngles;
        return Memory::getInstance().read<QAngle>(ptr);
    }

    QAngle readViewAngle() {
        ulong ptr = _basePointer + Offsets::getInstance().viewAngle;
        return Memory::getInstance().read<QAngle>(ptr);
    }

    void writeViewAngle(QAngle viewAngle) {
        ulong ptr = _basePointer + Offsets::getInstance().viewAngle;
        Memory::getInstance().write(ptr, viewAngle);
    }

    Vector3d readCameraPosition() {
        ulong ptr = _basePointer + Offsets::getInstance().cameraPosition;
        return Memory::getInstance().read<Vector3d>(ptr);
    }

    void setAsInvalid() {
        _isValid = false;
        _position = Vector3d::zero();
        _cameraPosition = Vector3d::zero();
        _isInAttack = false;
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
        _basePointer = ReadBasePointer();
        _isValid = Memory::isValidPointer(_basePointer);
        if(!_isValid) {
            setAsInvalid();
            return;
        }

        _team = readTeam();
        _position = readPosition();
        _cameraPosition = readCameraPosition();
        _isInAttack = readIsInAttack();
        _weaponPunch = readWeaponPunch();
        _breathAngles = readBreathAngles();
        _viewAngle = readViewAngle();
    }

    ulong getBasePointer() const {
        return _basePointer;
    }

    bool isValid() const {
        return _isValid;
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
        writeViewAngle(viewAngle);
    }
};
