#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int createDir(char *path);

int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Unsuccessful Operation\n");
		return -1;	
	}
	int flag = -1;
	char *path = argv[1];
	flag = createDir(path);
	// Error handling
	if(flag == -1) {
		return -1;
	}
	return 0;
}

int createDir(char *path) {
	int flag = -1;
	// Creating file using mkdir system call and assigning permissions
	flag = mkdir(path, S_ISGID | S_ISVTX | S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
	chmod(path, 01777);
	return flag;
}
