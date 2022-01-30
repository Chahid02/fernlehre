/*!
**************************************************************
* @file    main.h
* @brief   GUI HEADER file
* @author  GRUPPE *
* @version V1.0
* @date    2022-01-27
* @copyright Copyright (c) FH CAMPUS ESE22. All rights reserved.
**************************************************************
*/
#ifndef MAIN_H_
#define MAIN_H_


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

#include "gui.h"
#include "middleware.h"

/*---------------------------------------------------------*/
/*---- PROTOTYPE DECLARATION  -----------------------------*/
/*---------------------------------------------------------*/

void *UI_INTERFACE(void *threadID);
extern int middleware();

#endif