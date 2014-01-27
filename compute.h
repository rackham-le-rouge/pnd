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

int isItAPrimeNumber(mpz_t p_mpzNumber);
int millerRabin(mpz_t p_mpzA, mpz_t p_mpzN, int p_iScreenLineNumber);


#endif
