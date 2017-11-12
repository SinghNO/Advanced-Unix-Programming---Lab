#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main() {
	int out;
	pid_t odd, even;

	if((out = open("out", O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP)) == -1) {
		perror("Can't create output file");
		return -1;
	}
	if((mkfifo("otot", S_IRUSR | S_IWUSR | S_IRGRP)) == -1) {
		perror("Can't create FIFO file");
		return -1;
	}
	if((mkfifo("ttoo", S_IRUSR | S_IWUSR | S_IRGRP)) == -1) {
		perror("Can't create FIFO file");
		return -1;
	}

	if((odd = fork()) < 0) {
		perror("Unable to create child process");
		return -1;
	}
	if(odd != 0) {
		if((even = fork()) < 0) {
			perror("Unable to create child process");
			return -1;
		}
	}
		
	if(odd == 0) {
		int otot, ttoo, counter;
		char next;
		pid_t podd;
		
		podd = getpid();
		next = 'a';
		
		if((otot = open("otot", O_WRONLY)) == -1) {
			perror("Unable to open FIFO for writing");
			return -1;
		}
		if((ttoo = open("ttoo", O_RDONLY)) == -1) {
			perror("Unable to open FIFO for reading");
			return -1;
		}
		
		counter = 1;
			
		do {
			dprintf(out, "%d\t\t%d\n", counter, podd);
			write(otot, &next, sizeof(char));
			counter += 2;
		}while(read(ttoo, &next, sizeof(char)) && counter != 101);
		close(otot);
		close(ttoo);
		
		return 0;
	}
	
	if(even == 0) {
		int otot, ttoo, counter;
		char next;
		pid_t peven;
		
		peven = getpid();
		next = 'a';
		
		if((otot = open("otot", O_RDONLY)) == -1) {
			perror("Unable to open FIFO for writing");
			return -1;
		}
		if((ttoo = open("ttoo", O_WRONLY)) == -1) {
			perror("Unable to open FIFO for reading");
			return -1;
		}
		
		counter = 2;
			
		while(read(otot, &next, sizeof(char)) && counter != 102) {
			dprintf(out, "%d\t\t%d\n", counter, peven);
			write(ttoo, &next, sizeof(char));
			counter += 2;
		} 			
		close(otot);
		close(ttoo);

		return 0;
	}
	
	if(odd && even) {
		waitpid(odd, NULL, 0);
		waitpid(even, NULL, 0);
		close(out);
		return 0;
	}

	return 0;
}
