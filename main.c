/*!
**************************************************************
* @file    main.c
* @brief   MAIN SOURCE file
* @author  GRUPPE *
* @version V1.0
* @date    2022-01-27
* @copyright Copyright (c) FH CAMPUS ESE22. All rights reserved.
**************************************************************
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>

#include "main.h"
#include "gui.h"
#include "middleware.h"

int main(int argc, char **argv)
{
    clrscr();
    // pthread_t threads[NUM_THREADS];
    // int threadCreate;
    // threadCreate = pthread_create(&threads[NUM_UI_TREAD], NULL, UI_INTERFACE, (void *)NUM_UI_TREAD);

    // if (threadCreate != 0)
    // {
    //     printf("[X] Error:unable to create thread, %dr\\n", threadCreate);
    //     exit(-1);
    // }
    // else
    // {
    //     printf("---------------------------------------------\n");
    //     printf("----- MULTI THREADING  ----------------------\n");
    //     printf("---------------------------------------------\n");
    //     printf("[X] Created Thread ID, %d\r\n", threadCreate);
    // }

    char testdata[BYTES_PAYLOAD];
    
    for (uint8_t i = 0; i < BYTES_PAYLOAD; i++)
    {
        testdata[i] = 0x20;
    }
    
    
    uint16_t testchecksum = 0;
    uint8_t error = 0;
    error = calcChecksum(testdata, &testchecksum);
    printf("Testchecksum: %d\n", testchecksum);
    while (1)
    {
        break; // Just to test
    }

    //pthread_exit(NULL);
}



/*!
**************************************************************
* @brief
*
* @note
*
* @warning
*
* @param[in]
* @param[out]
*
* @return
*
* @retval = 0 -> Success
* @retval > 0 -> Warning
* @retval < 0 -> Fail
*
**************************************************************
*/
void *UI_INTERFACE(void *threadID)
{
    long thread_ID;
    thread_ID = (long)threadID;

    printf("[X] UI INTERFACE ID, %ld started\r\n", thread_ID);

    // test
    UI_MAIN();

    pthread_exit(NULL);
}
