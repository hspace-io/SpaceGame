# challenge_name

# Usage
문제를 빌드할 수 있는 사용법을 알려주세요

# Concept
- reversing
- hardware
- misc

# Writeup
IoT 장비 취약점 분석을 하기 위해 피지컬랩에서 판매하는 홈캠(PL-CAM-2024)을 구매했다.
PCB에서 EEPROM으로 추정되는 칩과 데이터 시트를 찾았는데, 칩에 쓰여진 데이터를 어떻게 읽을 수 있을까?

- spi-server 접속 정보 제공 (nc war.hspace.io 21346)
- 웹 페이지 링크 제공 (http://war.hspace.io:21345/)
- 데이터 시트(PL25SL128C_DATASHEETv3) 제공 [파일 첨부]

본 문제는 데이터시트를 분석해서 펌웨어를 추출하고 파일 시스템을 분석하여 플래그를 얻는 문제입니다.

1) Flash Memory Firmware Extract
문제의 21346 포트로 nc 접속을 하면, SOP 10pin의 PL25SL128C 칩을 확인할 수 있습니다.
이때, 유저에게 제공되는 PL25SL128C_DATASHEETv3.pdf 파일을 읽어서 칩의 핀맵을 적절하게 매핑해주고 Read Data 코드 번호를 통해 펌웨어를 추출할 수 있습니다.

추출을 위한 칩과 시리얼 포트 매핑은 다음과 같습니다. 
- set_port(CS, 15)
- set_port(DO, 12)
- set_port(VSS, 7)
- set_port(VCC1, 2)
- set_port(VCC2, 4)
- set_port(RP, 3)
- set_port(CLK, 11)
- set_port(DI, 14)

추출 코드
```python
from pwn import *

p = process('./prob')
#p = remote('127.0.0.1', 21346)

CS = 1
DO = 2
HOLD = 3
WP = 4
VSS = 5
VCC1 = 6
VCC2 = 7
RP = 8
CLK = 9
DI = 10

def set_port(index, serial):
    p.sendlineafter(b'> ', b'1')
    p.sendlineafter(b'> ', str(index).encode())
    p.sendlineafter(b'> ', str(serial).encode())

set_port(CS, 15)
set_port(DO, 12)
set_port(VSS, 7)
set_port(VCC1, 2)
set_port(VCC2, 4)
set_port(RP, 3)
set_port(CLK, 11)
set_port(DI, 14)

p.sendlineafter(b'> ', b'2')
p.sendlineafter(b'> ', b'3')
p.sendlineafter(b'> ', b'y')

p.interactive()
```

2) Firmware Analysis
```bash
binwalk HS-PL21345v3.bin
```
위 명령어를 통해 펌웨어에서 파일시스템을 추출하여, 파일시스템 분석을 진행합니다.
squash/web/cgi-bin 경로에서 수상한 바이너리 파일을 확인할 수 있습니다.


3) binary(fd2895412cffbc6a) Analysis
IDA를 통해 바이너리 파일을 분석하면, 쉘 명령어를 받아 실행해주는 웹쉘인 것을 확인할 수 있습니다.
또한, 해당 경로로 접근하면 웹쉘 명령어도 사용 가능합니다. 하지만, flag 파일을 읽을 수 없도록 구성했습니다.

바이너리를 분석하다보면, cmd 파라미터를 가져와서 command_result 함수로 넘기고, 해당 파라미터의 +77 번째 값이 2라면, flag 함수가 실행됩니다.
```bash
http://[url]:[port]/cgi-bin/fd2895412cffbc6a?cmd=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbb%02
```
따라서, 웹 서버에 위와같은 요청을 보내면 flag를 획득할 수 있습니다.


4) 사진을 포함한 writeup은 exploit 경로에 pdf 파일로 확인하실 수 있습니다.
