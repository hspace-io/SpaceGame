# 문제이름
My VM

# Usage
- 공란

# Concept
- ARM
- VM

# Writeup
- 풀이 1
<br>
바이너리를 분석하면 알 수 있듯이 사용자로부터 opcode를 입력 받아 mov, add, sum, mul, div, xor, and, or, push, pop 기능을 수행하게 된다. 이 때, 같이 제공된 ``input`` 파일의 데이터를 넣었을 때 수행하는 로직을 python으로 똑같이 구현한 뒤, push된 데이터들을 출력하면 flag를 구할 수 있다.

```py
command = open('./input','rb').read()
pc = 0
reg = [0,0,0,0,0,0,0,0,0,0]
stack = b''
top = 0

while pc < len(command):

    opcode = command[pc]

    if opcode == 0x20:
        reg[command[pc+1]] = command[pc+2]
        pc += 2
    elif opcode == 0x30:
        reg[command[pc+1]] += command[pc+2]
        pc += 2
    elif opcode == 0x40:
        reg[command[pc+1]] -= command[pc+2]
        pc += 2
    elif opcode == 0x50:
        reg[command[pc+1]] *= command[pc+2]
        pc += 2
    elif opcode == 0x60:
        reg[command[pc+1]] //= command[pc+2]
        pc += 2
    elif opcode == 0x70:
        reg[command[pc+1]] ^= command[pc+2]
        pc += 2
    elif opcode == 0x80:
        reg[command[pc+1]] &= command[pc+2]
        pc += 2
    elif opcode == 0x90:
        reg[command[pc+1]] |= command[pc+2]
        pc += 2
    elif opcode == 0xa0:
        stack += bytes([reg[command[pc+1]]])
        top += 1
        pc += 1
    elif opcode == 0xb0:
        top -= 1
        reg[command[pc+1]] = ord(stack[top])
        pc += 1
    pc += 1

print('flag : ',stack)
```
- 풀이 2
<br>
동적 디버깅을 통해 프로그램이 종료되기 직전 push한 값들을 확인하기 위해서는 바이너리에 있는 안티디버깅 로직을 우회할 필요가 있다.
```c
void anti_debug(){
	if ( ptrace(PTRACE_TRACEME, 0LL, 0LL, 0LL) < 0 ){
    	exit(1);
	}
}
```
ptrace 호출 부분을  nop로 패치하는 등의 작업을 통해 안티디버깅을 우회 할 수 있다.
```
<stack>:	0x61707368	0x777b6563	0x5f744168	0x795f3044
<stack+16>:	0x745f7530	0x6b6e3148	0x74315f3f	0x5361575f
<stack+32>:	0x735f615f	0x31706d31	0x4d565f33	0x6731525f
<stack+48>:	0x7d3f7468
```

```
flag : hspace{whAt_D0_y0u_tH1nk?_1t_WaS_a_s1mp13_VM_R1ght?}
```