CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11
TARGET = s21_cat

all: $(TARGET)

$(TARGET): s21_cat.c
	rm -f $(TARGET)
	$(CC) $(CFLAGS) -o $(TARGET) s21_cat.c

clean:
	rm -f $(TARGET)