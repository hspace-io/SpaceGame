#include <iostream>
#include <map>
#include <cstring>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <openssl/sha.h>
#include "erc20.h"

using namespace std;

address USER;

class RevertException : public runtime_error {
public:
    RevertException(const string& message) : runtime_error(message) {}
};

ERC20::ERC20() {
}

ERC20::ERC20(address user) {
    for (int i=0; i<10; i++) {
        address rand_addr = rand() % 0x10000;
        uint32_t rand_val = rand() % 100000;

        this->_mint(rand_addr, rand_val);
    }    
    this->_mint(user, 16);
}

uint32_t ERC20::balanceOf(address account) {
    return this->_balance[account];
}

void ERC20::approve(address spender, uint32_t amount) {
    this->_allowance[USER][spender] = amount;
}

uint32_t ERC20::allowance(address owner, address spender) {
    if (owner == spender)
        return this->balanceOf(owner);
    return this->_allowance[owner][spender];
}

void ERC20::transfer(address to, uint32_t amount) {
    try {
        if (this->_balance[USER] >= amount) {
            this->_update(USER, to, amount);
            cout << "Transferred " << amount << " tokens to address " << to << "." << endl;
        } else {
            throw RevertException("Tx failed: insufficient balance.");
        }
    } catch (const RevertException& e) {
        cout << "[!] " << e.what() << endl;
    }
}

void ERC20::transferFrom(address sender, address recipient, uint32_t amount) {
    try {
        uint32_t currentAllowance = this->allowance(sender, USER);
        if (currentAllowance >= amount && this->_balance[sender] >= amount) {
            this->_spendAllowance(sender, USER, amount);
            this->_update(sender, recipient, amount);
            cout << "Transferred " << amount << " tokens from address " << sender << " to address " << recipient << "." << endl;
        } else {
            throw RevertException("Tx failed: insufficient balance or allowance.");
        }
    } catch (const RevertException& e) {
        cout << "[!] " << e.what() << endl;
    }
}

void ERC20::_spendAllowance(address owner, address spender, uint32_t value) {
    if (owner != spender)
        this->_allowance[owner][spender] -= value;
}

void ERC20::_mint(address account, uint32_t value) {
    this->_balance[account] += value;
    this->_totalSupply += value;
}

void ERC20::_update(address from, address to, uint32_t value) {
    uint32_t from_balance = this->_balance[from];
    uint32_t to_balance = this->_balance[to];
    this->_balance[from] = from_balance - value;
    this->_balance[to] += to_balance + value;
}

void ERC20::check(address account) {
    try {
        if (this->balanceOf(account) > 10000000) {
            char buf[0x200];
            memset(buf, 0, 0x200);
            int fd = open("flag", 0);
            if (fd < 0)
                throw RevertException("Failed to open flag, contact admin please.");
            read(fd, buf, 0x200);
            puts(buf);
            exit(0);
        } else {
            throw RevertException("Tx failed: insufficient balance.");
        }
    } catch (const RevertException& e) {
        cout << "[!] " << e.what() << endl;
    }    
}

address privkey_to_wallet(uint32_t privkey) {

    unsigned char privkey_bytes[sizeof(privkey)];
    for (int i = 0; i < sizeof(privkey); ++i)
        privkey_bytes[i] = (privkey >> (i * 8)) & 0xFF;

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(privkey_bytes, sizeof(privkey_bytes), hash);

    address addr = (hash[0] << 8) | hash[1];
    return addr;
}