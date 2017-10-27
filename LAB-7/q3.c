#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	int fd1, fd2, num;
	fd1 = open(argv[1], O_RDONLY);
	fd2 = open(argv[2], O_WRONLY|O_CREAT|O_APPEND|O_TRUNC, 0644);

	// hw.txt becomes the standard input
	if(dup2(fd1, 0) == -1) {
		printf("Error\n");
		return -1;
	}

	// integer is taken from hw.txt instead of standard input
	scanf("%d", &num);

	// hw-copy.txt becomes the standard output
	if(dup2(fd2, 1) == -1) {
		printf("Error\n");
		return -1;
	}
	printf("This will go to hw-copy.txt and not standard output\n");
	printf("This is this integer taken as input from hw.txt %d\n", num);
	return 0;
}
