/*!
**************************************************************
* @file    gui.c
* @brief   GUI SOURCE file
* @author  GRUPPE *
* @version V1.0
* @date    2022-01-27
* @copyright Copyright (c) FH CAMPUS ESE22. All rights reserved.
**************************************************************
*/

/*--------------------------*/
/*---- STANDARD INCLUDE ----*/
/*--------------------------*/
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

/*------------------------*/
/*---- PRIVATE INCLUDE----*/
/*------------------------*/
#include "gui.h"

/*------------------------*/
/*---- FILE DESC VARS ----*/
/*------------------------*/
int file_descriptor, readSize;
char filename[100];

void UI_INT(void)
{
    while (1)
    {  
        UI_start();
      
    }
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
void UI_start(void)
{
    printf("----------------------------\n");
    printf("----- Programm started -----\n");
    printf("----------------------------\n");

    /*-------------------------*/
    /*---- File Descriptor ----*/
    /*--------------------------------------------------------------------------------------------*/
    char *fileContent = (char *)calloc(500, sizeof(char *)); // reserve file content with 500 character
    snprintf(filename, sizeof(filename), "%s", configFile);  //
    file_descriptor = open(filename, O_RDONLY);

    if (file_descriptor == -1)
    {
        perror("File Not found.");
        exit(1);
    }
    readSize = read(file_descriptor, fileContent, 500);
    // string end char '\0'
    fileContent[499] = '\0';
    /*--------------------------------------------------------------------------------------------*/

    int CMD_NR;
    printf("[X] Enter your CMD :");

    while (scanf("%d", &CMD_NR) == EOF)
    {
    };

    printf("[X] Value Choosen:%d \r\n", CMD_NR);

    char nr[20], ip[20], port[20], log[20];
    char *searchNewLine;

    /*---------------------------------*/
    /*---- UI INTERFACE SELECTION -----*/
    /*--------------------------------------------------------------------------------------------*/
    switch (CMD_NR)
    {
    case 1:

        printf("[X] Reading file : %s\n", configFile);
        printf("[X] Number of bytes written in file %s: %d\r\n", configFile, readSize);
        printf("[X] fileContent:\n%s", fileContent);

        break;
    case 2:
        sscanf(fileContent, "%s%s%s%s", nr, ip, port, log);
        printf("[X] Reading:%s--%s--%s--%s\n", nr, ip, port, log);

        searchNewLine = strstr(fileContent, "#5");
        sscanf(searchNewLine, "%s%s%s%s", nr, ip, port, log);
        printf("[X] Reading:%s--%s--%s--%s--@pos%ld\n", nr, ip, port, log, searchNewLine - fileContent);

        char logPeer5[100];
        snprintf(logPeer5, sizeof(logPeer5), "%s--%s--%s--%s", nr, ip, port, log);
        printf("[X] PeerNr5 : %s\n", logPeer5);

        break;

    default:
        break;
    }
    // free buffer of fileContent
    free(fileContent);

    /*-------------------------*/
    /*---- Close Programm -----*/
    /*--------------------------------------------------------------------------------------------*/
    close(file_descriptor);
}
