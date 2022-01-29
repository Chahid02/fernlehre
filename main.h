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
/*---- MULTITHREAD DEFINES  -------------------------------*/
/*---------------------------------------------------------*/
#define NUM_THREADS 2
#define NUM_UI_TREAD 0

/*---------------------------------------------------------*/
/*---- STRUCT DEFINES--------------------------------------*/
/*---------------------------------------------------------*/



/*---------------------------------------------------------*/
/*---- PROTOTYPE DECLARATION  -----------------------------*/
/*---------------------------------------------------------*/

void *UI_INTERFACE(void *threadID);

<<<<<<< HEAD
void testChecksum();
void testStoreFrame();
=======

>>>>>>> 2f46bff493eafa5a3eaa1dd9232f5c6aa4654f0d
#endif