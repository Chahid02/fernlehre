#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "gui.h"

int file_descriptor, readSize;
char filename[100];

void UI_start(void)
{
    char *fileContent = (char *)calloc(500, sizeof(char *));

    printf("----------------------------\n");
    printf("----- Programm started -----\n");
    printf("----------------------------\n");

    printf("[X] Reading file : %s\n", configFile);
    snprintf(filename, sizeof(filename), "%s", configFile);
    file_descriptor = open(filename, O_RDONLY);
    if (file_descriptor == -1)
    {
        perror("File Not found.");
        exit(1);
    }
    readSize = read(file_descriptor, fileContent, 500);

    fileContent[499] = '\0';
    printf("[X] fileContent:\n%s", fileContent);
    printf("[X] Number of bytes read %s: %d\n", configFile, readSize);

    char nr[20], ip[20], port[20], log[20];
    char *searchNewLine;
    sscanf(fileContent, "%s%s%s%s", nr, ip, port, log);
    printf("[X] Reading:%s--%s--%s--%s\n", nr, ip, port, log);
    searchNewLine = strstr(fileContent, "#5");
    sscanf(searchNewLine, "%s%s%s%s", nr, ip, port, log);
    printf("[X] Reading:%s--%s--%s--%s--@pos%ld\n", nr, ip, port, log, searchNewLine - fileContent);

    char logPeer5[100];
    snprintf(logPeer5, sizeof(logPeer5), "%s--%s--%s--%s", nr, ip, port, log);
    printf("[X] PeerNr5 : %s\n", logPeer5);

    free(fileContent);

    /*----------------------------------------------
    Closes Programm
    ----------------------------------------------*/

    close(file_descriptor);
}