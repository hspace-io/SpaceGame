#ifndef STACK_H
#define STACK_H

#include <cstdint>
#include <cstddef>
#include "uint256.h"

class Stack {
public:
    Stack();
    ~Stack();

    void push(const uint256& value);
    uint256 pop();
    uint256 top();
    bool empty() const;
    size_t size() const;

private:
    uint8_t* begin;
    uint8_t* end;
    uint8_t* current;
    size_t capacity;

    void allocateMemory();
    void deallocateMemory();
};

#endif // STACK_H
