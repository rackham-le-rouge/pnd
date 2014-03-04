/**
 * @file        multithread.h
 * @program     pnd
 * @brief       Declare functions of multithread.c and import other functions used in this source code
 * @description Control the multithread part of the program, share the work between all threads, start them, wait them all and gather informations
 * @date        Somewhere in 2013 and near of 2014
 * @licence     Beerware (rev 42)
 * @author      Jerome GRARD - A bored engineering student...
 */



#ifndef MULTITHREAD_H
#define MULTITHREAD_H


/*

        Header of the multithread part

*/


void createAllComputingThreads(structProgramInfo* p_structCommon);

/* imported from std libs */
typedef	unsigned int	uint_t;		/** don't know why i need to put that here. I already included sys/types.h and all is in it. FIXME ! */
typedef uint_t		useconds_t;	/** Time, in microseconds */
int usleep(useconds_t usec);

#endif
