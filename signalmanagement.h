/**
 * @file        signalmanagement.h
 * @program     pnd
 * @brief       Declare functions of signalmanagement.c and import other functions used in this source code
 * @description In order to catch and act just after signal sending. So, we can have the time to save parameter just before kill
 * @date        1/2014
 * @licence     Beerware (rev 42)
 * @author      Jerome GRARD - A bored engineering student...
 */



#ifndef SIGNALMANAGEMENT_H
#define SIGNALMANAGEMENT_H


/*

        Header of the signal handling part of the program

*/


void initialisationOfTheSignal(void);
void signalHandler(int p_iSignalNo);
void commonSignalEnding(void);
void saveCurrentContext(char p_cMode, structProgramInfo* p_structCommon);
void toogleProgramSpeed(char p_cMode, structProgramInfo* p_structCommon);


/* Copied from stdlib */
int kill( pid_t pid, int sig );

#endif
