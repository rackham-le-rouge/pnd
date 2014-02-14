/**
 * Program:     pnd
 * File:        main.h
 * Brief:       Call all functions of the rest of the program -> usage of extern
 * Description: Cf main.c
 * Date:        Somewhere in 2013 and near of 2014
 * Licence:     Beerware (rev 42)
 * Author:      Jerome GRARD - A bored engineering student...
 */



#ifndef MAIN_H
#define MAIN_H


/*

	Header of the main program

*/

void setDefaultValueToTheProgramStructure(structProgramInfo* p_structStructure);

extern void initBar();
extern void topText(const char*);
extern void botText(const char*);
extern int drawElement(int, int, char, int);
extern void initColor();
extern void drawIntro(int, int);
extern void disableBar();
extern void drawMainMenu(int p_iRow, int p_iCol);
extern int isItAPrimeNumber(mpz_t p_mpzNumber);
extern int millerRabin(mpz_t p_mpzA, mpz_t p_mpzN);
extern void createAllComputingThreads(structProgramInfo* p_structCommon);
extern void initialisationOfTheSignal(void);
extern void eraseWorkingScreen(int p_iRow, int p_iCol);
extern void drawCurrentMersenneOrder(structProgramInfo* structCommon);

/* From std lib */
FILE *popen(const char *commande, const char *type);
int pclose(FILE *stream);

#endif
