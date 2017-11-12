#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int dirhandler;

void check_symlink(char *path, int count) {
	char name[250];
	struct stat buf;
	ssize_t namesize;
	if(count == 50) {
		printf("\nError: Possible circular dependency. Not resolving any further\n");
		exit(-1);
	}
	if((lstat(path, &buf)) == -1) {
		perror("Error while reading file info");
		exit(-1);
	}

	if((buf.st_mode & S_IFMT) == S_IFLNK) {
		if((namesize = readlink(path, name, sizeof(char) * 250)) == -1) {
			perror("Error while reading symbolic link");
			exit(-1);
		}
		name[namesize] = '\0';
		printf("-> %s ", name);
		check_symlink(name, ++count);
	}
	return;
}

int lnk_filter(const struct dirent *dir) {
	struct stat buf;
	if((fstatat(dirhandler, dir->d_name, &buf,
		 AT_SYMLINK_NOFOLLOW)) == -1) {
		perror("Error reading file info");
		return -1;
	}
	if(S_ISLNK(buf.st_mode))
		return 1;
	return 0;
}

int main(int argc, char *argv[]) {
	char *dirname;
	struct dirent **list;	
	int count;
	DIR *dirp;
	
	if(argc > 2) {
		printf("Usage:\nrealpath <dirname, optional>\n");
		return 1;
	}

	setlocale(LC_ALL, "");	
	dirname = ".";
	if(argv[1]) {
		dirname = argv[1];
	}
	
	if(!(dirp = opendir(dirname))) {
		perror("Error opening directory");
		return -1;
	}
	if((dirhandler = dirfd(dirp)) == -1) {
		perror("Error opening directory");
		return -1;
	}	
	
	count = scandir(dirname, &list, lnk_filter, alphasort);
	
	if(count == -1) {
		perror("Error scanning directory");
		return -1;
	}
	while(count--) {
		char resolved_name[250];
		int bufsize;
		if((bufsize = readlinkat(dirhandler, (*list)->d_name,
			 resolved_name, 250)) == -1) {
			perror("Error reading symbolic link");
			return -1;
		}
		resolved_name[bufsize] = '\0';
		printf("%s ", (*list)->d_name);
		printf("-> %s", resolved_name);
		check_symlink(resolved_name, 0);
		printf("\n");
		list++;
	}

	return 0;
}		
