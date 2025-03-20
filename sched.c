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

#define ANSI_COLOR_GRAY "\x1b[30m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define TERM_CLEAR() printf("\033[H\033[J")
#define TERM_GOTOXY(x,y) printf("\033[%d;%dH", (y), (x))

#define SIZE 2000000

typedef struct _thread_data_t {
    int localTid;
    const int *data;
    int numVals;
    pthread_mutex_t *lock;
    long long int *totalSum;
} thread_data_t;

void* arraySum(void* a);
void print_progress(pid_t localTid, size_t value);

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
        threadData[i].lock = &lock;
        threadData[i].totalSum = totalSum;
    }

    pthread_t threads[numberOfThreads];
    
    for(int i = 0; i < numberOfThreads; i++)
    {
        pthread_create(&threads[i], NULL, arraySum, &threadData[i]);
    }
    for(int i = 0; i < numberOfThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }

}

void print_progress(pid_t localTid, size_t value) {
    pid_t tid = syscall(__NR_gettid);
    TERM_GOTOXY(0,localTid+1);
    char prefix[256];
    size_t bound = 100;
    sprintf(prefix, "%d: %ld (ns) \t[", tid, value);
    const char suffix[] = "]";
    const size_t prefix_length = strlen(prefix);
    const size_t suffix_length = sizeof(suffix) - 1;
    char *buffer = calloc(bound + prefix_length + suffix_length + 1, 1);
    size_t i = 0;
    strcpy(buffer, prefix);
    for (; i < bound; ++i)
    {
        buffer[prefix_length + i] = i < value/10000 ? '#' : ' ';
    }
    strcpy(&buffer[prefix_length + i], suffix);
    if (!(localTid % 7))
        printf(ANSI_COLOR_WHITE "\b%c[2K\r%s\n" ANSI_COLOR_RESET, 27, buffer);
    else if (!(localTid % 6))
        printf(ANSI_COLOR_BLUE "\b%c[2K\r%s\n" ANSI_COLOR_RESET, 27, buffer);
    else if (!(localTid % 5))
        printf(ANSI_COLOR_RED "\b%c[2K\r%s\n" ANSI_COLOR_RESET, 27, buffer);
    else if (!(localTid % 4))
        printf(ANSI_COLOR_GREEN "\b%c[2K\r%s\n" ANSI_COLOR_RESET, 27, buffer);
    else if (!(localTid % 3))
        printf(ANSI_COLOR_CYAN "\b%c[2K\r%s\n" ANSI_COLOR_RESET, 27, buffer);
    else if (!(localTid % 2))
        printf(ANSI_COLOR_YELLOW "\b%c[2K\r%s\n" ANSI_COLOR_RESET, 27, buffer);
    else if (!(localTid % 1))
        printf(ANSI_COLOR_MAGENTA "\b%c[2K\r%s\n" ANSI_COLOR_RESET, 27,buffer);
    else
        printf("\b%c[2K\r%s\n", 27, buffer);
    fflush(stdout);
    free(buffer);
}
