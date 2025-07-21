#include "EVM.h"

EVM::EVM() {
    for (int i = 0; i < 256; ++i) {
        opcodes[i] = &EVM::unknownOp;
    }
    opcodes[STOP] = &EVM::opStop;
    opcodes[ADD] = &EVM::opAdd;
    opcodes[MUL] = &EVM::opMul;
    opcodes[SUB] = &EVM::opSub;
    opcodes[DIV] = &EVM::opDiv;
    opcodes[SDIV] = &EVM::opSdiv;
    opcodes[MOD] = &EVM::opMod;
    opcodes[SMOD] = &EVM::opSmod;
    opcodes[ADDMOD] = &EVM::opAddmod;
    opcodes[MULMOD] = &EVM::opMulmod;
    opcodes[EXP] = &EVM::opExp;
    opcodes[SIGNEXTEND] = &EVM::opSignextend;
    opcodes[MLOAD] = &EVM::opMload;
    opcodes[MSTORE] = &EVM::opMstore;
    opcodes[MSTORE8] = &EVM::opMstore8; 
    opcodes[SLOAD] = &EVM::opSload;
    opcodes[SSTORE] = &EVM::opSstore;
    for (int i = PUSH1; i <= PUSH32; ++i) {
        opcodes[i] = &EVM::opPush;
    }
    for (int i = DUP1; i <= DUP16; ++i) {
        opcodes[i] = &EVM::opDup;
    }
    for (int i = SWAP1; i <= SWAP16; ++i) {
        opcodes[i] = &EVM::opSwap;
    }
}

void EVM::loadProgram(const std::vector<uint8_t>& code) {
    this->code = code;
}

uint256 EVM::execute() {
    pc = 0;
    while (!stk.empty()) stk.pop();
    memory.Resize(32);

    while (pc < code.size()) {
        uint8_t opcode = code[pc];
        (this->*opcodes[opcode])();
    }

    if(!stk.empty()) {
        return stk.top();
    }
    return uint256(0);
}

void EVM::opStop() {
    pc = code.size();
}

void EVM::opAdd() {
    binaryOp([](const uint256& a, const uint256& b) { return a + b; });
}

void EVM::opMul() {
    binaryOp([](const uint256& a, const uint256& b) { return a * b; });
}

void EVM::opSub() {
    binaryOp([](const uint256& a, const uint256& b) { return a - b; });
}

void EVM::opDiv() {
    binaryOp([](const uint256& a, const uint256& b) {
        if (b == uint256(0)) throw std::runtime_error("Division by zero");
        return a / b;
    });
}

void EVM::opSdiv() {
    binaryOp([](const uint256& a, const uint256& b) {
        if (b == uint256(0)) throw std::runtime_error("Division by zero");
        return a / b;
    });
}

void EVM::opMod() {
    binaryOp([](const uint256& a, const uint256& b) {
        if (b == uint256(0)) throw std::runtime_error("Modulo by zero");
        return a % b;
    });
}

void EVM::opSmod() {
    binaryOp([](const uint256& a, const uint256& b) {
        if (b == uint256(0)) throw std::runtime_error("Modulo by zero");
        return a % b;
    });
}

void EVM::opAddmod() {
    ternaryOp([](const uint256& a, const uint256& b, const uint256& c) {
        if (c == uint256(0)) throw std::runtime_error("Modulo by zero");
        return (a + b) % c;
    });
}

void EVM::opMulmod() {
    ternaryOp([](const uint256& a, const uint256& b, const uint256& c) {
        if (c == uint256(0)) throw std::runtime_error("Modulo by zero");
        return (a * b) % c;
    });
}

void EVM::opExp() {
    binaryOp([](const uint256& base, const uint256& exp) {
        uint256 result(1);
        uint256 b(base);
        uint256 e(exp);

        while (e != uint256(0)) {
            if (e % uint256(2) == uint256(1)) {
                result *= b;
            }
            e /= uint256(2);
            b *= b;
        }
        return result;
    });
}

