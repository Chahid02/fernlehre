#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#define BYTES_MSG_ID 1
#define BYTES_ACK 1
#define BYTES_PEER_NR 1
#define BYTES_PAYLOAD_LENGTH 1
#define BYTES_PAYLOAD 32
#define BYTES_CHECKSUM 2
#define BYTES_TERM 1
#define BYTES_FRAME_TOTAL (BYTES_MSG_ID + BYTES_ACK + BYTES_PEER_NR + BYTES_PAYLOAD + BYTES_CHECKSUM + BYTES_TERM) //38 Bytes total

typedef struct
{
    uint8_t msgId;
    uint8_t ack;
    uint8_t peerNr;
    uint8_t payloadLength;
    char payload[BYTES_PAYLOAD + 1];
    uint8_t checksum;
}Frame;




/*
data: payload for which the checksum be calculated
checksumBuffer: Pointer to data where checksum will be stored
length: length of data in bytes
*/

uint8_t calcChecksum(char* data, uint16_t* checksumBuffer);

uint8_t storeFrame(Frame* storageFrame, char* rawFrame);

uint8_t createRawFrame(char rawFrame[BYTES_FRAME_TOTAL] , uint8_t msgId, uint8_t ack, uint8_t peerNr, uint8_t payloadLength, char* inputData);



#endif