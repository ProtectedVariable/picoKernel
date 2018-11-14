#include "picolib.h"
#include "../common/util.h"

/**
 * Function to read the entered command in the shell
 * @param buf The buffer in which to write the command
 */
static void read_string(char* buf) {
	char c = 0;
	int counter = 0;
	while ((c = getc()) != '\n') {
		if (c == '\b') {
			if (counter > 0) {
				buf--;
				counter--;
				putc(c);
			}
		} else if (c != '\t') {
			putc(c);
			*buf++ = c;
			counter++;
		}
	}
	*buf = 0;
}

static void cat(char* filename) {
	int fd = file_open(filename);
	if (fd == -1) {
		printf("cat: failed reading \"%s\"\n", filename);
		return;
	}
	int sz = 256;
	char buf[sz + 1];
	int n;
	do {
		n = file_read(fd, buf, sz);
		buf[n] = 0;
		puts(buf);
	} while (n);
	file_close(fd);
}

static void list_files() {
	file_iterator_t it = file_iterator();
	while (file_has_next(&it)) {
		char name[256];
		file_next(name, &it);
		stat_t stat;
		file_stat(name, &stat);
		printf("%s (%d)\n", name, stat.size);
	}
}

static void help() {
	putc('\n');
	puts("ls        : list files present in the file system\n");
	puts("cat FILE  : dump the content of FILE to the screen\n");
	puts("run PROG  : execute the program PROG.\n");
	puts("sleep N   : sleep the specified number of milliseconds\n");
	puts("exit      : exit the shell\n");
}

static void run() {
	puts("A picoKernel = kernel * 10^-12 !\n");
	char buf[512];
	while (1) {
		puts(">");
		read_string(buf);
		char* line = trim(buf);  // remove heading and trailing spaces
		if (line[0] == 0) {
			putc('\n');
			continue;
		} else if (strcmp("help", line) == 0) {
			help();
		} else if (starts_with("run ", line)) {
			char* file = trim(line + strlen("run "));
			putc('\n');
			if (exec(file) == -1) {
				printf("Failed executing \"%s\"\n", file);
			}
		} else if (starts_with("cat ", line)) {
			putc('\n');
			cat(trim(line + strlen("cat ")));
		} else if (starts_with("sleep ", line)) {
			uint ms = atoi(trim(line + strlen("sleep ")));
			putc('\n');
			printf("Sleeping for %dms...\n", ms);
			sleep(ms);
		} else if (strcmp("ls", line) == 0) {
			putc('\n');
			list_files();
		} else if (strcmp("exit", line) == 0) {
			puts("\nBye.\n");
			exit();
		} else {
			putc('\n');
			printf("%s: command not found\n", line);
		}
	}
}

void main() {
	run();
}
