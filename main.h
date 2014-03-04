/**
 * @file        main.h
 * @program     pnd
 * @brief       Call all functions of the rest of the program
 * @description All main functions of the program like deamon, exit point, entry point, default value settings and user menu
 * @date        Somewhere in 2013 and near of 2014
 * @licence     Beerware (rev 42)
 * @author      Jerome GRARD - A bored engineering student...
 */



#ifndef MAIN_H
#define MAIN_H


/*

	Header of the main program

*/

void setDefaultValueToTheProgramStructure(structProgramInfo* p_structStructure);
void killTheApp(structProgramInfo* p_structCommon);
int daemonizeMe(structProgramInfo* p_structCommon);
void extractConfigFromCommandLine(int argc, char** argv, structProgramInfo* p_structCommon, char* p_bAutoAction, int* p_iAutoActionChoice);
char mainMenu(structProgramInfo* p_structCommon, char* p_bAutoAction, int* p_iAutoActionChoice);

/* From std lib */
FILE *popen(const char *commande, const char *type);
int pclose(FILE *stream);

#endif
