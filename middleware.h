#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/uio.h>
#include <fcntl.h>

#include "main.h"

/*
#define DEBUG //See everything thats going on
#define ACK   //See if message gets acknoledged
*/

#define BYTES_MSG_ID 1
#define BYTES_SENDER_ID 1
#define BYTES_ACK 1
#define BYTES_PEER_NR 1
#define BYTES_PAYLOAD_LENGTH 1
#define BYTES_PAYLOAD 32
#define BYTES_CHECKSUM 2
#define BYTES_TERM 1
#define BYTES_FRAME_TOTAL (BYTES_MSG_ID + BYTES_SENDER_ID + BYTES_ACK + BYTES_PEER_NR + BYTES_PAYLOAD + BYTES_CHECKSUM + BYTES_TERM) //40 Bytes total

//------------------------------------
//CHANGE GROUPSIZE
#define MAX_PEERS 5
//------------------------------------

typedef struct
{
    uint8_t msgId;
    uint8_t sndId;
    uint8_t ack;
    uint8_t peerNr;
    uint8_t payloadLength;
    char payload[BYTES_PAYLOAD + 1];
    uint16_t checksum;
}Frame;

typedef struct gm
{   
    uint8_t id;                     //ID-Number of groupmember
    char ipv4[16];              //ipv4 adress of groupmember
    uint16_t port;              //Port of groupmember
    struct sockaddr_in addr;    //socket adress of groupmember
}groupmember;

typedef struct
{
    char* userMsg;
    uint8_t msgLength;
    bool newMsgReceived;
}inputData;


//Global Variables:
extern char frameToSend[BYTES_FRAME_TOTAL];     //Connection between MW and UI
extern uint8_t groupsize;        //Amount of group members (needs to be set by UI)
extern uint8_t myID;              //ID of Peer (needs to be set by UI)
extern uint32_t message_cnt;       //message counter represents the latest message id
extern groupmember mygroup[MAX_PEERS];
extern pthread_mutex_t mymutex;
extern pthread_mutex_t mutexInput;
extern pthread_mutex_t mutexBitError;
extern inputData myInputData;
extern bool errorInjectionCalled;
extern uint16_t userErrorBit;

int middleware(void);
int sendgroup(groupmember (*mygroup)[], int groupsize, int myID, int *mysocket, char *payload);
int recvgroup(int *mysocket);
void getID(void);
void getMembers(groupmember (*mygroup)[], int groupsize);
int setupMW(groupmember (*mygroup)[],int myID, int *mysocket);
groupmember parsemes(char *message);
int ACK(groupmember (*mygroup)[], int *mysocket, int peerid);
bool readConfig(const char *filename);
int getWords(char *base, char target[20][120]);

/*
data: payload for which the checksum be calculated
checksumBuffer: Pointer to data where checksum will be stored
*/
uint8_t calcChecksum(char* data, uint16_t* checksumBuffer);

/*
stores the information of the received frame rawFrame in the struct storageFrame
*/
uint8_t storeFrame(Frame* storageFrame, char rawFrame[BYTES_FRAME_TOTAL]);

/*
creates a raw frame for sending according to the frame format, with the given input data
*/
uint8_t createRawFrame(char rawFrame[BYTES_FRAME_TOTAL] , uint8_t msgId, uint8_t sndId, uint8_t ack, uint8_t peerNr, inputData userInputData);

/*
creates logfile if it doesn't exist, or clears the old one if it exists, and writes header information
*/
uint8_t createLog(char* filepath);

/*
writes info from msgFrame into logfile at path filepath
*/
uint8_t logMessage(Frame msgFrame, char* filepath);

/*
incect biftlip in payload at given index bitIndex
*/
uint8_t injectError(char* rawFrame, uint16_t bitIndex);



#endif