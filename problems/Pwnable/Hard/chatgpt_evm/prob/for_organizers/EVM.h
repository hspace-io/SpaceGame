#ifndef EVM_H
#define EVM_H

#include <vector>
#include <stack>
#include <unordered_map>
#include <stdexcept>
#include <cstdint>
#include <iostream>
#include "uint256.h"
#include "Memory.h"
#include "Stack.h"

using namespace std;

enum OpCode {
    STOP = 0x00,
    ADD = 0x01,
    MUL = 0x02,
    SUB = 0x03,
    DIV = 0x04,
    SDIV = 0x05,
    MOD = 0x06,
    SMOD = 0x07,
    ADDMOD = 0x08,
    MULMOD = 0x09,
    EXP = 0x0a,
    SIGNEXTEND = 0x0b,
    MLOAD = 0x51,
    MSTORE = 0x52,
    MSTORE8 = 0x53,
    SLOAD = 0x54,
    SSTORE = 0x55,
    PUSH1 = 0x60,
    PUSH32 = 0x7f,
    DUP1 = 0x80,
    DUP16 = 0x8f,
    SWAP1 = 0x90,
    SWAP16 = 0x9f,
};

class EVM {
public:
    EVM();
    void loadProgram(const vector<uint8_t>& code);
    uint256 execute();

private:
    void (EVM::*opcodes[256])();
    std::vector<uint8_t> code;
    Stack stk;
    size_t pc;
    std::unordered_map<uint256, uint256> storage;
    Memory memory;

    void opStop();
    void opAdd();
    void opMul();
    void opSub();
    void opDiv();
    void opSdiv();
    void opMod();
    void opSmod();
    void opAddmod();
    void opMulmod();
    void opExp();
    void opSignextend();
    void opPush();
    void opDup();
    void opSwap();
    void unknownOp();
    void opMload();
    void opMstore();
    void opMstore8();
    void opSload();
    void opSstore();

    template <typename F>
    void binaryOp(F func);
    template <typename F>
    void ternaryOp(F func);
};

#endif // EVM_H
