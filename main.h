/**
 * Program:     pnd
 * File:        main.h
 * Brief:       Call all functions of the rest of the program
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
void killTheApp(structProgramInfo* p_structCommon);
int daemonizeMe(structProgramInfo* p_structCommon);

/* From std lib */
FILE *popen(const char *commande, const char *type);
int pclose(FILE *stream);

#endif
