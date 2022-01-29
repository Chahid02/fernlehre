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
#include <time.h>
#include <string.h>
#include <errno.h>

#include "gui.h"

char logFilename[100] = {0};
char configFilename[100] = {0};
int readSize;

bool IS_INT_STRING(char *input)
{
    int i;
    for (i = 0; i < strlen(input); i++)
    {
        if (input[i] < '0' || input[i] > '9')
        {
            return false;
        }
    }
    return true;
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
    printf("[X] NOTE: INPUT BUFFER IS BROKEN AS FUCK !!!!\r\n");
    printf("[X] If you make a typing error, please press enter and type again !\r\n");
    printf("[X] Enter your CMD (1 < n < 10): #");
}

void UI_CONF_CONFIG(void)
{
    int file_descriptor;

    printf("---------------------------------------------\n");
    printf("----- CONFIGURE CONFIG FILE -----------------\n");
    printf("---------------------------------------------\n");
    printf("[X] Enter the Path of the Config File:");

    scanf("%s", configFilename);
    // snprintf(filename, sizeof(filename), "%d", configPath);
    file_descriptor = open(configFilename, O_RDWR | O_CREAT, 0777);

    if (file_descriptor == -1)
    {
        perror("[X] CONFIG FILE NOT FOUND");
        //exit(1);
    }
    else
    {
        printf("[X] CONFIG FILE FOUND OR CREATED SUCCESSFULLY\r\n");
        close(file_descriptor);
    }

}

void UI_READ_CONFIG(void)
{
    printf("---------------------------------------------\n");
    printf("----- CONFIG FILE ---------------------------\n");
    printf("---------------------------------------------\n");
    fflush(stdout);
    /*--------------------------------*/
    /*---- CONFIG FILE Descriptor ----*/
    int file_descriptor;

    /*--------------------------------------------------------------------------------------------*/
    char *configContent = (char *)calloc(500, sizeof(char *)); // reserve file content with 500 character

    /*--------------------------------------------------------------------------------------------*/
    file_descriptor = open(configFilename, O_RDWR, 0777);
    if (file_descriptor == -1)
    {
        printf("[X] CONFIG PATH NOT CONFIGURED !!!\r\n");
        printf("[X] PLEASE CONFIGURE THE PATH \r\n");
        fflush(stdout);
    }
    else
    {

        readSize = read(file_descriptor, configContent, 500);
        printf("[X] Reading file : %s\n", configFilename);
        printf("[X] Number of bytes written in file %s: %d\r\n", configFilename, readSize);
        printf("[X] configContent:\n%s", configContent);
        fflush(stdout);
        free(configContent);
        close(file_descriptor);
    }

}

void UI_PEER_INFO(void)
{
    int file_descriptor;
    /*--------------------------------*/
    /*---- CONFIG FILE Descriptor ----*/
    /*--------------------------------------------------------------------------------------------*/
    char *configContent = (char *)calloc(500, sizeof(char *)); // reserve file content with 500 character

    /*--------------------------------------------------------------------------------------------*/
    printf("---------------------------------------------\n");
    printf("----- PEER INFORMATION ----------------------\n");
    printf("---------------------------------------------\n");

    file_descriptor = open(configFilename, O_RDONLY, 0777);
    if (file_descriptor == -1)
    {
        printf("[X] CONFIG PATH NOT CONFIGURED !!!\r\n");
        printf("[X] PLEASE CONFIGURE THE PATH \r\n");
    }
    else
    {
        char nr[20], ip[20], port[20], log[20];
        char *searchNewLine;
        char maxLenght[] = PEER_NR;
        char inputNr[sizeof(PEER_NR)];
        char searchNr[sizeof(PEER_NR) + 1];

        printf("[X] Enter the PEER #NR:");
        int ret = scanf("%s", inputNr);
        snprintf(searchNr, sizeof("#"), "%s", "#");
        int inputDec = atoi(inputNr);
        readSize = read(file_descriptor, configContent, 500);
        if (inputDec <= PEER_MAX_DEC && readSize > 0)
        {
            snprintf(searchNr, sizeof(inputNr), "%s", inputNr);
            // snprintf(searchNr, sizeof("#"), "%s", "#");

            sscanf(configContent, "%s%s%s%s", nr, ip, port, log);
            printf("[X] Reading:%s--------%s--------%s--%s\n", nr, ip, port, log);
            fflush(stdout);

            searchNewLine = strstr(configContent, searchNr);
            sscanf(searchNewLine, "%s%s%s%s", nr, ip, port, log);
            printf("[X] PEER %s: %s--%s--%s--%s--@pos%ld\n", searchNr, nr, ip, port, log, searchNewLine - configContent);
            fflush(stdout);
        }
        else
        {

            printf("[X] WRONG PEER #NR OR FILE IS EMPTY!!!");
            fflush(stdout);
        }
        configContent[499] = '\0';
        close(file_descriptor);
    }
}

void UI_LOG(void)
{
    printf("---------------------------------------------\n");
    printf("----- CREATE LOG FILE -----------------------\n");
    printf("---------------------------------------------\n");
    printf("[X] Enter the Path of the LOG File:");

    scanf("%s", logFilename);
    // snprintf(filename, sizeof(logFilename), "%d", logPath);
    int file_descriptor = open(logFilename, O_RDWR | O_CREAT, 0777);

    if (file_descriptor == -1)
    {
        perror("[X] LOG FILE NOT FOUND");
        //exit(1);
    }
    else
    {
        printf("[X] LOG FILE FOUND OR CREATED SUCCESSFULLY\r\n");
    
        close(file_descriptor);
    }

}

void timeStampFunc(void)
{
    char cur_time[128];

    time_t t;
    struct tm *ptm;

    t = time(NULL);
    ptm = localtime(&t);

    strftime(cur_time, 128, "%d-%b-%Y %H:%M:%S", ptm);

    printf("Current date and time: %s\n", cur_time);
}

void UI_LOG_WRITE(void)
{
    printf("---------------------------------------------\n");
    printf("----- CREATE LOG FILE -----------------------\n");
    printf("---------------------------------------------\n");
    FILE *fp = fopen(logFilename, "a+");
    if (fp == NULL)
    {
        perror("[X] LOG FILE NOT FOUND");
        //exit(1);
    }
    else
    {
    LOG(INFO, "File open success.");
    LOG(WARN, "File path missing.");
    LOG(ERROR, "File close failed.");
    fclose(fp);
    }
   
}




void UI_MAIN(void)
{
    char s[100];
    char *end;
    bool correct = false;
    long result;

    while (1)
    {

        clrscr();

        int switchNumber = (int)result;
        switch (switchNumber)
        {

        case 1:
            UI_CONF_CONFIG();
            break;
        case 2:
            UI_READ_CONFIG();
            break;
        case 3:
            UI_PEER_INFO();
            break;
        case 4:
            UI_LOG();
            break;

            case 5:
            UI_LOG_WRITE();
            break;

        default:
            break;
        }

        GUI_SELECTION();

        do
        {
            int returnVal = scanf("%s", s);

            if (IS_INT_STRING(s))
            {
                correct = true;
                result = strtol(s, &end, 10);
                break;
            }
            else
            {
                printf("[X] Enter your CMD (1 < n < 10): #");
            }
        } while (!correct);
    }
}
