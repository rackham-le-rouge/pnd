/**
 * Program:     pnd
 * File:        signalmanagement.h
 * Brief:       Declare functions of signalmanagement.c and import other functions used in this source code
 * Description: Cf brief
 * Date:        1/2014
 * Licence:     Beerware (rev 42)
 * Author:      Jerome GRARD - A bored engineering student...
 */



#ifndef SIGNALMANAGEMENT_H
#define SIGNALMANAGEMENT_H


/*

        Header of the signal handling part of the program

*/


void initialisationOfTheSignal(void);
void signalHandler(int p_iSignalNo);
void commonSignalEnding(void) __attribute__ ((noreturn));
void saveCurrentContext(char p_cMode, structProgramInfo* p_structCommon);

#endif
