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
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <inttypes.h>

#include "gui.h"
#include "middleware.h"
#include "main.h"

extern int PAYLOAD;

int switchNumber;
char s[100];
char *end;
bool correct = false;
long result;
// extern int inputID_GLOBAL = 0;
// extern int inputID_FLAG = 0;
char logfilePath[100] = {0};
char configfilePath[100] = {0};;
int readSize;
int LogCreateFlag = 0;

char lineText[120];
char TextMember[120];
int lines = 0;

char str[20][120], s1[120];
int k = 5, found = 0;
char arr[20][120];

static char userInputBuffer[BYTES_PAYLOAD+1];

int getWords(char *base, char target[20][120])
{
    int n = 0, i, j = 0;

    for (i = 0; true; i++)
    {
        if (base[i] != ' ')
        {
            target[n][j++] = base[i];
        }
        else
        {
            target[n][j++] = '\0'; // insert NULL
            n++;
            j = 0;
        }
        if (base[i] == '\0')
            break;
    }
    return n;
}

void searchString(char str[20][120], char s1[120], int j)
{
    int found;
    int n = j;

    for (int i = 0; i < n; i++)
    {
        if (strcmp(s1, str[i]) == 0)
        {
            found = 1;
            printf("Found in row-%d\n", i + 1);
        }
    }

    if (found == 0)
        printf("Not found\r\n");
}

bool read_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return false;

    char *line = NULL;
    size_t linesize = 0;

    while (getline(&line, &linesize, file) != -1)
    {
        snprintf(str[lines], sizeof(lineText), "%s", lineText);
        int j = snprintf(lineText, linesize, "%s", line);

        // printf("%s", str[lines]);
        str[lines][120] = '\0';
        lineText[120] = '\0';
        char *pch = strstr(str[lines], "#1");

        if (pch)
        {
            found = 1;
            // printf("Found in row@%s\n ",pch);
        }

        int n = getWords(lineText, arr);
        int i = 0;
        while (i <= n)
        {
            char nr[20], ip[20], port[20], logF[20];

            sscanf(arr[i], "%s%s%s%s", nr, ip, port, logF);
            char *ptr;
            long value = strtol(nr + 1, &ptr, 10);

            mygroup[lines - 1].id = value;
            mygroup[lines - 1].id = value;
            snprintf(mygroup[lines - 1].ipv4, sizeof(mygroup[lines - 1].ipv4), "%s", ip);
            *ptr;
            value = strtol(port, &ptr, 10);
            mygroup[lines - 1].port = value;

            i++;
        }

        lines++;
    }
    lines = 0;
    free(line);
    fclose(file);

    return true;
}

// void UI_MW_MEMBER(void)
// {

//     if (!read_file(configfilePath))
//     {
//         printf("Error reading file\n");
//         exit(EXIT_FAILURE);
//     }

