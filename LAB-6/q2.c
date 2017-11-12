#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <stdlib.h>

void *threadcpu(void *arg);
int main() {
	int i, *args;
	long cpu;
	cpu_set_t *cpuset;
	pthread_t *pthreadset;
	pthread_attr_t *pthreadattrset;

	cpu = sysconf(_SC_NPROCESSORS_ONLN);
	printf("The system has %ld processing cores\n", cpu);
	
	cpuset = (cpu_set_t*)malloc(sizeof(cpu_set_t) * (int)cpu);
	pthreadset = (pthread_t*)malloc(sizeof(pthread_t) * (int)cpu);
	pthreadattrset = (pthread_attr_t*)malloc(sizeof(pthread_attr_t) * (int)cpu);
	args = (int *)malloc(sizeof(int) * (int)cpu);

	for(i = 0; i < (int)cpu; i++) {
		CPU_ZERO(&cpuset[i]);
	}	
	for(i = 0; i < (int)cpu; i++) {
		CPU_SET(i, &cpuset[i]);
	}
	 
	for(i = 0; i < (int)cpu; i++) {
		pthread_attr_init(&pthreadattrset[i]);
	}
	for(i = 0; i < (int)cpu; i++) {
		pthread_attr_setaffinity_np(&pthreadattrset[i], sizeof(cpu_set_t), &cpuset[i]);
	}
	for(i = 0; i < (int)cpu; i++) {
		args[i] = i;
		pthread_create(&pthreadset[i], &pthreadattrset[i], &threadcpu, (void*)&args[i]);
	}
	
	for(i = 0; i < (int)cpu; i++) {
		pthread_join(pthreadset[i], NULL);
	}
	return 0;
}

void *threadcpu(void *arg) {
	int argu;
	argu = *(int *)arg;
	printf("Thread#%d is running on CPU#%d\n", argu, sched_getcpu());
	return NULL;
}
