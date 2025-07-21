///usr/bin/arm-linux-gnueabi-gcc-11 -no-pie -o prob prob.c -static

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#define BUF_SIZE 0x1000

// 문제 컨샙
// 0. arm 아키택처 바이너리
// 1. input 으로 들어온 가상 인스트럭션들을 처리하는 로직 
// 2. 기본적으로 모든 함수 symbols들은 strip을 할 예정

char reg[10];
char stack[100];
int top;

void init(){
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
}

void mov(char idx, char data){
	reg[idx] = data;
}

void add(char idx, char data){
	reg[idx] += data;
}

void sub(char idx, char data){
	reg[idx] -= data;
}

void mul(char idx, char data){
	reg[idx] *= data;
}

void div_f(char idx, char data){
	reg[idx] /= data;
}

void xor(char idx, char data){
	reg[idx] ^= data;
}

void and(char idx, char data){
	reg[idx] &= data;
}

void or(char idx, char data){
	reg[idx] |= data;
}

void push(char data){
	stack[top] = reg[data];
	top += 1;
}

char pop(){
	top -= 1;
	return stack[top];
}

void cpu(char * buf, int len){

	for(int i=0;i<len;i++){

		switch(buf[i]){
			case '\x10':
				puts("Not syscall...");
				break;
			case '\x20':
				mov(buf[i+1], buf[i+2]);
				i += 2;
				break;
			case '\x30': //add
				add(buf[i+1], buf[i+2]);
				i += 2;
				break;
			case '\x40': //sub
				sub(buf[i+1], buf[i+2]);
				i += 2;
				break;
			case '\x50': //mul
				mul(buf[i+1], buf[i+2]);
				i += 2;
				break;
			case '\x60': //div
				div_f(buf[i+1], buf[i+2]);
				i += 2;
				break;
			case '\x70': //xor
				xor(buf[i+1], buf[i+2]);
				i += 2;
				break;
			case '\x80': //and
				and(buf[i+1], buf[i+2]);
				i += 2;
				break;
			case '\x90': //or
				or(buf[i+1], buf[i+2]);
				i += 2;
				break;
			case '\xa0': //push
				push(buf[i+1]);
				i += 1;
				break;
			case '\xb0': //pop
				reg[buf[i+1]] = pop();
				i += 1;
			default:
				puts("[*] Not Instruction");
				exit(1);
		}
	}

}


void anti_debug(void) __attribute__((constructor));
void anti_debug(){
	if ( ptrace(PTRACE_TRACEME, 0LL, 0LL, 0LL) < 0 ){
    	exit(1);
	}
}

int main(int argc, char * argv[]){

	init();
	char * buf = malloc(BUF_SIZE);
	printf("opcode >>> ");
	int len = read(0, buf, BUF_SIZE);
	cpu(buf, len);
	free(buf);

	return 0;
}
