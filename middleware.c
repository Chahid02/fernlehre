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
