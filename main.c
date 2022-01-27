#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "main.h"
#include "gui.h"

int main(int argc, char **argv)
{
    pthread_t threads[NUM_THREADS];
    int threadCreate;
    threadCreate = pthread_create(&threads[NUM_UI_TREAD], NULL, UI_INTERFACE, (void *)NUM_UI_TREAD);

    if (threadCreate != 0)
    {
        printf("[X] Error:unable to create thread, %dr\\n", threadCreate);
        exit(-1);
    }
    else
    {
        printf("[X] Created Thread ID, %d\r\n", threadCreate);
    }
    

    while (1)
    {

        break; // Just to test
    }

    pthread_exit(NULL);
}


void *UI_INTERFACE(void *threadID)
{
    long thread_ID;
    thread_ID = (long)threadID;
    printf("[X] UI_INTERFACE ID, %ld started\r\n", thread_ID);
    while (1)
    {
        UI_start();

        break; // Just to test
    }

    pthread_exit(NULL);
}
