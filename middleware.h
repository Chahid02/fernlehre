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


#define BYTES_PAYLOAD 32
#define MESSAGESIZE 1+1+1+BYTES_PAYLOAD+2

//Typedefs:

typedef struct gm
{   
    uint8_t id;                     //ID-Number of groupmember
    char ipv4[16];              //ipv4 adress of groupmember
    uint16_t port;              //Port of groupmember
    struct sockaddr_in addr;    //socket adress of groupmember
}groupmember;

typedef struct mes
{
    uint8_t id;         //Message ID
    uint8_t ack;        //Acknoledge (1/0)
    uint8_t peer;       //peer id
    char payload[32];   //message
    uint16_t cs;        //checksum
}message;

//Global Variables:
char buffer[BYTES_PAYLOAD];     //Connection between MW and UI
uint8_t groupsize = 2;          //Amount of group members (needs to be set by UI)
uint8_t myID = 0;               //ID of Peer (needs to be set by UI)
uint32_t message_cnt = 0;       //message counter represents the latest message id


/*
data: payload for which the checksum be calculated
checksumBuffer: Pointer to data where checksum will be stored
length: length of data in bytes
*/

int middleware(void);
int sendgroup(groupmember (*mygroup)[], int groupsize, int myID, int *mysocket, char *payload);
int recvgroup(int *mysocket, char *message_recv);
int getID(void);
void getMembers(groupmember (*mygroup)[], int groupsize);
int setupMW(groupmember (*mygroup)[],int myID, int *mysocket);
groupmember parsemes(char *message);
int ACK(groupmember (*mygroup)[], int *mysocket, int peerid); 
uint8_t calcChecksum(char* data, uint16_t* checksumBuffer);

#endif