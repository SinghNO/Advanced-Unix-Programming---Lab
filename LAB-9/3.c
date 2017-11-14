#include <setjmp.h>
#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>
jmp_buf env;
int charToInt(char *str) {
        int result = 0, count = 0;
        while(str[count]) {
                if(isdigit(str[count])) {
                        result = result * 10 + (str[count] - '0');
                }
                else {
                        return INT_MIN;
                }
                count++;
        }
        return result;
}
static void sig_alrm(int signo) {
	longjmp(env, 1);
}
unsigned int sleep_aup(unsigned int seconds) {
	unsigned int diff;
	void (*handler)(int);
	handler = signal(SIGALRM, sig_alrm);
	if(handler == SIG_ERR) {
		return seconds;
	}
	if(setjmp(env) == 0) {
		diff = alarm(3600);
		if(diff != 0) {
			alarm(diff);
		}
		else {
			alarm(seconds);
		}
		pause();
	}
	signal(SIGALRM, handler);
	return alarm(0);
}
void pass(int signo) {
	printf("You are in the user defined signal handler.\n");
	return;
}
int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Not enough arguments\n");
		return 1;
	}
	unsigned int a, unslept;
	a = charToInt(argv[1]);
	if(a == INT_MIN) {
		printf("Enter a valid possitive number\n");
		return 1;
	}
	alarm(1);
	signal(SIGALRM, pass);
	unslept = sleep_aup(a);
	printf("Number of unslept seconds = %d\n", unslept);
	if(signal(SIGALRM, SIG_DFL) == pass) {
		printf("Sucessfull\n");
	}
	return 0;
}
