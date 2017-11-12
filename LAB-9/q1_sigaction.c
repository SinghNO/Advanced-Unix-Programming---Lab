#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// for dump of SIGSEGV signal check the timestamp of /proc/sys/kernel/core_pattern file

void sig_term(int signo);

int main(int argc, char *argv[]) {
	struct sigaction a1, a2, a3;
	a1.sa_handler = &sig_term;
	a2.sa_handler = SIG_IGN;
	a3.sa_handler = SIG_DFL;
	// block every signal during the handler
	sigfillset(&a1.sa_mask);
	sigfillset(&a2.sa_mask);
	sigfillset(&a3.sa_mask);
	if(sigaction(SIGTERM, &a1, NULL) < 0) {
		printf("Cannot handle SIGTERM\n");
	}
	if(sigaction(SIGINT, &a2, NULL) < 0) {
		printf("Cannot handle SIGINT");
	}
	if(sigaction(SIGSEGV, &a3, NULL) < 0) {
		printf("Cannot handle SIGSEGV\n");
	}
	for( ; ; ) {
		pause();
	}
	return 0;
}

void sig_term(int signal) {
	if(signal == SIGTERM) {
		printf("SIGTERM caught\n");
	}
	return;
}

