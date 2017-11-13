#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<fcntl.h>

int main(){
    int fd, pid;
    fd = open("input.txt", O_RDONLY);
    int n = fcntl(fd,F_SETFD,FD_CLOEXEC);
    if((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    char buf[10];
    sprintf(buf,"%d",fd);
    if(pid == 0)
        execl("child","child",buf, NULL);
    return 0;
}
