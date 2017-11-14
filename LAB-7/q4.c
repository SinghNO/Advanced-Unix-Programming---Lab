#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Arguements are insufficient\n");
		return 1;
	}
	setuid(2000);
	execv("/bin/cat", argv);
	return 0;
}
