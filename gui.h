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

typedef enum {false, true} bool;

#define configFile "config.txt"
#define logFile "log.txt"
#define configPath "../config.txt"
#define logPath "../log.txt"

/*---------------------------------------------------------*/
/*---- PROTOTYPE DECLARATION  -----------------------------*/
/*---------------------------------------------------------*/

void UI_START(void);
void UI_MAIN(void);
void GUI_SELECTION(void);
void UI_LOG(void);
void *UI_INTERFACE(void *threadID);
bool isIntString(char* input ) ;
void clear_buffer();

#endif
