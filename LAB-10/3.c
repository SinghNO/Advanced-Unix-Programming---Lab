#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#define SIZE 4
#define ITERATION 10
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};
int toggle_sem(int semid, int sem_no, int flag, int sem_op) {
	struct sembuf sb;
	sb.sem_num = sem_no;
	sb.sem_op = sem_op;
	sb.sem_flg = flag;
	return semop(semid, &sb, 1);
}
void reader(key_t key, int shmid) {
	int *data, semid, count;
	data = (int *) shmat(shmid, 0, 0);
	semid = semget(key, 3, IPC_EXCL | 0666);
	if(semid == -1) {
		perror("semget failed ");
		exit(1);
	}
	for(count = 0; count < ITERATION; count++) {
		toggle_sem(semid, 2, 0, -1); //wait till the buffer has at least one element
		toggle_sem(semid, 0, 0, -1); //lock on shared memory
		printf("Data consumed = %d\n", data[count % SIZE]);
		toggle_sem(semid, 0, 0, 1);
		toggle_sem(semid, 1, 0, 1);
	}
}
void producer(key_t key, int shmid) {
	int *data, count;
	data = (int *) shmat(shmid, 0, 0);
	int semid = semget(key, 3, IPC_EXCL | 0666);
	if(semid == -1) {
		perror("Semget failed");
		exit(1);
	}
	for(count = 0; count < ITERATION; count++) {
		toggle_sem(semid, 1, 0, -1);		//wait until the buffer has at least one empty space
		toggle_sem(semid, 0, 0, -1);		//Lock on shared memory
		data[count % SIZE] = count;
		printf("Data produced = %d\n", data[count % SIZE]);
		toggle_sem(semid, 0, 0, 1);
		toggle_sem(semid, 2, 0, 1);
	}
}
int main(int argc, char *argv[]) {
	int shmid, pid, semid;
	key_t key = 5683;
	shmid = shmget(key, SIZE * sizeof(int), IPC_CREAT | 0600);
	if(shmid == -1) {
		perror("Error while creating Shared memeory: ");
		return 1;
	}
	semid = semget(key, 3, IPC_EXCL | IPC_CREAT | 0666);
	if(semid >= 0) {
		union semun arg;
		arg.val = 1;
		semctl(semid, 0, SETVAL, arg);	//Semaphore for buffer
		arg.val = SIZE;
		semctl(semid, 1, SETVAL, arg); //number of empty elements in buffer
		arg.val = 0;
		semctl(semid, 2, SETVAL, arg); //number of filled elements in buffer
		pid = fork();
		if(pid == 0) {
			reader(key, shmid);
		}
		else if(pid != -1) {
			producer(key, shmid);
		}
		else {
			perror("Fork failed: ");
		}
	}
	else {
		perror("Faled to get semaphore ");
	}
	semctl(semid, 0, IPC_RMID);
	shmctl(shmid, 0, IPC_RMID);
	return 0;
}
