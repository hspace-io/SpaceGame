#include "uint256.h"
#include <algorithm>
#include <iostream>

uint256::uint256() {
    data.fill(0);
}

uint256::uint256(uint64_t value) {
    from_uint64(value);
}

uint256::uint256(const uint8_t* input, size_t size) {
    if (size > 32) {
        throw std::out_of_range("Input size exceeds 32 bytes");
    }
    data.fill(0);
    std::copy(input, input + size, data.end() - size);
}

std::array<uint8_t, 32> uint256::to_bytes32() const {
    return data;
}

uint64_t uint256::to_uint64() const {
    uint64_t value = 0;
    for (size_t i = 0; i < 8; ++i) {
        value |= static_cast<uint64_t>(data[31 - i]) << (i * 8);
    }
    return value;
}

uint256 uint256::operator+(const uint256& other) const {
    uint256 result = *this;
    result += other;
    return result;
}

uint256 uint256::operator-(const uint256& other) const {
    uint256 result = *this;
    result -= other;
    return result;
}

uint256 uint256::operator*(const uint256& other) const {
    uint256 result = *this;
    result *= other;
    return result;
}

uint256 uint256::operator/(const uint256& other) const {
    uint256 result = *this;
    result /= other;
    return result;
}

uint256 uint256::operator%(const uint256& other) const {
    uint256 result = *this;
    result %= other;
    return result;
}

uint256& uint256::operator+=(const uint256& other) {
    uint64_t carry = 0;
    for (int i = 31; i >= 0; --i) {
        uint64_t sum = static_cast<uint64_t>(data[i]) + other.data[i] + carry;
        data[i] = static_cast<uint8_t>(sum);
        carry = sum >> 8;
    }
    return *this;
}

uint256& uint256::operator-=(const uint256& other) {
    uint64_t borrow = 0;
    for (int i = 31; i >= 0; --i) {
        uint64_t diff = static_cast<uint64_t>(data[i]) - other.data[i] - borrow;
        data[i] = static_cast<uint8_t>(diff);
        borrow = (diff >> 63) & 1;
    }
    return *this;
}

uint256& uint256::operator*=(const uint256& other) {
    uint256 result;
    uint256 multiplier = *this;

    for (int i = 0; i < 256; ++i) {
        if (other.data[31 - (i / 8)] & (1 << (i % 8))) {
            result += (multiplier << i);
        }
    }

    *this = result;
    return *this;
}

uint256& uint256::operator/=(const uint256& other) {
    if (other == uint256(0)) {
        throw std::runtime_error("Division by zero");
    }

    uint256 quotient;
    uint256 remainder = *this;

    for (int i = 255; i >= 0; --i) {
        remainder <<= 1;
        quotient <<= 1;

        if (remainder >= other) {
            remainder -= other;
            quotient.data[31] |= 1;
        }
    }

    *this = quotient;
    return *this;
}

uint256& uint256::operator%=(const uint256& other) {
    if (other == uint256(0)) {
        throw std::runtime_error("Division by zero");
    }

    uint256 remainder = *this;

    for (int i = 255; i >= 0; --i) {
        remainder <<= 1;

        if (remainder >= other) {
            remainder -= other;
        }
    }

    *this = remainder;
    return *this;
}

uint256 uint256::operator<<(uint64_t shift) const {
    uint256 result;
    if (shift >= 256) {
        return result;
    }
    size_t byteShift = shift / 8;
    size_t bitShift = shift % 8;

    for (size_t i = 0; i < 32 - byteShift; ++i) {
        result.data[i] = data[i + byteShift] << bitShift;
        if (i + byteShift + 1 < 32) {
            result.data[i] |= data[i + byteShift + 1] >> (8 - bitShift);
        }
    }
    return result;
}

uint256 uint256::operator>>(uint64_t shift) const {
    uint256 result;
    if (shift >= 256) {
        return result;
    }
    size_t byteShift = shift / 8;
    size_t bitShift = shift % 8;

    for (size_t i = 0; i < 32 - byteShift; ++i) {
        result.data[31 - i] = data[31 - i - byteShift] >> bitShift;
        if (i + byteShift + 1 < 32) {
            result.data[31 - i] |= data[31 - i - byteShift - 1] << (8 - bitShift);
        }
    }
    return result;
}

uint256& uint256::operator<<=(uint64_t shift) {
    *this = *this << shift;
    return *this;
}

uint256& uint256::operator>>=(uint64_t shift) {
    *this = *this >> shift;
    return *this;
}

bool uint256::operator>=(const uint256& other) const {
    for (size_t i = 0; i < 32; ++i) {
        if (data[i] > other.data[i]) return true;
        if (data[i] < other.data[i]) return false;
    }
    return true;
}

bool uint256::operator<=(const uint256& other) const {
    for (size_t i = 0; i < 32; ++i) {
        if (data[i] < other.data[i]) return true;
        if (data[i] > other.data[i]) return false;
    }
    return true;
}

bool uint256::operator>(const uint256& other) const {
    for (size_t i = 0; i < 32; ++i) {
        if (data[i] > other.data[i]) return true;
        if (data[i] < other.data[i]) return false;
    }
    return false;
}

bool uint256::operator<(const uint256& other) const {
    for (size_t i = 0; i < 32; ++i) {
        if (data[i] < other.data[i]) return true;
        if (data[i] > other.data[i]) return false;
    }
    return false;
}

uint256 uint256::operator&(const uint256& other) const {
    uint256 result;
    for (size_t i = 0; i < 32; ++i) {
        result.data[i] = data[i] & other.data[i];
    }
    return result;
}

uint256 uint256::operator|(const uint256& other) const {
    uint256 result;
    for (size_t i = 0; i < 32; ++i) {
        result.data[i] = data[i] | other.data[i];
    }
    return result;
}

uint256 uint256::operator~() const {
    uint256 result;
    for (size_t i = 0; i < 32; ++i) {
        result.data[i] = ~data[i];
    }
    return result;
}

bool uint256::operator==(const uint256& other) const {
    return data == other.data;
}

bool uint256::operator!=(const uint256& other) const {
    return !(*this == other);
}

void uint256::from_uint64(uint64_t value) {
    data.fill(0);
    for (size_t i = 0; i < 8; ++i) {
        data[31 - i] = static_cast<uint8_t>(value);
        value >>= 8;
    }
}
