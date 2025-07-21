#include <iostream>
#include <sstream>
#include <vector>
#include <ostream>
#include "EVM.h"
#include "Memory.h"
#include "uint256.h"
using namespace std;

int main() {
    EVM *evm = new EVM();
    while (true) {
        // 입력 받은 헥사 스트림
        string bytecode;
        cout << "Enter bytecode as hex (6080...): ";
        getline(cin, bytecode);
        if (bytecode.empty()) break;

        // 헥사 스트림을 vector<uint8_t>로 변환
        vector<uint8_t> code;
        stringstream ss;
        for (size_t i = 0; i < bytecode.size(); i += 2) {
            string byteString = bytecode.substr(i, 2);
            uint8_t byte = stoi(byteString, nullptr, 16);
            // std::cout << "byte: " << std::hex << byteString << std::endl;
            code.push_back(byte);
        }
        evm->loadProgram(code);
        uint256 ret = evm->execute();

        cout << "Return value: ";
        for (auto byte : ret.to_bytes32()) {
            cout << hex << (int)byte << " ";
        }
        cout << endl;
        // print whole stack
        // cout << "Stack: ";
        // while (!evm->stk.empty()) {
        //     uint256 val = evm->stk.top();
        //     evm->stk.pop();
        //     cout << val.to_uint64() << " ";
        // }
    }

    return 0;
}
