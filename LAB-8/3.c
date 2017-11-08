#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>
int main(int argc, char *argv[]) {
	int pid = fork();
	if(pid == 0) {
		printf("Child without exec....\n");
		printf("Before pgid of child = %u\n", getpgid(0));
		sleep(5);
		printf("After pgid of child = %u\n", getpgid(0));
	}
	else {
		sleep(2);
		setpgid(pid, getppid());
		wait(NULL);
		pid = fork();
		if(pid == 0) {
			printf("Child with exec of ls......\n");
			execv("/bin/ls", argv);
		}
		else {
			sleep(5);
			int result = setpgid(pid, getppid());
			if(result == -1) {
				perror("Could not change GPID: ");
			}
		}
	}
	return 0;
}
