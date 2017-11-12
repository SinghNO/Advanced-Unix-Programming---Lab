#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	int infile, outfile;
	char str[10];
	
	infile = 0;
	outfile = 1;
	if(argc > 3) {
		printf("Usage:\npipe <input, optional> <output, optional>\n");
		return 1;
	}
	
	if(argc > 1) {
		if((infile = open(argv[1], O_RDONLY)) == -1) {
			perror("Error opening input file");
			return -1;
		}
		dup2(infile, 0);
	}
	
	if(argc > 2) {
		if((outfile = open(argv[2], O_WRONLY | O_CREAT 
			| O_TRUNC, 00664)) == -1) {
		perror("Error opening output file");
		return -1;
		}
		dup2(outfile, 1);
	}

	scanf("%s", str);
	printf("%s This should be in a file\n", str);
	
	return 0;
}
