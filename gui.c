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
int file_descriptor;
int file_descriptor2;
int readSize;
char filename[100];

/*------------------------*/
/*---- MAIN FUNCTION -----*/
/*------------------------*/
void UI_MAIN(void)
{
    while (1)
    {
        UI_START();
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
void UI_START(void)
{

    /*--------------------------------*/
    /*---- CONFIG FILE Descriptor ----*/
    /*--------------------------------------------------------------------------------------------*/
    char *configContent = (char *)calloc(500, sizeof(char *)); // reserve file content with 500 character

    /*--------------------------------------------------------------------------------------------*/

    /*--------------------------------*/
    /*---- LOG FILE Descriptor ----*/
    /*--------------------------------------------------------------------------------------------*/
    char *logContent = (char *)calloc(500, sizeof(char *)); // reserve file content with 500 character

    /*--------------------------------------------------------------------------------------------*/

    /*-------------------------*/
    /*---- GUI SELECTION ------*/
    /*--------------------------------------------------------------------------------------------*/

    GUI_SELECTION();

    /*--------------------------------------------------------------------------------------------*/
    int CMD_NR;
    printf("[X] If you make a typing error, please press enter and type again !\r\n");
    printf("[X] Enter your CMD (1 < n < 10): #");

    do
    {
        fflush(stdin);
    } while (!scanf("%d", &CMD_NR) && getchar());

    printf("[X] VALUE:%d \r\n", CMD_NR);

    char nr[20], ip[20], port[20], log[20];
    char *searchNewLine;
    clrscr();
    /*---------------------------------*/
    /*---- UI INTERFACE SELECTION -----*/
    /*--------------------------------------------------------------------------------------------*/
    switch (CMD_NR)
    {
    case 1:
        printf("---------------------------------------------\n");
        printf("----- CONFIGURE CONFIG FILE -----------------\n");
        printf("---------------------------------------------\n");
        printf("[X] Enter the Path of the Config File:");

        scanf("%s", filename) == EOF;
        // snprintf(filename, sizeof(filename), "%d", configPath);
        file_descriptor = open(filename, O_RDWR | O_CREAT, 0777);

        if (file_descriptor == -1)
        {
            perror("[X] CONFIG FILE NOT FOUND");
            exit(1);
        }
        else
        {
            printf("[X] CONFIG FILE FOUND OR CREATED SUCCESSFULLY\r\n");
        }

        // string end char '\0'
        configContent[499] = '\0';
        break;

    case 2:
        printf("---------------------------------------------\n");
        printf("----- CONFIG FILE ---------------------------\n");
        printf("---------------------------------------------\n");
        file_descriptor = open(filename, O_RDWR | O_CREAT, 0777);
        if (file_descriptor == -1)
        {
            printf("[X] CONFIG PATH NOT CONFIGURED !!!\r\n");
            printf("[X] PLEASE CONFIGURE THE PATH \r\n");
            break;
            /*
            perror("[X] [X] CONFIG PATH NOT CONFIGURED !!!");
            exit(1);
            */

            break;
        }
        else
        {

            readSize = read(file_descriptor, configContent, 500);
            printf("[X] Reading file : %s\n", configPath);
            printf("[X] Number of bytes written in file %s: %d\r\n", configPath, readSize);
            printf("[X] configContent:\n%s", configContent);
        }

        break;
    case 3:
        printf("---------------------------------------------\n");
        printf("----- PEER INFORMATION ----------------------\n");
        printf("---------------------------------------------\n");
        file_descriptor = open(filename, O_RDWR | O_CREAT, 0777);
        if (file_descriptor == -1)
        {
            printf("[X] CONFIG PATH NOT CONFIGURED !!!\r\n");
            printf("[X] PLEASE CONFIGURE THE PATH \r\n");
            break;
            /*
            perror("[X] [X] CONFIG PATH NOT CONFIGURED !!!");
            exit(1);
            */

            break;
        }
        else
        {

            readSize = read(file_descriptor, configContent, 500);

            sscanf(configContent, "%s%s%s%s", nr, ip, port, log);
            printf("[X] Reading:%s--%s--%s--%s\n", nr, ip, port, log);

            searchNewLine = strstr(configContent, "#5");
            sscanf(searchNewLine, "%s%s%s%s", nr, ip, port, log);
            printf("[X] Reading:%s--%s--%s--%s--@pos%ld\n", nr, ip, port, log, searchNewLine - configContent);

            char logPeer5[100];
            snprintf(logPeer5, sizeof(logPeer5), "%s--%s--%s--%s", nr, ip, port, log);
            printf("[X] PeerNr5 : %s\n", logPeer5);
        }
        break;

    case 4:
        break;

    case 5:
        break;

    case 6:
        break;

    case 7:
        break;
    case 8:
        break;

    case 9:
        break;

    case 10:
        break;

    default:
        break;
    }
    // free buffer of configContent
    free(configContent);

    /*-------------------------*/
    /*---- Close Programm -----*/
    /*--------------------------------------------------------------------------------------------*/
    close(file_descriptor);
}

void GUI_SELECTION(void)
{
    printf("\n");
    printf("---------------------------------------------\n");
    printf("----- UI SELECTION --------------------------\n");
    printf("---------------------------------------------\n");
    printf("[X] NUMBER:1 -----> CONFIGURE CONFIG PEER FILE \n");
    printf("[X] NUMBER:2 -----> READ CONFIG PEER FILE \n");
    printf("[X] NUMBER:3 -----> GET PEER INFORMATION \n");
    printf("[X] NUMBER:4 -----> CREATE LOG FILE \n");
    printf("[X] NUMBER:5 -----> READ LOG FILE \n");
    printf("[X] NUMBER:6 -----> DEFINE PAYLOAD SIZE \n");
    printf("[X] NUMBER:7 -----> ERROR INJECTION BIT \n");
    printf("[X] NUMBER:8 -----> SEND MESSAGE \n");
    printf("[X] NUMBER:9 -----> SENT MESSAGES \n");
    printf("[X] NUMBER:10 ----> RECEIVED MESSAGES \n");
    printf("---------------------------------------------\n");
}
