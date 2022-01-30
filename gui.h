/*!
**************************************************************
* @file    gui.h
* @brief   GUI HEADER file
* @author  GRUPPE *
* @version V1.0
* @date    2022-01-27
* @copyright Copyright (c) FH CAMPUS ESE22. All rights reserved.
**************************************************************
*/

#ifndef GUI_H_
#define GUI_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/uio.h>
#include <fcntl.h>
/*---------------------------------------------------------*/
/*---- MULTITHREAD DEFINES  -------------------------------*/
/*---------------------------------------------------------*/
#define NUM_THREADS 2
#define NUM_UI_TREAD 0

#define LOG(X, Y) fprintf (fp, #X ": Time:%s, File:%s(%d) " #Y  "\n", __TIMESTAMP__, __FILE__, __LINE__)

/*---------------------------------------------------------*/
/*---- TERMINAL PRINTCLEAR --------------------------------*/
/*---------------------------------------------------------*/
#define clrscr() printf("\e[1;1H\e[2J") // Clear screen.

/*---------------------------------------------------------*/
/*---- GUI FILES  -----------------------------------------*/
/*---------------------------------------------------------*/
#define PEER_NR "#5"
#define PEER_MAX_DEC 5
#define consumeBuffer() while (getchar() != '\n');


#define configFile "config.txt"
#define logFile "log.txt"
#define configPath "../config.txt"
#define logPath "../log.txt"



//groupmember mygroup[groupsize];

extern char logFilename[100];
extern char configFilename[100];
//extern int LogCreateFlag;


/*---------------------------------------------------------*/
/*---- PROTOTYPE DECLARATION  -----------------------------*/
/*---------------------------------------------------------*/

void UI_START(void);
void UI_MAIN(void);
void GUI_SELECTION(void);
void UI_LOG(void);
void UI_CONF_CONFIG(void);
void UI_READ_CONFIG(void);
void UI_PEER_INFO(void);
void *UI_INTERFACE(void *threadID);
void clear_buffer();
void timeStampFunc(void);
void UI_LOG_READ(void);
void UI_GROUPID(void);
extern void UI_MW_MEMBER(void);
#endif
