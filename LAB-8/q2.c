#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	pid_t child;
	int status;	
	if((child = fork()) < 0) {
		perror("Error while fork");
		return -1;
	}
	
	if(child == 0) {
		pid_t processid;
		char buf[L_ctermid];
		processid = getpid();
		printf("Process ID: %u\nProcess Group ID: %u\nSession ID: %u\nControlling Terminal: %s\n", getpid(), getpgid(processid), getsid(processid), ctermid(buf));
		if((setsid()) == -1) {
			perror("Error while creating session");
			return -1;
		}
		printf("Process ID: %u\nProcess Group ID: %u\nSesssion ID: %u\nControlling Terminal: %s\n", getpid(), getpgid(processid), getsid(processid), ctermid(buf));
		_exit(0);	 	
	}
	else {
		wait(&status);
		if(WIFEXITED(status)) {
			return 0;
		}
		else {
			printf("Some error occured with child process");
			return -1;
		}
	}
	
	return 0;
}
