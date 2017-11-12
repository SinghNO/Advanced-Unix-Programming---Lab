#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// Implementation using signal
static void sig_term(int signo);

int main(int argc, char *argv[]) {
	if(signal(SIGTERM, sig_term) == SIG_ERR) {
		printf("Couldn't catch SIGTERM\n");
	}
	if(signal(SIGINT, SIG_IGN) == SIG_ERR) {
		printf("Couldn't ignore SIGINT\n");
	}
	if(signal(SIGSEGV, SIG_DFL) == SIG_ERR) {
		printf("Couldn't perform default action for SIGSEGV\n");
	}	
	for( ; ; ) {
		// Suspend the process until a signal comes
		pause();
	}
	return 0;
}

static void sig_term(int signo) {
	// reestablishing signal handler
	signal(SIGTERM, sig_term);
	if(signo == SIGTERM) {
		printf("SIGTERM caught\n");
	}
	return;
} 
