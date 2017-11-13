#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#define MAXLENGTH 32

void *search(void *arg);
int match(char *word, int fd);

// Structure helpful in searching
typedef struct {
	char *word;
	char *filename;
} pthreadstruct;

// global variable for thread exit
int flag = 0;

int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Insufficient arguments\n");
		return -1;
	}
	char *word = argv[1]; 			// pattern
	int num_files = argc - 2;		// number of files
	pthreadstruct pths[num_files];		// struct for files
	pthread_t threads[num_files];		// pthread array
	int i = 0;
	for(i = 0; i < num_files; i++) {
		pths[i].word = word;
		pths[i].filename = argv[i + 2];
		
		if(pthread_create(&threads[i], NULL, search, &pths[i])) {
			printf("Thread creation failed\n");
			return -1;
		}
	}
	for(i = 0; i < num_files; i++) {
		void *ret;
		if(pthread_join(threads[i], &ret)) {
			printf("Thread join failed\n");
		}
	}
	return 0;
}

void *search(void *arg) {
	pthreadstruct *args = (pthreadstruct *)arg;
	int fd;
	fd = open(args->filename, O_RDONLY);
	if(fd == -1) {
		printf("File Not Found\n");
		pthread_exit(NULL);
	}
	if(match(args->word, fd)) {
		printf("%s\n", args->filename);
		flag = 1;
		pthread_exit((char*)(args->filename));
	}
	else {
		pthread_exit(NULL);
	}
}

// utility for search function

int match(char *word, int fd) {
	char line[MAXLENGTH];
	char c;
	int i, n;
	while((n = read(fd, line, MAXLENGTH)) > 0) {
		if(strstr(line, word) != NULL) {
			printf("The pattern is %s\n", line);
			return 1;
		}
	}
	return 0;
}
