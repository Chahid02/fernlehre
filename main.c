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

char *logfilePathTesting = "../log.txt"; // das benutzen für Middlewaretests, später den Path aus der GUI benutzen

inputData myInputData = {.newMsgReceived = false};

extern int inputID_FLAG;
extern int inputID_GLOBAL;

extern char frameToSend[BYTES_FRAME_TOTAL]; // Connection between MW and UI
extern uint8_t groupsize;                   // Amount of group members (needs to be set by UI)
extern uint8_t myID;                        // ID of Peer (needs to be set by UI)
extern uint32_t message_cnt;                // message counter represents the latest message id

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
    inputData myInputData;
    char myRawFrame[BYTES_FRAME_TOTAL];
    uint8_t myMsgId = 0x01;
    uint8_t mySndId = 0x01;
    uint8_t myAck = 0x00;
    uint8_t myPeerNr = 0x01;
    myInputData.msgLength = 4;
    myInputData.userMsg = "Test";
    createRawFrame(myRawFrame, myMsgId, mySndId, myAck, myPeerNr, myInputData);
    myInputData.newMsgReceived = false;
    printf("RawFrame: %s\n", myRawFrame);
    for (int i = 0; i < BYTES_FRAME_TOTAL; i++)
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

    for (int i = 0; i < 5; i++)
    {
        logMessage(myStorageFrame, logfilePathTesting);
    }
}

// testfunction to test middleware without UI
void testMiddleWare()
{
    Frame myStorageFrame;
    int inputID;

     //printf("Enter group ID:\n");
     printf("[X] Enter your GROUP ID:");
     scanf("%d", &inputID);

    myID = (uint8_t)inputID;
    switch (myID)
    {
    case 0:
        myInputData.userMsg = "This is peer 1.";
        myInputData.msgLength = strlen(myInputData.userMsg);
        myInputData.newMsgReceived = true;
        break;

    case 1:
        myInputData.userMsg = "This is peer 2.";
        myInputData.msgLength = strlen(myInputData.userMsg);
        myInputData.newMsgReceived = true;
        break;

    case 2:
        myInputData.userMsg = "This is peer 3.";
        myInputData.msgLength = strlen(myInputData.userMsg);
        myInputData.newMsgReceived = true;
        break;

    default:
        printf("Invalid ID\n");
    }
    createRawFrame(frameToSend, 1, myID, 0x00, myID, myInputData);
    // myInputData.newMsgReceived = false;

    storeFrame(&myStorageFrame, frameToSend);

    printf("MsgID: %d\n", myStorageFrame.msgId);
    printf("ACK: %d\n", myStorageFrame.ack);
    printf("PeerNr: %d\n", myStorageFrame.peerNr);
    printf("PayloadLength: %d\n", myStorageFrame.payloadLength);
    printf("Payload: %s\n", myStorageFrame.payload);
    printf("Checksum: %d\n", myStorageFrame.checksum);
}

int main(int argc, char **argv)
{
    clrscr();
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
        printf("---------------------------------------------\n");
        printf("----- MULTI THREADING  ----------------------\n");
        printf("---------------------------------------------\n");
        printf("[X] Created Thread ID, %d\r\n", threadCreate);
    }

    createLog(logfilePathTesting);
            testMiddleWare();
            middleware();
        

        //testMiddleWare();
        // testChecksum();
        // testStoreFrame();
        //middleware();
 

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

    printf("[X] UI INTERFACE ID, %ld started\r\n", thread_ID);

    if (do_mutex)
    {
       pthread_mutex_lock(&mutex);
       printf("MUTEX ON");
        UI_MAIN();
    }
    else
    {
        pthread_mutex_unlock(&mutex);
        printf("MUTEX ON");
        UI_MAIN();
        

    }
    
    // UI_LOG();

    pthread_exit(NULL);
}
