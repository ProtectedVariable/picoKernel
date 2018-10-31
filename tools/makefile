all: fs_create fs_add fs_del fs_list fs_info

fs_%: fs%.o fsutil.o
	gcc $^ -o $@ -lm

fsutil.o: fsutil.c fsutil.h
	gcc -Wall -Wextra -std="gnu11" -c fsutil.c -o fsutil.o

%.o: %.c
	gcc -Wall -Wextra -std="gnu11" -c $^ -o $@

clean:
	-rm *.o
	-rm fs_*
