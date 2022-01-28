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
/*---- TERMINAL PRINTCLEAR --------------------------------*/
/*---------------------------------------------------------*/
#define clrscr() printf("\e[1;1H\e[2J") // Clear screen.

/*---------------------------------------------------------*/
/*---- GUI FILES  -----------------------------------------*/
/*---------------------------------------------------------*/
#define PEER_NR "#5"
#define PEER_MAX_DEC 5
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
#endif
