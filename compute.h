/**
 * @program     fley
 * @file        compute.h
 * @brief       All computing functions of the program
 * @description Computation functions, the brain of the program
 * @date        1/2014
 * @licence     Beerware (rev 42)
 * @author      Jerome GRARD - A bored engineering student...
 */



#ifndef COMPUTE_H
#define COMPUTE_H


/*

	Header of the compute functions

*/

char isItAPrimeNumberMPZ(mpz_t p_mpzNumber);
char isItAPrimeNumberULI(double p_dNumber);
/*int millerRabin(mpz_t p_mpzA, mpz_t p_mpzN);*/
int isItAPrimeNumberMRMultiThread(mpz_t p_mpzNumber, int p_iSectionNumber, int p_iTotalSection, structProgramInfo* p_structStructure);
int isItAPrimeNumberMultiThread(mpz_t p_mpzNumber, int p_iSectionNumber, int p_iTotalSection, structProgramInfo* p_structStructure);
void storeAndCleanMPZNumber(mpz_t*  p_mpzNumber, char p_iAction);

/* Copied from math.c */
double sqrt  (double x);
double fmod  (double numer     , double denom);

#endif
