#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#define SIZE sizeof(int)
void reader(key_t key, int shmid) {
	int *data, semid;
	data = (int *) shmat(shmid, 0, 0);
	semid = semget(key, 1, IPC_EXCL | 0666);
	printf("trying to lock....\n");
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = -1;  /* set to allocate resource */
	sb.sem_flg = SEM_UNDO;
	while(1) {
		sleep(1);
		if (semop(semid, &sb, 1) == -1) {
			 perror("semop");
		}
		else
			printf("Locked.\n");
	}
}
void producer(key_t key, int shmid) {
	int *data;
	data = (int *) shmat(shmid, 0, SHM_RDONLY);
	int semid = semget(key, 1, IPC_EXCL | 0666);
	if(semid == -1) {
		perror("Semget failed");
		exit(1);
	}
	printf("Trying to unlock...\n");
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = 1; /* free resource */
	while(1) {
		sleep(1);
		if (semop(semid, &sb, 1) == -1) {
			perror("semop");
			exit(1);
		}
		else
			printf("Unlocked\n");
	}
}
int main(int argc, char *argv[]) {
	int shmid, pid, semid;
	struct sembuf sb;
	key_t key = 5683;
	shmid = shmget(key, SIZE, IPC_CREAT | 0600);
	if(shmid == -1) {
		perror("Error while creating Shared memeory: ");
		return 1;
	}
	semid = semget(key, 1, IPC_EXCL | 0666);
	if(semid >= 0) {
		union semum {
			int val;
			struct semid_ds *buf;
			ushort *array;
		}arg;
		arg.val = 0;
		semctl(semid, 0, SETVAL, arg);
		sb.sem_num = 0;
        	sb.sem_op = 1;
        	sb.sem_flg = 0;
        	semop(semid, &sb, 1);
		pid = fork();
		if(pid == 0) {
			printf("Inside child\n");
			reader(key, shmid);
		}
		else if(pid != -1) {
			printf("Inside parent\n");
		//	producer(key, shmid);
		}
		else {
			perror("Fork failed: ");
		}
	}
	else {
		perror("Faled to get semaphore ");
	}
//	semctl(semid, 0, IPC_RMID);
	shmctl(shmid, 0, IPC_RMID);
	return 0;
}
