/**
 * Program:     fley
 * File:        compute.h
 * Brief:       All computing functions of the program
 * Description: Cf compute.c
 * Date:        1/2014
 * Licence:     Beerware (rev 42)
 * Author:      Jerome GRARD - A bored engineering student...
 */



#ifndef COMPUTE_H
#define COMPUTE_H


/*

	Header of the compute functions

*/

char isItAPrimeNumberMPZ(mpz_t p_mpzNumber);
char isItAPrimeNumberULI(double p_dNumber);
int millerRabin(mpz_t p_mpzA, mpz_t p_mpzN);
int isItAPrimeNumberMultiThread(mpz_t p_mpzNumber, int p_iSectionNumber, int p_iTotalSection, structProgramInfo* p_structStructure);
void storeAndCleanMPZNumber(mpz_t*  p_mpzNumber, char p_iAction);

/* Copied from math.c */
double sqrt  (double x);
double fmod  (double numer     , double denom);

#endif
