all: fs_create

fs_create: fscreate.o fsutil.o
	gcc $^ -o fs_create -lm

fscreate.o: fscreate.c
	gcc -Wall -Wextra -std="gnu11" -c fscreate.c -o fscreate.o

fsutil.o: fsutil.c fsutil.h
	gcc -Wall -Wextra -std="gnu11" -c fsutil.c -o fsutil.o

clean:
	-rm *.o
	-rm fs_create
