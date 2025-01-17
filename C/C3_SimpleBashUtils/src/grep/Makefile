CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11
TARGET = s21_grep

all: $(TARGET)

$(TARGET): s21_grep.c
	rm -f $(TARGET)
	$(CC) $(CFLAGS) -o $(TARGET) s21_grep.c

clean:
	rm -f $(TARGET)