/*

Programmer: Parth Kapur
CS 433 Programming Assingment 4: Thread Synchronization

This program will create two threads, one being a consumer thread that will consume date. The second being a producer thread that produces data. 
It will also prevent a race condition like the consumer reading before the producer writing by using a mutex semaphore as well as a counting
semaphore for coordination to determine when a thread can safely write or read to and from a common shared memory region.

*/


#include <semaphore.h>
#include <pthread.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// globally declared variables
int block = 0;
int xtime = 0;
int32_t x;
int * callocPtr;
int c;
int d;
short checksum;
short checksum2;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*
The function errorCheck will check for valid arguments when running the program via command line. 
It will return 1 if failed, otherwise will return 0. Flag is the variable in which the
returning number is stored.
*/
int errorCheck(int argcount, char** argvector, int *block, int *xtime) {
	int flag = 0; // 0 is success, so if it returns 0 no error occurred.
	if (argcount != 3) {
		printf("You have given an invalid number of arguments.\n");
		flag = 1;
	}

	int memsize = atoi(argvector[1]);
	if (memsize % 32 != 0 || memsize <= 0) {
		printf("Memory size has to be positive as well as a multiple of 2.\n");
		flag = 1;
	}
	*xtime = atoi(argvector[2]);
	if (xtime <= 0) {
		printf("Producer should write at least 1 time(s)\nConsumer should read at least 1 time(s)\n");
		flag = 1;
	}
	//This is the total number of block of the shared memory region.
	if (flag == 0) {
		*block = memsize / 32;
	}
	return flag;
}




void* producer(void* arg) {
	int pcounter;
	int i;
	for (pcounter = 0; pcounter < xtime;pcounter++) {

		pthread_mutex_lock(&mutex);
		for (i = 0; i < 8; i++) {
			callocPtr[i] = rand();
		}
		// We are now deleting the last two bytes for Checksum
		callocPtr[7] = callocPtr[7] & 0xFFFF0000;
		//do the checksum for each integer
		c = 0;
		for (i = 0; i < 8; i++) {
			c += callocPtr[i];
		}
		c = ~c;
		c++;
		//using only the last 2 bytes of checksum
		c = c & 0x0000FFFF;
		checksum = (short)c;
		short* l2b = (short*)callocPtr;
		l2b[14] = checksum;
		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(NULL);
}

void * consumer(void * arg) {
	d = 0;
	int i;
	int ccounter;
	for (ccounter = 0; ccounter < xtime; ccounter++) {
		pthread_mutex_lock(&mutex);
		for (i = 0; i < 8; i++) {
			d += callocPtr[i];
		}
		d = ~d;
		d++;
		//using only the last 2 bytes of checksum
		d = d & 0x0000FFFF;
		//printf("The last two bytes of checksum are %d\n",c);
		checksum2 = (short)d;
		short* l2b_2 = (short*)callocPtr;
		l2b_2[14] = checksum2;


		if (checksum != checksum2)
			printf("Error occurred in block %d\n", ccounter);
	}
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}



int main(int argc, char** argv) {
	if (errorCheck(argc, argv, &block, &xtime) != 0)
		exit(0);

	printf("There are %d block(s)!\n", block);
	printf("Read & Write %d time(s)\n", xtime);

	srand(time(NULL));

	// This will create a shared memory region
	// This also allocates an array of "block" elements, each element is size 32
	callocPtr = calloc(block, 32);

	// Now we will create the producer thread.
	pthread_t pid;
	pthread_create(&pid, NULL, producer, NULL);
	pthread_join(pid, NULL);

	pthread_t cid;
	pthread_create(&cid, NULL, producer, NULL);
	pthread_join(cid, NULL);

	return 0;
}