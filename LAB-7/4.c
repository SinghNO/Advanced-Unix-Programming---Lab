#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Arguements are insufficient\n");
		return 1;
	}
	uid_t ruid, euid;
	ruid = getuid();
	euid = geteuid();
	printf("reuid = %u euid = %u \n", ruid, euid);
	setuid(ruid);
	execv("/bin/cat", argv);
	return 0;
}
