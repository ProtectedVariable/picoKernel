FLAGS=-fno-builtin -ffreestanding -nostdlib

all: cursor.o display.o util.o

test: all

%.o: %.c %.h
	gcc -m32 -Wall -Wextra -I../kernel $(FLAGS) -std="gnu11" -c $< -o $@

clean:
	-rm *.o -f
