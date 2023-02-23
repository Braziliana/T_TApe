#pragma once
#include <iostream>
#include <sstream>
#include <sys/uio.h>
#include <cmath>
#include <string>
#include <vector>
#include "Strings.hpp"

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
        FILE *cmd_pipe = popen("pidof -s R5Apex.exe", "r");
        fgets(buf, 512, cmd_pipe);
        pid_t pid = strtoul(buf, NULL, 10);
        pclose(cmd_pipe);

        return pid;
    }

    bool read(ulong address, void *pBuff, size_t size) {
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
        bool success = read(address, &buffer, size);

        if (!success) {
            throw new std::invalid_argument(concatenateStrings("Failed to read ", typeid(T).name(), " at address: ", convertPointerToHexString(address)));
        }
        
        return buffer;
    }

    template<typename T>
    void write(ulong address, T& buffer)
    {
        int size = sizeof(T);
        bool success = write(address, &buffer, size);

        if (!success) {
            throw new std::invalid_argument(concatenateStrings("Failed to write ", typeid(T).name(), " at address: ", convertPointerToHexString(address)));
        }
    }

    std::string readString(ulong address, int length) {
        char buffer[length] = {0};
        bool success = read(address, &buffer, length);
        
        if (!success)
            throw new std::invalid_argument(concatenateStrings("Failed to read string with size: ", length, " at address: ", convertPointerToHexString(address)));

        buffer[length-1] = '\0';

        return std::string(buffer);
    }

    static std::string convertPointerToHexString(ulong pointer)
    {
        std::stringstream stream;
        stream << "0x" << std::hex << pointer;
        std::string result(stream.str());
        return result;
    }
};



/*


class ProcessMemoryMap {
public:
    ProcessMemoryMap(pid_t pid) : pid_(pid) {}

    bool refresh() {
        std::ifstream maps_file("/proc/" + std::to_string(pid_) + "/maps");
        if (!maps_file.is_open()) {
            return false;  // Failed to open maps file
        }

        mappings_.clear();
        std::string line;
        while (std::getline(maps_file, line)) {
            std::istringstream iss(line);
            unsigned long long start, end;
            char dash, r, w, x, p;
            unsigned int offset, major, minor, inode;
            std::string dev, pathname;

            // Parse the line into its fields
            if (!(iss >> std::hex >> start >> dash >> end >> r >> w >> x >> p >> offset >> dev >> std::dec >> major >> dash >> minor >> inode >> pathname)) {
                continue;  // Failed to parse the line
            }

            // Add the mapping to the cache
            mappings_.push_back({start, end});
        }

        maps_file.close();
        return true;
    }

    const std::vector<MemoryMapping>& mappings() const {
        return mappings_;
    }

private:
    pid_t pid_;
    std::vector<MemoryMapping> mappings_;
};


bool is_valid_address(pid_t pid, void* addr) {
    static ProcessMemoryMap mem_map(pid);

    if (!mem_map.mappings().size() || !is_address_in_range(mem_map.mappings(), addr)) {
        if (!mem_map.refresh() || !is_address_in_range(mem_map.mappings(), addr)) {
            return false;
        }
    }

    return true;
}

bool is_address_in_range(const std::vector<MemoryMapping>& mappings, void* addr) {
    for (const auto& mapping : mappings) {
        if (addr >= reinterpret_cast<void*>(mapping.start) && addr < reinterpret_cast<void*>(mapping.end)) {
            return true;
        }
    }

    return false;
}
*/