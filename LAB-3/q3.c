#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

mode_t current_umask;

mode_t read_umask(void);

int main(int argc, char *argv[]) {
	mode_t val = read_umask();
	printf("umask is %d\n", val);
}

mode_t read_umask() {
	// Sets the process umask and returns a copy of old umask
	current_umask = umask(0);
	// again setting it with current_umask wihout leaving it unchanged
	umask(current_umask);
	// returning the current umask
	return current_umask;
}