//     for (int i = 0; i < PEER_MAX_DEC; i++)
//     {
//         printf("ID: %i ", mygroup[i].id);
//         printf("IPv4: %s ", mygroup[i].ipv4);
//         printf("Port: %i\n", mygroup[i].port);
//     }
// }

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
    printf("[X] NUMBER:1 -----> SEND MESSAGE \n");
    printf("[X] NUMBER:2 -----> SHOW RECEIVED MESSAGES (read log file) \n");
    printf("[X] NUMBER:3 -----> SET ERROR INJECTION BIT \n");
    printf("[X] NUMBER:4 -----> SHOW INFOS ABOUT GROUP \n");
    printf("---------------------------------------------\n");
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

    scanf("%s", configfilePath);
    // snprintf(filename, sizeof(filename), "%d", configPath);
    file_descriptor = open(configfilePath, O_RDONLY, 0777);

    if (file_descriptor == -1)
    {
        perror("[X] CONFIG FILE NOT FOUND (please create one before starting)\n");
        exit(-1);
    }
    else
    {
        printf("[X] CONFIG FILE FOUND\r\n");
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
    /*--------------------------------------------------------------------------------------------*/
    int file_descriptor;
    char *configContent = (char *)calloc(500, sizeof(char *)); // reserve file content with 500 character

    /*--------------------------------------------------------------------------------------------*/
    file_descriptor = open(configfilePath, O_RDWR, 0777);
    if (file_descriptor == -1)
    {
        printf("[X] CONFIG PATH NOT CONFIGURED !!!\r\n");
        printf("[X] PLEASE CONFIGURE THE PATH \r\n");
        fflush(stdout);
    }
    else
    {

        readSize = read(file_descriptor, configContent, 500);
        printf("[X] Reading file : %s\n", configfilePath);
        printf("[X] Number of bytes written in file %s: %d\r\n", configfilePath, readSize);
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

    file_descriptor = open(configfilePath, O_RDONLY, 0777);
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

            strcat(searchNr, inputNr);
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

    scanf("%s", logfilePath);
    // snprintf(filename, sizeof(logfilePath), "%d", logPath);
    int file_descriptor = open(logfilePath, O_RDWR | O_CREAT, 0777);

    if (file_descriptor == -1)
    {
        perror("[X] LOG FILE NOT FOUND");
        // exit(1);
        LogCreateFlag = 0;
    }
    else
    {
        printf("[X] LOG FILE FOUND OR CREATED SUCCESSFULLY\r\n");
        LogCreateFlag = 1;
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

void UI_GROUPID(void)
{
    printf("---------------------------------------------\n");
    printf("----- GROUP ID SELECTION --------------------\n");
    printf("---------------------------------------------\n");
    printf("Please enter your ID! (0-%i)\n",groupsize-1);

    char buffer[4];

    fgets(buffer, 3, stdin);
    
    pthread_mutex_lock(&mymutex);
    myID = atoi(buffer);
    printf("\nYou selected: %i\n",myID);
    pthread_mutex_unlock(&mymutex);

}

void UI_LOG_READ(void)
{
    printf("---------------------------------------------\n");
    printf("----- READ LOG FILE -------------------------\n");
    printf("---------------------------------------------\n");
    fflush(stdout);
    /*--------------------------------*/
    /*---- LOG FILE Descriptor ----*/
    int file_descriptor;

    /*--------------------------------------------------------------------------------------------*/
    char *logContent = (char *)calloc(500, sizeof(char *)); // reserve file content with 500 character

    /*--------------------------------------------------------------------------------------------*/
    file_descriptor = open(logfilePath, O_RDWR, 0777);
    if (file_descriptor == -1)
    {
        printf("[X] CONFIG PATH NOT CONFIGURED !!!\r\n");
        printf("[X] PLEASE CONFIGURE THE PATH \r\n");
        fflush(stdout);
    }
    else
    {

        readSize = read(file_descriptor, logContent, 500);
        printf("[X] LOGFILE @Path: %s: \n%s", logfilePath, logContent);
        fflush(stdout);
        free(logContent);
        close(file_descriptor);
    }
}

void UI_SND_MSG(void){

    printf("---------------------------------------------\n");
    printf("----- SEND MESSAGE TO GROUP------------------\n");
    printf("---------------------------------------------\n");
    printf("[X] Please enter your message:\n");

    // char buffer[BYTES_PAYLOAD+1];
    // buffer[0]='\0';
    uint8_t inputLength = 0;
    char* ptrToBuffer = &userInputBuffer[0];
    fflush(stdin);
    clearStdinNewlines();
    fgets(userInputBuffer, BYTES_PAYLOAD+1, stdin);
    printf("User Input: %s", userInputBuffer);
    inputLength = strlen(ptrToBuffer);
    printf("InputLegnth: %d\n", inputLength);
    pthread_mutex_lock(&mutexInput);
    //memcpy(myInputData.userMsg, ptrToBuffer, inputLength+1);
    myInputData.userMsg = ptrToBuffer;
    printf("User data after copy: %s\n", myInputData.userMsg);
    myInputData.msgLength = inputLength;
    myInputData.newMsgReceived = true;
    #ifdef DEBUG
    printf("Your input message: %s with length: %i and new: %i\n",myInputData.userMsg,myInputData.msgLength,myInputData.newMsgReceived);
    #endif
    pthread_mutex_unlock(&mutexInput);

}

void UI_ERR_INJ(void){
    clrscr();
    char input[4];
    uint16_t inputInt;
    printf("Which bit should be flipped (0-255)?\n");
    
    clearStdinNewlines();
    fgets(input, 3, stdin);
    inputInt = atoi(input);
    if(inputInt > 255)
    {
        printf("Invalid bit index!\n");
    }
    else
    {
        pthread_mutex_lock(&mutexBitError);
        userErrorBit = inputInt;
        errorInjectionCalled = true;
        pthread_mutex_unlock(&mutexBitError);
    }


}

void UI_MAIN(void)
{
    
    clrscr();
    //UI_CONF_CONFIG();
    strcpy(configfilePath,"../config.txt");
    clrscr();
    //UI_LOG();
    strcpy(logfilePath,"../log.txt");
    clrscr();
    UI_GROUPID();


     while (1)
    {

    clrscr();

    switchNumber = (int)result;
    switch (switchNumber)
    {
     case 1:
        UI_SND_MSG();
        break; 
    case 2:
        UI_LOG_READ();
        break;
    case 3:
        UI_ERR_INJ();
        break;
    case 4:
        UI_PEER_INFO();
        break;
    default:
        break;
    }

    GUI_SELECTION();

    do
    {
        //clearStdinNewlines();
        fgets(s, 2, stdin);

        if (IS_INT_STRING(s))
        {
            correct = true;
            result = strtol(s, &end, 10);
            break;
        }
        else
        {
            printf("[X] Enter your CMD (1 <= n <= 4): #");
        }
    } while (!correct);
    
     }
}

void clearStdinNewlines(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}