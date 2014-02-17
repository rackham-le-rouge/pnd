/*******************************************************

		Program configuration
		Set parameters here and
		recompile in order to apply
		Need a 'make clean' !!!

*******************************************************/



/*   Avaiable colors 	*

        COLOR_BLACK   0
        COLOR_RED     1
        COLOR_GREEN   2
        COLOR_YELLOW  3
        COLOR_BLUE    4
        COLOR_MAGENTA 5
        COLOR_CYAN    6
        COLOR_WHITE   7
**			*/




#ifndef CONFIG_H
#define CONFIG_H

/* Some includes */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <omp.h>
#include <gmp.h>
#include <signal.h>
#include <limits.h>
#include <sys/types.h>

/* Some constants */
#define	DEBUG			1
/*#define DEBUG_VERBOSE		1*/		/* if you want to have all debugs messages */
#define TRACE_EXEC		1

#define MIN_SCREEN_HEIGHT	10
#define	MIN_SCREEN_LENGHT	40
#define MIN_ARGC		3


/* logic stuff */
#define TRUE			1
#define FALSE			0
#define DONT_KNOW		2

/* Kind of menu */
#define MENU_NEW_UNDEFINED		0
#define MENU_NEW_ORDER			1
#define MENU_THIS_IS_A_PRIME_NUMBER	2
#define MENU_THIS_IS_NOT_A_PRIME_NUMBER	3
#define MENU_SET_THREAD_NUMBER		4
#define MENU_ABOUT			5
#define MENU_GIVE_UP_SEARCH		6

/* Colors of lines (Bottom and Top) */
#define COLOR_LINE_BG_BOTTOM  	COLOR_RED
#define COLOR_LINE_BG_TOP 	COLOR_BLUE
#define COLOR_LINE_FG_BOTTOM	COLOR_BLACK
#define COLOR_LINE_FG_TOP	COLOR_BLACK


/* Details about progress bar */
#define PROGRESS_BAR_BODY_CHARACTER	'-'
#define PROGRESS_BAR_HEAD_CHARACTER	'>'
#define PROGRESS_BAR_COLOR		enumCyan	/* Avaible colors at the end of this file - Refer the enum block */


/* Colors of element in a single matrix */
#define COLOR_ELEMENT_FG	COLOR_BLACK
#define GRAPHIC_MODE		0		/* 1 background is colored 0 background id black and letter is colored */

typedef enum
{
	enumNoir = 	3,
	enumRouge =	4,
	enumVert =	5,
	enumJaune =	6,
	/* 7 is reserved don't use it ! */
	enumBleu = 	8,
	enumMagenta =	9,
	enumCyan =	10,
	enumBlanc =	11
}g_enumJeuxDeCouleursDispo;

#define DEFAULT_MERSENNE_ORDER	31
#define USEC_BETWEEN_KEY_CHECK	500000			/* be carefull !!! if you put a lower value, there is a risk of weird character spawning on the screen */
#define USEC_BETWEEN_AUTO_SEARCH 2000000


/* Debug part - Lots of usefull macro */
#define	DEBUG_T1	{mvprintw(1, 1, "/%d/", 314);refresh();getch();}
#define	DEBUG_T2	{mvprintw(1, 1, "/%d/", 235);refresh();getch();}
#define CPOINT		if(TRACE_EXEC) {char __macroTemporaryBufferCP[50]; snprintf(__macroTemporaryBufferCP, 50*sizeof(char), "echo [%s] %d  >> pnd.log", __FILE__, __LINE__); system(__macroTemporaryBufferCP);}

#define POPEN_BUFFER_LENGHT		10
#define MACRO_LENGHT_OF_BUFFER_FOR_INT	150

#define LOG_WRITE(macro_sString)						if(TRACE_EXEC) {char __macroTemporaryBufferA[MACRO_LENGHT_OF_BUFFER_FOR_INT]; snprintf(__macroTemporaryBufferA, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo [%s] %d \t\t %s >> pnd.log", __FILE__, __LINE__,  macro_sString); system(__macroTemporaryBufferA);		/*system("echo " __FILE__ " " macro_sString " >> pnd.log");*/}
#define LOG_WRITE_STRING(macro_sString)						if(TRACE_EXEC) {char __macroTemporaryBufferB[MACRO_LENGHT_OF_BUFFER_FOR_INT]; snprintf(__macroTemporaryBufferB, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo [%s] %d \t\t %s >> pnd.log", __FILE__, __LINE__,  macro_sString); system(__macroTemporaryBufferB);}
#define LOG_WRITE_STRING_MPZ(macro_sString, macro_mpz)				if(TRACE_EXEC) {char __macroTemporaryBufferC[MACRO_LENGHT_OF_BUFFER_FOR_INT]; char macro_mpzNumber[MACRO_LENGHT_OF_BUFFER_FOR_INT]; mpz_get_str(macro_mpzNumber, 10, macro_mpz); snprintf(__macroTemporaryBufferC, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo [%s] %d \t\t %s %s >> pnd.log", __FILE__, __LINE__, macro_sString, macro_mpzNumber); system(__macroTemporaryBufferC);}
#define LOG_WRITE_LONG(macro_lLong)						if(TRACE_EXEC) {char __macroTemporaryBufferD[MACRO_LENGHT_OF_BUFFER_FOR_INT]; snprintf(__macroTemporaryBufferD, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo [%s] %d \t\t %ld >> pnd.log", __FILE__, __LINE__, macro_lLong); system(__macroTemporaryBufferD);}
#define LOG_WRITE_STRING_LONG(macro_sString, macro_lLong)			if(TRACE_EXEC) {char __macroTemporaryBufferE[MACRO_LENGHT_OF_BUFFER_FOR_INT]; snprintf(__macroTemporaryBufferE, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo [%s] %d \t\t %s  %ld >> pnd.log", __FILE__, __LINE__, macro_sString, macro_lLong); system(__macroTemporaryBufferE);}
#define LOG_WRITE_LONG_LONG(macro_lLong1, macro_lLong2)				if(TRACE_EXEC) {char __macroTemporaryBufferF[MACRO_LENGHT_OF_BUFFER_FOR_INT]; snprintf(__macroTemporaryBufferF, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo [%s] %d \t\t %ld  %ld >> pnd.log", __FILE__, __LINE__, macro_lLong1, macro_lLong2); system(__macroTemporaryBufferF);}
#define LOG_WRITE_STRING_LONG_LONG(macro_sString, macro_lLong1, macro_lLong2)	if(TRACE_EXEC) {char __macroTemporaryBufferG[MACRO_LENGHT_OF_BUFFER_FOR_INT]; snprintf(__macroTemporaryBufferG, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo [%s] %d \t\t %s *%ld* *%ld* >> pnd.log", __FILE__, __LINE__, macro_sString, macro_lLong1, macro_lLong2); system(__macroTemporaryBufferG);}


typedef struct structProgramInfo_
{
	int iMersenneOrder;
	unsigned char iThreadNumber;					/* we can put it in char, more than 256 thread is suspicious... */
	char bIsComputing;
	char bNeedToRedrawProgressBar;
	char bDead;							/* when at least one thread found at least one divider */
	int  iCol;
	int  iRow;
	char bAutoSearch;
}structProgramInfo;





/* Other includes, they need to be at the end in order to have the struct just above */
#include "compute.h"
#include "main.h"
#include "drawing.h"
#include "multithread.h"
#include "signalmanagement.h"




#endif
