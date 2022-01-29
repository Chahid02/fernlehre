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

static int do_mutex;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char* logfilePathTesting = "../log.txt"; //das benutzen für Middlewaretests, später den Path aus der GUI benutzen

void testChecksum()
{
    char testdata[BYTES_PAYLOAD];
    for (uint8_t i = 0; i < BYTES_PAYLOAD; i++)
    {
        testdata[i] = 0x20;
    }
    uint16_t testchecksum = 0;
    uint8_t error = 0;
    error = calcChecksum(testdata, &testchecksum);
    printf("Testchecksum: %d\n", testchecksum);
}

void testStoreFrame()
{
    Frame myStorageFrame;
    char myRawFrame[BYTES_FRAME_TOTAL];
    uint8_t myMsgId = 0x01;
    uint8_t myAck = 0x00;
    uint8_t myPeerNr = 0x01;
    uint8_t myPayloadLength = 4;
    char myInputData[5]= "Test";
    createRawFrame(myRawFrame , myMsgId, myAck, myPeerNr, myPayloadLength, myInputData);
    printf("RawFrame: %s\n", myRawFrame);
    for(int i = 0; i < BYTES_FRAME_TOTAL; i++)
    {
        printf("Rawframe index %d: %X\n", i, myRawFrame[i]);
    }
    storeFrame(&myStorageFrame, myRawFrame);

    printf("MsgID: %d\n", myStorageFrame.msgId);
    printf("ACK: %d\n", myStorageFrame.ack);
    printf("PeerNr: %d\n", myStorageFrame.peerNr);
    printf("PayloadLength: %d\n", myStorageFrame.payloadLength);
    printf("Payload: %s\n", myStorageFrame.payload);
    printf("Checksum: %d\n", myStorageFrame.checksum);

    for(int i=0; i<5; i++){
        logMessage(myStorageFrame, logfilePathTesting);
    }
}

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

    createLog(logfilePathTesting);
    testChecksum();
    testStoreFrame();

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


        UI_MAIN();
    

    // UI_LOG();

    pthread_exit(NULL);
}

