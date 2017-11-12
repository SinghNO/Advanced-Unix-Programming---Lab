#include <stdio.h>
#include <signal.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
	int pid;
	sigset_t newset, oldset, set;
	sigemptyset(&newset);
	if(sigaddset(&newset, SIGALRM) == -1) {
		perror("Sigaddset Error ");
		return 1;
	}
	if(sigprocmask(SIG_BLOCK, &newset, &oldset) == -1) {
		perror("Sigprocmask error ");
		return 1;
	}
	printf("Signal SIGALRM is blocked in parent\n");
	raise(SIGALRM);
	if(sigpending(&set) == -1) {
		perror("Error in sigpending ");
		return 1;
	}
	pid = fork();
	if(pid == 0) {
		if(sigprocmask(SIG_SETMASK, NULL, &oldset) == -1) {
			perror("Sigprocmask error in child ");
			return 1;
		}
		if(sigismember(&oldset, SIGALRM)) {
			printf("SIGALRM is blocked in child\n");
		}
		if(sigpending(&set) == -1) {
			perror("sigpending error in child ");
			return 1;
		}
		if(!sigismember(&set, SIGALRM)) {
			printf("SIGALRM is not pending in child\n");
		}
		else {
			printf("SIGALRM is pening in child\n");
		}
	}
	else if(pid > 0){
		if(!sigismember(&set, SIGALRM)) {
			printf("SIGALRM signal is not pending in the parent\n");
		}
		else {
			printf("SIGALRM is pending in the parent\n");
		}
	}
	else {
		perror("Fork failed ");
		return 1;
	}
	return 0;
}
