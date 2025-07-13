#include "Memory.h"

#include <iostream>
Memory::Memory() : lastGasCost(0) {}

void Memory::Set(uint64_t offset, uint64_t size, const std::vector<uint8_t>& value) {
    if (size > 0) {
        if (store.end() < store.begin() + offset + size) {
            throw std::out_of_range("invalid memory: store empty");
        }
        std::copy(value.begin(), value.begin() + size, store.begin() + offset);
    }
}

void Memory::Set32(uint64_t offset, const uint256& val) {
    if (store.end() < store.begin() + offset + 32) {
        throw std::out_of_range("invalid memory: store empty");
    }
    auto b32 = val.to_bytes32();
    std::copy(b32.begin(), b32.end(), store.begin() + offset);
}

void Memory::Resize(int64_t size) {
    if (static_cast<int64_t>(store.size()) < size) {
        store.resize(size, 0);
    }
}

std::vector<uint8_t> Memory::GetCopy(int64_t offset, int64_t size) const {
    if (size == 0) {
        return std::vector<uint8_t>();
    }
    
    if (store.end() >= store.begin() + offset + size) {
        std::vector<uint8_t> cpy(size);
        std::copy(store.begin() + offset, store.begin() + offset + size, cpy.begin());
        return cpy;
    } else {
        throw std::out_of_range("invalid memory: store empty");
    }
    return std::vector<uint8_t>();
}

int Memory::Len() const {
    return store.size();
}

const std::vector<uint8_t>& Memory::Data() const {
    return store;
}

void Memory::Copy(uint64_t dst, uint64_t src, uint64_t len) {
    if (len == 0) {
        return;
    }
    std::copy(store.begin() + src, store.begin() + src + len, store.begin() + dst);
}
