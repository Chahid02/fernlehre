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


/*=========================================================*/
/*==== MULTITHREAD DEFINES  ===============================*/
/*=========================================================*/
#define NUM_THREADS 2
#define NUM_UI_TREAD 0




/*=========================================================*/
/*==== PROTOTYPE DECLARATION ==============================*/
/*=========================================================*/

void *UI_INTERFACE(void *threadID);

#endif