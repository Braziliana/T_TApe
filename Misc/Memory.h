//
// Created by koelion on 05.02.24.
//

#ifndef APEX_MEMORY_H
#define APEX_MEMORY_H

#include <iostream>
#include <sstream>
#include <sys/uio.h>
#include <cmath>
#include <string>
#include <vector>
#include "Strings.h"

struct MemoryMapping {
    ulong start;
    ulong end;
};

class Memory{
private:
    pid_t _pid = 0;
    std::vector<MemoryMapping> _memoryMappings;


    Memory() {}
    ~Memory() {}

    pid_t getPID() {
        char buf[512];
        FILE *cmd_pipe = popen("pidof -s r5apex.exe", "r");
        fgets(buf, 512, cmd_pipe);
        pid_t pid = strtoul(buf, NULL, 10);
        pclose(cmd_pipe);

        return pid;
    }

    bool read_internal(ulong address, void *pBuff, size_t size) {
        if (_pid == 0 || size == 0 || pBuff == nullptr || !isValidPointer(address)) {
            return false;
        }

        struct iovec iovLocalAddressSpace[1]{0};
        struct iovec iovRemoteAddressSpace[1]{0};
        iovLocalAddressSpace[0].iov_base = pBuff;     // Store data in this buffer
        iovLocalAddressSpace[0].iov_len = size;       // which has this size.
        iovRemoteAddressSpace[0].iov_base = (void*)address; // The data comes from here
        iovRemoteAddressSpace[0].iov_len = size;      // and has this size.
        ssize_t sSize = process_vm_readv(_pid, iovLocalAddressSpace, 1, iovRemoteAddressSpace, 1, 0);

        if (sSize == (ssize_t)size)
            return true;

        else if (sSize == 0)
            return false;

        return false;
    }

    bool write(ulong address, void *pBuff, size_t size) {
        if (_pid == 0 || size == 0 || pBuff == nullptr || !isValidPointer(address)) {
            return false;
        }

        struct iovec iovLocalAddressSpace[1]{0};
        struct iovec iovRemoteAddressSpace[1]{0};
        iovLocalAddressSpace[0].iov_base = pBuff;     // Store data in this buffer
        iovLocalAddressSpace[0].iov_len = size;       // which has this size.
        iovRemoteAddressSpace[0].iov_base = (void*)address; // The data will be writted here
        iovRemoteAddressSpace[0].iov_len = size;      // and has this size.
        ssize_t sSize = process_vm_writev(_pid, iovLocalAddressSpace, 1, iovRemoteAddressSpace, 1, 0);

        if (sSize == (ssize_t)size)
            return true;

        else if (sSize == 0)
            return false;

        return false;
    }

public:

    static Memory& getInstance() {
        static Memory instance;
        return instance;
    }

    Memory(const Memory&) = delete;
    Memory& operator=(const Memory&) = delete;

    static bool isValidPointer(ulong pointer) {
        return pointer > 0x00010000 && pointer < 0x7FFFFFFEFFFF;
    }

    bool isReady() {
        if(_pid != 0){
            return true;
        }

        _pid = getPID();

        return _pid != 0;
    }

    template<typename T>
    T read(ulong address)
    {
        int size = sizeof(T);
        T buffer;
        bool success = read_internal(address, &buffer, size);

        if (!success) {
            throw new std::invalid_argument(concatenateStrings("Failed to read ", typeid(T).name(), " at address: ", convertPointerToHexString(address)));
        }

        return buffer;
    }

    template<typename T>
    bool read(ulong address, T* buffer)
    {
        int size = sizeof(T);
        return read_internal(address, buffer, size);
    }

    template<typename T>
    bool write(ulong address, T& buffer)
    {
        int size = sizeof(T);
        return write(address, &buffer, size);

//        if (!success) {
//            throw new std::invalid_argument(concatenateStrings("Failed to write ", typeid(T).name(), " at address: ", convertPointerToHexString(address)));
//        }
    }

    template<typename T>
    bool write2(ulong address, T buffer)
    {
        int size = sizeof(T);
        return write(address, &buffer, size);
    }

    std::string readString(ulong address, int length) {
        char buffer[64] = {0};
        if(length > 64){
            length = 64;
        }
        bool success = read_internal(address, &buffer, length*sizeof(char));

        if (!success)
        {
            return "";
        }

        buffer[length-1] = '\0';

        return std::string(buffer);
    }

    bool readString(ulong address, char* buffer, int length) {
        bool success = read_internal(address, &buffer, length*sizeof(char));

        if(!success)
        {
            buffer[0] = '\0';
            return false;
        }

        //buffer[length-1] = '\0';

        return success;
    }

    static std::string convertPointerToHexString(ulong pointer)
    {
        std::stringstream stream;
        stream << "0x" << std::hex << pointer;
        std::string result(stream.str());
        return result;
    }
};


#endif //APEX_MEMORY_H
