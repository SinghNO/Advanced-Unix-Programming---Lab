#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	pid_t pid;
	if((pid = fork())< 0) {
		perror("Fork Failed");
	}
	else if(pid == 0) {
		// child process
		printf("Child Process\n");
		printf("\tReal user id %d\n\tReal group id %d\n\tEffective user id %d\n\tEffective group id %d\n", getuid(), getgid(), geteuid(), getegid());
		printf("\tProcess id %d\n\tParent Process id %d\n", getpid(), getppid());
	}
	else if(pid > 0) {
		// parent process
		sleep(5);
		printf("Parent Process\n");
		printf("\tReal user id %d\n\tReal group id %d\n\tEffective user id %d\n\tEffective group id %d\n", getuid(), getgid(), geteuid(), getegid());
		printf("\tProcess id %d\n\tParent Process id %d\n", getpid(), getppid());
	}
	return 0;
}
