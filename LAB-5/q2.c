#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
	pid_t child;
	int fd1, fd2, status;
	char c[] = "This was written from the parent process\n";
	
	if((fd1 = open("filefork_test1.txt", O_WRONLY | O_CREAT, S_IRUSR	| 
			S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)) == -1) {
		perror("filefork.c: Error while opening file");
		return 1;
	}

	if((write(fd1, c, sizeof(c))) == -1) {
		perror("filefork.c: Error while writing to file");
		return 1;
	}

	if((child = fork()) < 0) {
		perror("filefork.c: Error while forking");
		return 1;
	}
	
	if(child == 0) {
		char d[] = "This was written from the child process\n";
		sleep(3);
		if((write(fd1, d, sizeof(d))) == -1) {
			perror("filefork.c: Error while writing from child process");
			return 1;
		}
		if((write(fd2, d, sizeof(d))) == -1) {
			perror("filefork.c: Error while writing from child process");
			return 1;
		}
		return 0;
	}
	else {
		if((fd2 = open("filefork_test2.txt", O_WRONLY | O_CREAT, S_IRUSR |
				S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)) == -1) {
			perror("filefork.c: Error while opening file");
			return 1;
		}
		if((write(fd2, c, sizeof(c))) == -1) {
			perror("filefork.c: Error while writing to file");
			return 1;
		}
		wait(&status);
		close(fd1);
		close(fd2);
		if(status > 0) {
			printf("filefork.c: Some error occured. Please refer logs above\n");
			return 1;
		}
		else {
			return 0;
		}
	}
	return 0;
}
		
