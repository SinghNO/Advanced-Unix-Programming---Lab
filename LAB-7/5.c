#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Arguements are insufficient\n");
		return 1;
	}
	uid_t ruid;
	ruid = getuid();
	euid = geteuid();
	printf("reuid = %ld euid = %ld \n", ruid, euid);
//	execv("/bin/cat", argv);
	return 0;
}
