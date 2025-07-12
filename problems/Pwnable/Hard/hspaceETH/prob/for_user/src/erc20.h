#include <iostream>
#include <map>
#define address uint16_t

using namespace std;

extern address USER;

class ERC20 {
public:
    ERC20();
    ERC20(address user);
    uint32_t balanceOf(address account);
    void approve(address spender, uint32_t amount);
    uint32_t allowance(address owner, address spender);
    void transfer(address to, uint32_t amount);
    void transferFrom(address sender, address recipient, uint32_t amount);
    void check(address account);

private:
    map<address, uint32_t> _balance;
    map<address, map<address, uint32_t>> _allowance;
    uint32_t _totalSupply;

    void _spendAllowance(address owner, address spender, uint32_t value);
    void _mint(address account, uint32_t value);
    void _update(address from, address to, uint32_t value);
};

address privkey_to_wallet(uint32_t privkey);