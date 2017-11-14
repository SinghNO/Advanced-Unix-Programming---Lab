#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>

void sig_handler(int signo) {
	if((signal(SIGINT, sig_handler)) == SIG_ERR) {
		perror("Unable to set signal handler again");
	}
	printf("Caught SIGNINT\n");
	return;
}

int main() {
	pid_t child;
	
	if((child = fork()) < 0) {
		perror("Unable to create child process");
		return -1;
	}

	if(child == 0) {
		sleep(2);
		if((kill(getppid(), SIGINT)) == -1) {
			if(errno == ESRCH) {
				perror("Parent already died :-(");
				return -1;
			}
			else {
				perror("Unable to send signal to parent");
				return -1;
			}
		}
		return 0;
	}
	else {
		if((signal(SIGINT, sig_handler)) == SIG_ERR) {
			perror("Unable to set signal handler");
		}
		sleep(5);
		return 0;
	}
	
	return 0;
}	
