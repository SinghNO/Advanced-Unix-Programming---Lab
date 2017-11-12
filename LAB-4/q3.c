#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	int getpidi, forki, count;
	struct timeval tv1, tv2, diff;
	double avgm, diffm;
	avgm = 0;
	getpidi = forki = 10;

	while(getpidi--) {
		count = 100;
		gettimeofday(&tv1, NULL);
		while(count--)
			getpid();
		gettimeofday(&tv2, NULL);
		timersub(&tv2, &tv1, &diff);
		diffm = (double)diff.tv_sec * 1000000 + diff.tv_usec;
		diffm /= 100;
		printf("%f\n", diffm);
		avgm += diffm;
	}
	avgm /= 10;
	printf("Avg. of getpid(): %f\n", avgm);
	
	avgm =  0;
	while(forki--) {
		count = 100;
		gettimeofday(&tv1, NULL);
		while(count--)
			if(!fork())
				_exit(0);
		gettimeofday(&tv2, NULL);
		timersub(&tv2, &tv1, &diff);
		diffm = (double)diff.tv_sec * 1000000 + diff.tv_usec;
		diffm /= 100;
		printf("%f\n", diffm);
		avgm += diffm;
	}
	avgm /= 10;
	printf("Avg. of fork(): %f\n", avgm);

	return 0;
}
