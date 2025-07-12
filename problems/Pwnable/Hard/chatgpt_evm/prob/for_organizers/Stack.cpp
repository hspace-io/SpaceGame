#include "Stack.h"
#include <sys/mman.h>
#include <stdexcept>
#include <cstring>

#define STACK_SIZE 0x3000

Stack::Stack() : capacity(STACK_SIZE) {
    allocateMemory();
}

Stack::~Stack() {
    deallocateMemory();
}

void Stack::allocateMemory() {
    begin = static_cast<uint8_t*>(mmap(nullptr, capacity, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    if (begin == MAP_FAILED) {
        throw std::runtime_error("mmap failed");
    }
    end = begin + capacity;
    current = begin;
}

void Stack::deallocateMemory() {
    if (begin != MAP_FAILED) {
        munmap(begin, capacity);
    }
}

void Stack::push(const uint256& value) {
    if (current + sizeof(value) > end) {
        throw std::runtime_error("Stack overflow");
    }
    std::memcpy(current, value.to_bytes32().data(), sizeof(value));
    current += sizeof(value);
}

uint256 Stack::top() {
    if (current == begin) {
        throw std::runtime_error("Stack underflow");
    }
    uint256 value(current - sizeof(uint256), sizeof(uint256));
    return value;
}

uint256 Stack::pop() {
    if (current == begin) {
        throw std::runtime_error("Stack underflow");
    }
    current -= sizeof(uint256);
    uint256 value(current, sizeof(uint256));
    return value;
}

bool Stack::empty() const {
    return current == begin;
}

size_t Stack::size() const {
    return (current - begin) / sizeof(uint256);
}
