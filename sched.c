#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <pthread.h>
#include <time.h>

#define SIZE 2000000

typedef struct _thread_data_t {
    int localTid;
    const int *data;
    int numVals;
    pthread_mutex_t *lock;
    long long int *totalSum;
} thread_data_t;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Not enough parameters.\n");
        return -1;
    }

    int data[SIZE];
    long long int totalSum = 0;

    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);

    int numberOfThreads = atoi(argv[1]);
    thread_data_t threadData[numberOfThreads];

    for(int i = 0; i < numberOfThreads; i++)
    {
        threadData[i].localTid = i;
        threadData[i].data = data;
        threadData[i].numVals = SIZE;
        threadData[i].lock = lock;
        threadData[i].totalSum = totalSum;
    }

    pthread_t threads[numberOfThreads];
    

}
