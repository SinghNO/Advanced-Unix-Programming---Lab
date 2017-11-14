#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
extern char **environ;

void printAll();
void addNew(char *name, char *value);

int main(int argc, char *argv[]) {
	if(argc < 3) {
		// .o, name, value
		printf("Insufficient arguments\n");
		return -1;
	}
	printAll();
	printf("-----NEW ADDITION CORRESPONDING TO COMMAND LINE INPUT----\n");
	addNew(argv[1], argv[2]);
	printAll();
	pause();
	return 0;
}

void printAll() {
	int i = 0;
	for(i = 0; environ[i] != NULL; i++) {
		printf("%s\n", environ[i]);
	}
	return;
}

void addNew(char *name, char *value) {
	// rewrite bit is set
	setenv(name, value, 1);
	return;
}
