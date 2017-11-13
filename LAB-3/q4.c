#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


struct stat buffer;

int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Insufficient arguments\n");
		return -1;
	}
	char *filename = argv[1];
	if(stat(filename, &buffer) < 0) {
		printf("Error\n");
		return -1;
	}

	// Printing the device number
	printf("%s : \tdevice number : %ld\n", filename, buffer.st_dev);
	
	// Checking whether character file
	if(S_ISCHR(buffer.st_mode)) {
		printf("File : %s\tType : Character\tMajor No : %d\tMinor No : %d\n", filename, major(buffer.st_rdev), minor(buffer.st_rdev));
	}
	
	// Checking whether a block file
	else if(S_ISBLK(buffer.st_mode)) {
		printf("File : %s\tType : Block\tMajor No : %d\tMinor No : %d\n", filename, major(buffer.st_rdev), minor(buffer.st_rdev));
	}
	return 0;
}
