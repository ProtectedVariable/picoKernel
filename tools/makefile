all: fs_create fs_add fs_del

fs_create: fscreate.o fsutil.o
	gcc $^ -o fs_create -lm

fs_add: fsadd.o fsutil.o
	gcc $^ -o fs_add

fs_del: fsdel.o fsutil.o
	gcc $^ -o fs_del

fsdel.o: fsdel.c
	gcc -Wall -Wextra -std="gnu11" -c fsdel.c -o fsdel.o

fsadd.o: fsadd.c
	gcc -Wall -Wextra -std="gnu11" -c fsadd.c -o fsadd.o

fscreate.o: fscreate.c
	gcc -Wall -Wextra -std="gnu11" -c fscreate.c -o fscreate.o

fsutil.o: fsutil.c fsutil.h
	gcc -Wall -Wextra -std="gnu11" -c fsutil.c -o fsutil.o

clean:
	-rm *.o
	-rm fs_create
	-rm fs_add
	-rm fs_del
