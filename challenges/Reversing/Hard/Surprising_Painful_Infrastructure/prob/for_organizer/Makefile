CC = gcc
CFLAGS = -Wall -Wextra -O2

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

TARGET = chall

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
