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

#include "main.h"


int do_mutex = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

extern int inputID_FLAG;
extern int inputID_GLOBAL;

extern char frameToSend[BYTES_FRAME_TOTAL]; // Connection between MW and UI
extern uint8_t groupsize;                   // Amount of group members (needs to be set by UI)
extern uint8_t myID;                        // ID of Peer (needs to be set by UI)
extern uint32_t message_cnt;                // message counter represents the latest message id

int main(int argc, char **argv)
{
    clrscr();
    pthread_t th1;
    int threadCr1;
    if(pthread_mutex_init(&mymutex,NULL)!=0){
        printf("ERROR: Mutex was not created!\n");
    }
    if(pthread_mutex_init(&mutexInput,NULL)!=0){
        printf("ERROR: Mutex was not created!\n");
    }
    if(pthread_mutex_init(&mutexBitError,NULL)!=0){
        printf("ERROR: Mutex was not created!\n");
    }

    threadCr1 = pthread_create(&th1, NULL, UI_INTERFACE, (void *)NUM_UI_TREAD);
    if (threadCr1 != 0)
    {
        printf("[X] Error:unable to create thread, %dr\\n", threadCr1);
        exit(-1);
    }
    else
    {

        // printf("---------------------------------------------\n");
        // printf("----- MULTI THREADING  ----------------------\n");
        // printf("---------------------------------------------\n");
        // printf("[X] Created UI Thread ID, %d\r\n", NUM_UI_TREAD);
        // printf("[X] Created MW Thread ID, %d\r\n", NUM_MW_TREAD);
    }


    middleware();

    pthread_exit(NULL);
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

    // printf("[X] UI INTERFACE ID, %ld started\r\n", thread_ID);
 

        pthread_mutex_lock(&mutex);
        UI_MAIN();
        pthread_mutex_unlock(&mutex);
    

    pthread_exit(NULL);
}