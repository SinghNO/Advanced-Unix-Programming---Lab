#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <unistd.h>
int charToInt(char *str) {
        int result = 0, count = 0;
        while(str[count]) {
                if(isdigit(str[count])) {
                        result = result * 10 + (str[count] - '0');
                }
                else {
                        return INT_MIN;
                }
                count++;
        }
        return result;
}
void *function(void *arg) {
	int *a;
	a = (int*) arg;
	printf("Thread number = %d\n", *a);
	pthread_exit(0);
}
int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Not enough arguments\n");
		return 1;
	}
	int num, count, i;
	pthread_t *th;
	num = charToInt(argv[1]);
	th = (pthread_t *) malloc(sizeof(pthread_t) * num);
	for(count = 0; count < num; count++) {
		i = count;
		pthread_create(&th[count], NULL, function, &i);
		pthread_join(th[count], NULL);
	}
	return 0;
}
