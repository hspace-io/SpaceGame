#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <cstdint>
#include <stdexcept>
#include "uint256.h" 

class Memory {
public:
    Memory();
    void Set(uint64_t offset, uint64_t size, const std::vector<uint8_t>& value);
    void Set32(uint64_t offset, const uint256& val);
    void Resize(int64_t size);
    std::vector<uint8_t> GetCopy(int64_t offset, int64_t size) const;
    int Len() const;
    const std::vector<uint8_t>& Data() const;
    void Copy(uint64_t dst, uint64_t src, uint64_t len);

private:
    std::vector<uint8_t> store;
    uint64_t lastGasCost;
};

#endif // MEMORY_H
