#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int fd;
	char *str1 = "Suavecito!!";
	char *str2 = "Despacito!!";
	if(argc == 1) {
		printf("Usage:\n./hole <filename>\n");
		return 1;
	}
	
	fd = open(argv[1], O_WRONLY | O_CREAT,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if(fd == -1) {
		printf("Error opening file\n");
		return -1;
	}
	lseek(fd, 10, SEEK_SET);
	write(fd, (void *)str1, 10);
	lseek(fd, 30, SEEK_SET);
	write(fd, (void *)str2, 10);
	close(fd);			
	return 0;
}
	
