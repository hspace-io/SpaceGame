#ifndef UINT256_H
#define UINT256_H

#include <array>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <functional>  // std::hash 사용

class uint256 {
public:
    uint256();
    uint256(uint64_t value);
    uint256(const uint8_t* data, size_t size);

    std::array<uint8_t, 32> to_bytes32() const;
    uint64_t to_uint64() const;

    uint256 operator+(const uint256& other) const;
    uint256 operator-(const uint256& other) const;
    uint256 operator*(const uint256& other) const;
    uint256 operator/(const uint256& other) const;
    uint256 operator%(const uint256& other) const;

    uint256& operator+=(const uint256& other);
    uint256& operator-=(const uint256& other);
    uint256& operator*=(const uint256& other);
    uint256& operator/=(const uint256& other);
    uint256& operator%=(const uint256& other);

    uint256 operator<<(uint64_t shift) const;
    uint256 operator>>(uint64_t shift) const;
    uint256& operator<<=(uint64_t shift);
    uint256& operator>>=(uint64_t shift);

    bool operator>=(const uint256& other) const;
    bool operator<=(const uint256& other) const;
    bool operator>(const uint256& other) const;
    bool operator<(const uint256& other) const;

    bool operator==(const uint256& other) const;
    bool operator!=(const uint256& other) const;

    uint256 operator&(const uint256& other) const;
    uint256 operator|(const uint256& other) const;
    uint256 operator~() const;

private:
    std::array<uint8_t, 32> data;

    void from_uint64(uint64_t value);
};

// std::hash의 특수화
namespace std {
    template <>
    struct hash<uint256> {
        std::size_t operator()(const uint256& k) const {
            auto bytes = k.to_bytes32();
            std::size_t result = 0;
            for (const auto& byte : bytes) {
                result = (result * 31) ^ byte;
            }
            return result;
        }
    };
}

#endif // UINT256_H
