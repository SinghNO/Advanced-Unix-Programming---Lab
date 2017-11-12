#include <stdio.h>
#include <unistd.h>
#define MAXLINE 1024


int main() {
	int p1[2], p2[2], p3[2], p4[2], n;
	pid_t pid;
	char line[MAXLINE];

	if(pipe(p1) < 0) {
		printf("Pipe 1 creation failed\n");
		return -1;
	}
	if(pipe(p2) < 0) {	
		printf("Pipe 2 creation failed\n");
		return -1;
	}
	if(pipe(p3) < 0) {
		printf("Pipe 3 creation failed\n");
		return -1;
	}
	if(pipe(p4) < 0) {
		printf("Pipe 4 creation failed\n");
		return -1;
	}
	
	if((pid = fork()) > 0) {
		if(fork() > 0) {
			// Parent p
			close(p1[0]);
			close(p2[1]);
			close(p4[1]);
			write(p1[1], "Mumbai", 6);
			n = read(p2[0], line, sizeof(line));
			printf("Reading at parent from pipe 2 %s\n", line);
			n = read(p4[0], line, sizeof(line));
			printf("Reading at parent from pipe 4 %s\n", line);
		}
		else {
			// Child c2
			close(p4[0]);
			close(p3[1]);
			write(p4[1], "Cochin", 6);
			n = read(p3[0], line, sizeof(line));
			printf("Reading at c2 from pipe 3 %s\n", line);
		}
	}
	else if(pid == 0) {
		// Child c1
		close(p1[1]);
		close(p2[0]);
		close(p3[0]);
		write(p2[1], "Delhi", 5);
		write(p3[1], "Chennai", 7);
		n = read(p1[0], line, sizeof(line));
		printf("Reading at c1 from pipe 1 %s\n", line);
	}
	else if(pid < 0) {
		printf("Fork Failed\n");
		return -1;
	}
	return 0;
}
