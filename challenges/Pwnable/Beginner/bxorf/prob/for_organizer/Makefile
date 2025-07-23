# Makefile

# 컴파일러와 플래그 정의
CC = gcc
CFLAGS = -c -no-pie -fno-stack-protector
LDFLAGS = -no-pie -fno-stack-protector

# 타겟 실행 파일 이름
TARGET = bxorf

# 소스 파일
SRC = bxorf.c
OBJ = bxorf.o

# 기본 타겟
all: $(TARGET)

# 오브젝트 파일 생성
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) $(SRC)

# 심볼 제거 및 실행 파일 생성
$(TARGET): $(OBJ)
	objcopy --strip-symbol=cat $(OBJ)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJ)

# 정리
clean:
	rm -f $(OBJ) $(TARGET)
