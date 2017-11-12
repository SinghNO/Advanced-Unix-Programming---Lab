#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {
	int fd, i, ret, rete;
	char prog[256];
	char *arg[2];
	pid_t child;
	if(argc != 2) {
		printf("Usage:\nash <filename>\n");
		return 1;
	}
	
	if((fd = open(argv[1], O_RDONLY)) == -1) {
		perror("ash: Unable to open file");
		return -1;
	}
	
	i = 0;
	while(read(fd, &prog[i++], sizeof(char)));
	prog[i - 2] = '\0';
	arg[0] = prog;
	arg[1] = NULL;
	if((child = vfork()) < 0) {
		perror("ash: Unable to fork new process");
		return -1;
	}
	if(child == 0) {
		if((rete = execvp(prog, arg)) == -1) {
			perror("ash: Error while executing process");
			_exit(-1);
		}
	}
	else {
		wait(&ret);
		if(WIFEXITED(ret)) {
			printf("ash: Process exited with return status %d\n", WEXITSTATUS(ret));
		}
	}
	return 0;
}
