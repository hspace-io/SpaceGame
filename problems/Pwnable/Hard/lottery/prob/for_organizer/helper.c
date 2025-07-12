#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
int main()
{
	setbuf(stdout, 0);
	pid_t pid = fork();
	if (pid == -1)
	{
		puts("fork failed. please call admin");
		exit(-1);
	}
	else if (pid != 0) //parent
	{	
		usleep(500000);
		char buf[0x100] = {0,};
		snprintf(buf, sizeof(buf), "cat /proc/%d/maps", pid);
		puts("Use this memory map as a guide to write remote exploit code.");
		puts("This binary is not intended for exploitation! No vulnerabilities exist.");
		system(buf);
		kill(pid, SIGKILL);
	}
	else //child
	{
		close(1);
		char* tmp[2];
	    tmp[0] = "/home/lottery/lottery";
		tmp[1] = NULL;
		execve(tmp[0], tmp, NULL);
	}
}
