#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>
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
int main(int argc, char *argv[]) {
	if(argc < 3) {
		printf("Arguments not valid\n");
		return 1;
	}
	int children_, sleep_, count, pid;
	children_ = charToInt(argv[1]);
	sleep_ = charToInt(argv[2]);
	if(children_ == INT_MIN || sleep_ == INT_MIN) {
		printf("Incorrect arguments\n");
		return 1;
	}
	for(count = 0; count < children_; count++) {
		pid = fork();
		if(!pid) {
			break;
		}
	}
	if(!pid) {
		sleep(sleep_);
		printf("Child %d terminated \n", count + 1);
	}
	else {
		while(wait(NULL) > 0);
		printf("Parent terminated\n");
	}
	return 0;
}
