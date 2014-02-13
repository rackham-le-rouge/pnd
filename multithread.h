/**
 * Program:     pnd
 * File:        multithread.h
 * Brief:       Declare functions of multithread.c and import other functions used in this source code
 * Description: Cf brief
 * Date:        Somewhere in 2013 and near of 2014
 * Licence:     Beerware (rev 42)
 * Author:      Jerome GRARD - A bored engineering student...
 */



#ifndef MULTITHREAD_H
#define MULTITHREAD_H


/*

        Header of the multithread part

*/


void createAllComputingThreads(structProgramInfo* p_structCommon);
extern int isItAPrimeNumberMultiThread(mpz_t p_mpzNumber, int p_iSectionNumber, int p_iTotalSection, structProgramInfo* p_structStructure);
extern void drawSubMenu(int p_iRow, int p_iCol, int p_iMenuSelector);

#endif
