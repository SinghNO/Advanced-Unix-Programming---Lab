#include<stdio.h>
#include<unistd.h>
#include <sys/acct.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

pid_t wait2(int *wtime, int *rtime, int *iotime);
unsigned int long compt2ulong(comp_t comptime);
struct acct acdata;

int main(int argc, char *argv[]) {
	int i, wtime, rtime, iotime;
	pid_t pid;
	if((pid = fork()) < 0) {
		printf("...Fork failed...\n");
		return -1;
	}
	else if(pid > 0){	// parent process
		printf("Pid of caught process is %d\n", pid);
		pid = wait2(&wtime, &rtime, &iotime);
	}
	else {			// some computation in child process
		i = 10;	
		while(i > -1000) {
			i = i - 1;
		}
		exit(1);
	}
	return 0;
}

pid_t wait2(int *wtime, int *rtime, int *iotime) {
	int fd, val, ret, pid;
	char *pathname = "/home/nikunj/Desktop/AUP-ASSGN/LAB-8/info.txt";
	fd = open(pathname, O_RDWR | O_APPEND, 0777);
	acct(pathname);		// process accounting turned on
	pid = wait(NULL);	// waiting for child process
	acct(NULL);		// process accounting turned off
	ret = read(fd, &acdata, sizeof(acdata));
	if(ret == -1) {
		printf("...Read failed ...\n");
		return -1;
	}
	val = compt2ulong(acdata.ac_utime) + compt2ulong(acdata.ac_stime);
	*rtime = val;
	val = compt2ulong(acdata.ac_etime - acdata.ac_utime - acdata.ac_stime);
	*wtime = val;
	// calculation of iotime
	
	printf("rtime in clockticks is %d\nwtime in clockticks is %d\n", pid, rtime, wtime);
	return pid;
}

unsigned int long compt2ulong(comp_t comptime) {
	int val, exp;
	val = comptime & 0x1fff;
	exp = (comptime >> 13) & 7;
	while (exp-- > 0) { 
		val *= 8;
	}
	return val;
}
