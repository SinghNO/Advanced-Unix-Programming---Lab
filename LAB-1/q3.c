#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
	// 1st part  
	int fd;
	char *f1, c;
	char *buf = "abcde";
	char *new = "12345";
	int size = strlen(buf);
	if(argc < 2) {
		printf("Insufficient arguments\n");
		return -1;
	}
	f1 = argv[1];
	fd = creat(f1, S_IRWXU);
	write(fd, buf, size);
	close(fd);

	// 2nd part
	fd = open(f1, O_WRONLY | O_APPEND);
	if(fd == -1) {
		perror("Operation Unsuccessful\n");
		return -1;
	}
	// 3rd part
	lseek(fd, 0, SEEK_SET);
	// 4th part
	size = strlen(new);
	write(fd, new, size);
	int ret = read(fd, &c, 1);
	printf("ret = %d\n", ret);
	putchar(c);
	close(fd);
	return 0;
}
