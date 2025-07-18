# PhysicalLab-Whisperer

# Usage
docker compose up --build -d

# Concept
- hardware

# Writeup

prob 바이너리는 제공되지 않고 서버 주소만 제공된다. info.txt, code.ipynb 파일만이 제공된다. 

이 문제는 아래 과정을 통해 풀이할 수 있다. 
- [ ]  Datasheet 찾아서 타겟 칩 모델명 알아내기
- [ ]  해당 칩의 데이터시트를 통해 핀맵 확인하기
- [ ]  ChipWhisperer - Trezor pin 연결
- [ ]  SWD - Trezor pin 연결
- [ ]  glitching configuration 설정
- [ ]  글리칭 start

**칩 모델명 알아내기**

문제에서 주어진 타겟 기기(Trezor one)의 datasheet를 찾아서 칩 모델명 알아내기

- https://mcudev.github.io/trezor-dev-kit/

→ STM32F205

칩 모델명을 알맞게 입력하면 메뉴 선택으로 넘어간다. 

**메뉴 설명**

```cpp
====== Choose an action ======
1. Show ChipWhisperer
2. Connect Pins
3. Setup glitch configuration
4. Start glitching
5. Exit
```

1. ChipWhisperer-Husky의 모든 포트/핀들을 나타내줌
2. ChipWhisperer, SWD, Trezor 사이의 핀 연결
3. glitching을 주기 위한 configuration 설정 (ChipWhisperer code)
4. 모든 환경, 설정이 세팅되었다면 글리칭 start를 통해 성공/실패 여부 확인
    → 성공이면 flag 출력
5. 종료

**ChipWhisperer 포트/핀 확인** 

1. show ChipWhisperer 메뉴를 통해 확인 가능하다. 

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/5e9d3964-8271-4668-a6e3-e4abbb2221ec/41e5e1ad-1d3e-4055-8e37-895027569151/image.png)

**SWD 핀 확인**

info.txt의 [DEVICE INFORMATION]에서 주어진 SWD 링크로 이동하면 어떤 핀들이 있는지 확인 가능하다. 

**핀 연결**

STM32F205의 datasheet를 참고한다. 
[STM32F205RG PDF](https://pdf1.alldatasheet.com/datasheet-pdf/view/474805/STMICROELECTRONICS/STM32F205RG.html)

info.txt에서 pinout 64라는 정보를 줬기 때문에 해당 pin map을 확인하면 된다. 

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/5e9d3964-8271-4668-a6e3-e4abbb2221ec/42e70f21-8d0c-4519-a3df-67d8d4bafc6d/image.png)

info.txt의 [MEMO]에서 주어진 정보에 의해
C.W의 GND와 Trezor의 GND(VSS)는 연결할 필요가 없다. 
글리칭은 Crowbar 포트를 사용한다. 

아래와 같이 핀을 연결하면 된다. 
C.W Crowbar → Trezor Vcap
C.W NRST → Trezor NRST
SWD SWIO → Trezor PA13
SWD CLK → Trezor PA14
SWD SWO → Trezor PB3
SWD Vref → Trezor VDD


**glitch configuration 설정**

```cpp
// scope.adc.basic_mode = "rising_edge"
// scope.clock.clkgen_src = "system"
// scope.glitch.enabled = True
// scope.glitch.clk_src = 'pll'
// scope.glitch.trigger_src = "ext_single"
// scope.glitch.output = "enable_only"
// scope.trigger.triggers = "nrst"
// scope.io.nrst = "high_z"

// scope.io.glitch_lp = True
// scope.io.glitch_hp = False
```

ChipWhisperer의 Scope API 문서를 참조하여 글리칭을 위한 세팅을 해줘야 한다. 

[Scope API — ChipWhisperer 5.7.0 documentation](https://chipwhisperer.readthedocs.io/en/latest/scope-api.html)

ChipWhisperer의 공식 docs와 주어진 글리칭 코드(code.ipynb)를 참조하여 configuration을 설정한다. 

**글리칭 시도**

4번 메뉴를 통해 글리칭을 시도해도 된다. 

4초 sleep 후 핀 연결과 글리칭 conf 설정이 모두 완료된 경우 flag가 출력됨. 

**익스 코드**

아래 코드를 통해 flag를 획득할 수 있다. 

```python
from pwn import *

def connectPin(num, pin1, pin2):
    r.sendlineafter(b"choice: ", b"2")
    r.sendlineafter(b"num: ", str(num))
    r.sendlineafter(b"connect: ", pin1)
    r.sendlineafter(b"connect: ", pin2)

# r = process('./pysicallab_whisperer')
r = remote('')

r.sendlineafter(": ", b"STM32F205")

connectPin(1, b"Crowbar", b"Vcap")
connectPin(1, b"NRST", b"NRST")
connectPin(2, b"SWIO", b"PA13")
connectPin(2, b"CLK", b"PA14")
connectPin(2, b"SWO", b"PB3")
connectPin(2, b"Vref", b"VDD")

r.sendlineafter(b"choice: ", b"3")
r.sendlineafter(b"= ", b"rising_edge")
r.sendlineafter(b"= ", b"system")
r.sendlineafter(b"= ", b"True")
r.sendlineafter(b"= ", b"pll")
r.sendlineafter(b"= ", b"ext_single")
r.sendlineafter(b"= ", b"enable_only")
r.sendlineafter(b"= ", b"nrst")
r.sendlineafter(b"= ", b"high_z")
r.sendlineafter(b"= ", b"True")
r.sendlineafter(b"= ", b"False")

r.sendlineafter(b"choice: ", b"4")
r.recvuntil(b'hspace')
flag = r.recvline()
print(flag)

r.interactive()
```