void EVM::opSignextend() {
    binaryOp([](const uint256& b, const uint256& x) {
        uint64_t k = b.to_uint64();
        if (k < 31) {
            uint64_t m = 8 * (k + 1) - 1;
            uint256 mask = (uint256(1) << m) - 1;
            if ((x & (uint256(1) << m)) != uint256(0)) {
                return x | ~mask;
            } else {
                return x & mask;
            }
        }
        return x;
    });
}

void EVM::opPush() {
    uint8_t n = code[pc] - PUSH1 + 1;
    if (pc + n >= code.size()) {
        throw std::runtime_error("Out of bounds on PUSH");
    }
    uint256 value(&code[pc + 1], n);
    stk.push(value);
    pc += n + 1;
}

void EVM::opDup() {
    uint8_t n = code[pc] - DUP1 + 1;
    if (stk.size() < n) {
        throw std::runtime_error("Stack underflow on DUP");
    }
    std::vector<uint256> temp;
    for (int i = 0; i < n; ++i) {
        temp.push_back(stk.top());
        stk.pop();
    }
    uint256 value = temp.back();
    for (auto it = temp.rbegin(); it != temp.rend(); ++it) {
        stk.push(*it);
    }
    stk.push(value);
    pc++;
}

void EVM::opSwap() {
    uint8_t n = code[pc] - SWAP1 + 1;
    if (stk.size() < n + 1) {
        throw std::runtime_error("Stack underflow on SWAP");
    }
    std::vector<uint256> temp(n + 1);
    for (int i = 0; i <= n; ++i) {
        temp[i] = stk.top();
        stk.pop();
    }
    std::swap(temp[0], temp[n]);
    for (int i = n; i >= 0; --i) {
        stk.push(temp[i]);
    }
    pc++;
}

void EVM::unknownOp() {
    std::cerr << "Unknown opcode: " << std::hex << static_cast<int>(code[pc]) << std::endl;
    pc = code.size();
}

void EVM::opMload() {
    if (stk.size() < 1) {
        throw std::runtime_error("Stack underflow on MLOAD");
    }
    uint256 offset = stk.top(); stk.pop();
    uint256 value;
    std::vector<uint8_t> data = memory.GetCopy(offset.to_uint64(), 32);
    value = uint256(data.data(), data.size());
    stk.push(value);
    pc++;
}

void EVM::opMstore() {
    if (stk.size() < 2) {
        throw std::runtime_error("Stack underflow on MSTORE");
    }
    uint256 offset = stk.top(); stk.pop();
    uint256 value = stk.top(); stk.pop();
    memory.Resize(offset.to_uint64() + 32);
    memory.Set32(offset.to_uint64(), value);
    pc++;
}

void EVM::opMstore8() {
    if (stk.size() < 2) {
        throw std::runtime_error("Stack underflow on MSTORE8");
    }
    uint256 offset = stk.top(); stk.pop();
    uint256 value = stk.top(); stk.pop();
    memory.Resize(offset.to_uint64() + 1);
    memory.Set(offset.to_uint64(), 1, std::vector<uint8_t>{static_cast<uint8_t>(value.to_uint64() & 0xff)});
    pc++;
}


void EVM::opSload() {
    if (stk.size() < 1) {
        throw std::runtime_error("Stack underflow on SLOAD");
    }
    uint256 key = stk.top(); stk.pop();
    stk.push(storage[key]);
    pc++;
}

void EVM::opSstore() {
    if (stk.size() < 2) {
        throw std::runtime_error("Stack underflow on SSTORE");
    }
    uint256 key = stk.top(); stk.pop();
    uint256 value = stk.top(); stk.pop();
    storage[key] = value;
    pc++;
}

template <typename F>
void EVM::binaryOp(F func) {
    if (stk.size() < 2) {
        throw std::runtime_error("Stack underflow");
    }
    uint256 a = stk.top(); stk.pop();
    uint256 b = stk.top(); stk.pop();

    stk.push(func(a, b));
    pc++;
}

template <typename F>
void EVM::ternaryOp(F func) {
    if (stk.size() < 3) {
        throw std::runtime_error("Stack underflow");
    }
    uint256 a = stk.top(); stk.pop();
    uint256 b = stk.top(); stk.pop();
    uint256 c = stk.top(); stk.pop();
    stk.push(func(a, b, c));
    pc++;
}
