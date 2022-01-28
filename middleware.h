#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>



#define BYTES_PAYLOAD 32


/*
data: payload for which the checksum be calculated
checksumBuffer: Pointer to data where checksum will be stored
length: length of data in bytes
*/

uint8_t calcChecksum(char* data, uint16_t* checksumBuffer);



#endif