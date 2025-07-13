CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -fno-stack-protector

TARGET = cli-painter
OBJS = cli-painter.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
