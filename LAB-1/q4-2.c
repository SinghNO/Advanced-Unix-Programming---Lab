#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int fdsrc, fddest;
	char c;

	if(argc == 1 && argc != 3) {
		printf("Usage:\n./rmhole <src filename> <dest filename>\n");
		return 1;
	}
	
	fdsrc = open(argv[1], O_RDONLY);
	fddest = open(argv[2], O_WRONLY | O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if(fdsrc == -1 || fddest == -1) {
		printf("Error opening file\n");
		return -1;
	}

	while(read(fdsrc, &c, 1)) {
		if(c)
			write(fddest, &c, 1);
	}
	
	close(fdsrc);
	close(fddest);
	return 0;
}
