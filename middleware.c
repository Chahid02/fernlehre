#include "middleware.h"



uint8_t calcChecksum(char* data, uint16_t* checksumBuffer)
{
    uint8_t errCode = 0;
    uint32_t checksum = 0x00;

    for (int i = 0; i + 1 < BYTES_PAYLOAD; i+=2)
    {
        uint16_t dataBlock;
        memcpy(&dataBlock, data + i, 2);
        checksum += dataBlock;
        //printf("Checksum before bitshift: %d \n", checksum);
        if(checksum > 0xFFFF)
        {
            checksum -= 0xFFFF;
        }
        //printf("Checksum after bitshift: %d\n", checksum);
    }
    *checksumBuffer = ~checksum;

    return errCode;
}

uint8_t storeFrame(Frame* storageFrame, char* rawFrame)
{
    uint8_t errCode = 0;
    uint8_t bufferPosition = 0;

    if(rawFrame == NULL || storageFrame == NULL)
    {
        printf("NULL Pointer! \n");
    }

    if(strlen(rawFrame) != BYTES_FRAME_TOTAL)
    {
        printf("Invalid frame length! \n");
    }

    memcpy(&(storageFrame->msgId), rawFrame, BYTES_MSG_ID);
    bufferPosition = BYTES_MSG_ID;
    memcpy(&(storageFrame->ack), rawFrame + bufferPosition, BYTES_ACK);
    bufferPosition += BYTES_ACK;
    memcpy(&(storageFrame->peerNr), rawFrame + bufferPosition, BYTES_PEER_NR);
    bufferPosition += BYTES_PEER_NR;
    memcpy(&(storageFrame->payloadLength), rawFrame + bufferPosition, BYTES_PAYLOAD_LENGTH);
    bufferPosition += BYTES_PAYLOAD_LENGTH;
    memcpy(&(storageFrame->payload), rawFrame + bufferPosition, BYTES_PAYLOAD);
    bufferPosition += BYTES_PAYLOAD;
    storageFrame->payload[BYTES_PAYLOAD] = '\0';
    memcpy(&(storageFrame->checksum), rawFrame + bufferPosition, BYTES_CHECKSUM);

    return errCode;
}


uint8_t createRawFrame(char rawFrame[BYTES_FRAME_TOTAL], uint8_t msgId, uint8_t ack, uint8_t peerNr, uint8_t payloadLength, char* inputData)
{
    uint8_t errCode = 0;
    uint8_t bufferPosition = 0;
    char payloadTemp[BYTES_PAYLOAD + 1];
    uint16_t checksum;

    rawFrame[0] = msgId;
    bufferPosition += BYTES_MSG_ID;
    rawFrame[bufferPosition] = ack;
    bufferPosition += BYTES_ACK;
    rawFrame[bufferPosition] = peerNr;
    bufferPosition += BYTES_PEER_NR;
    rawFrame[bufferPosition] = payloadLength;
    bufferPosition += BYTES_PAYLOAD_LENGTH;

    //copy inputData to payloadTemp 
    for (uint8_t i = 0; i < payloadLength; i++)
    {
        payloadTemp[i] = inputData[i];
        //rawFrame[bufferPosition + i] = inputData[i];
    }
    //fill the unused bytes of the payload with zeroes
    for (uint8_t i = 0; i < BYTES_PAYLOAD - payloadLength; i++)
    {
        payloadTemp[payloadLength + i] = 0x00;
    }
    payloadTemp[BYTES_PAYLOAD] = '\0';

    //copy all data bytes except the terminating char to rawFrame
    memcpy(rawFrame+bufferPosition, payloadTemp, BYTES_PAYLOAD);

    bufferPosition += BYTES_PAYLOAD;
    errCode = calcChecksum(payloadTemp, &checksum);
    memcpy(rawFrame+bufferPosition, &checksum, BYTES_CHECKSUM);


    return errCode;
}