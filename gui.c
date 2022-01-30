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
#include "gui.h"

char logfilePath[100] = {0};
char configfilePath[100] = {0};
static char userInputBuffer[BYTES_PAYLOAD+1];

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
    printf("[X] Enter your CMD (1 <= n <= 4): #");
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

void UI_LOG_READ(void)
{
    printf("---------------------------------------------\n");
    printf("----- READ LOG FILE -------------------------\n");
    printf("---------------------------------------------\n");
    fflush(stdout);
    /*--------------------------------*/
    /*---- LOG FILE Descriptor ----*/
    int file_descriptor;
    int readSize;

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

    clearStdinNewlines();
    printf("\nHit enter to return to menu!\n");
    char enter = 0;
    while (enter != '\r' && enter != '\n')
    {
        enter = getchar();
    }

}

void UI_LOG(void)
{
    printf("---------------------------------------------\n");
    printf("----- CREATE LOG FILE -----------------------\n");
    printf("---------------------------------------------\n");
    printf("[X] Enter the Path of the LOG File:");

    scanf("%s", logfilePath);

    int file_descriptor = open(logfilePath, O_RDWR | O_CREAT, 0777);

    if (file_descriptor == -1)
    {
        perror("[X] LOG FILE NOT FOUND");
        exit(-1);
    }
    else
    {
        printf("[X] LOG FILE FOUND OR CREATED SUCCESSFULLY\r\n");
        close(file_descriptor);
    }
}

void UI_READ_CONFIG(void)
{
    printf("---------------------------------------------\n");
    printf("----- CONFIG FILE ---------------------------\n");
    printf("---------------------------------------------\n");
    fflush(stdout);
    printf("This is the Group:\n");

    for (int i = 0; i < groupsize; i++)
    {
        printf("ID: %i ,",mygroup[i].id);
        printf("IPv4: %s ,",mygroup[i].ipv4);
        printf("Port: %i",mygroup[i].port);
        if (i==myID)
        {
            printf("  <-- This is the selected Peer.");
        }
        printf("\n");
    }

    clearStdinNewlines();
    printf("\nHit enter to return to menu!\n");
    char enter = 0;
    while (enter != '\r' && enter != '\n')
    {
        enter = getchar();
    }
    
}

void UI_GROUPID(void)
{
    printf("---------------------------------------------\n");
    printf("----- GROUP ID SELECTION --------------------\n");
    printf("---------------------------------------------\n");
    printf("Please enter your ID! (0-%i)\n",groupsize-1);

    char buffer[4];

    clearStdinNewlines();
    fgets(buffer, 3, stdin);
    
    pthread_mutex_lock(&mymutex);
    myID = atoi(buffer);
    printf("\nYou selected: %i\n",myID);
    pthread_mutex_unlock(&mymutex);

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
    UI_CONF_CONFIG();
    //strcpy(configfilePath,"../config.txt");
    clrscr();
    UI_LOG();
    //strcpy(logfilePath,"../log.txt");
    clrscr();
    UI_GROUPID();


    int switchNumber;
    char s[100];
    char *end;
    bool correct = false;
    long result;

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
        UI_READ_CONFIG();
        break;
    default:
        break;
    }

    clrscr();

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