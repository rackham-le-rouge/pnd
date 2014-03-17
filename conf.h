
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
#include <sys/stat.h>

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
#define MENU_WAIT_CHECK_MERSENNE_ORDER	7
#define MENU_INFO_STD_ALGO_SET		8
#define MENU_INFO_MR_ALGO_SET		9
#define MENU_MERSENNE_ORDER		10

#define MODE_SAVE			0
#define MODE_INIT			1
#define MODE_LOAD			2
#define MODE_TOOGLE			3
#define MODE_CLEAN			4
#define MODE_ADD			5

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
#define GRAPHIC_MODE		0			/* 1 background is colored 0 background id black and letter is colored */


/* Kind of algo used */
#define ALGO_MILLER_RABIN               1
#define ALGO_STD                        2


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

#define DEFAULT_MR_WANTED_CHECK	20000		/* Default number of check for a Mersenne number by the miller rabin algo. Greater is this value, best is the security of the test */
#define DEFAULT_MERSENNE_ORDER	31
#define DEFAULT_MODERATION_TIME	50		/* If there is no moderation time defined by the command line, the default moderation
						time is applied if user want to toogle the ModerationTime function with SIGUSR1 calling */
#define USEC_BETWEEN_KEY_CHECK	500000		/* be carefull !!! if you put a lower value, there is a risk of weird character spawning 
						on the screen */
#define USEC_BETWEEN_AUTO_SEARCH 500000

/* Debug part - Lots of usefull macro */
#define CPOINT		if(TRACE_EXEC) {char __macroTemporaryBufferCP[50]; 				\
		snprintf(__macroTemporaryBufferCP, 50*sizeof(char), "echo %d [%s] %d  >> pnd.log",g_iCurrentPID, __FILE__, __LINE__); \
		system(__macroTemporaryBufferCP);							\
		} /*"*/

#define POPEN_BUFFER_LENGHT		10
#define MACRO_LENGHT_OF_BUFFER_FOR_INT	150

#define LOG_WRITE(macro_sString)						if(TRACE_EXEC) {	\
		char __macroTemporaryBufferA[MACRO_LENGHT_OF_BUFFER_FOR_INT]; 				\
		snprintf(__macroTemporaryBufferA, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo %d [%s] %d \t\t %s >> pnd.log", \
		g_iCurrentPID, __FILE__, __LINE__,  (macro_sString)); system(__macroTemporaryBufferA);			\
		}		/*" I add this to save the syntax coloration under nano - shame on me */
#define LOG_WRITE_STRING(macro_sString)						if(TRACE_EXEC) {	\
		char __macroTemporaryBufferB[MACRO_LENGHT_OF_BUFFER_FOR_INT]; 				\
		snprintf(__macroTemporaryBufferB, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo %d [%s] %d \t\t %s >> pnd.log",  \
		g_iCurrentPID, __FILE__, __LINE__,  (macro_sString)); system(__macroTemporaryBufferB);				\
		}		/*" I add this to save the syntax coloration under nano - shame on me */
#define LOG_WRITE_STRING_MPZ(macro_sString, macro_mpz)				if(TRACE_EXEC) {	\
		char __macroTemporaryBufferC[MACRO_LENGHT_OF_BUFFER_FOR_INT];				\
		char macro_mpzNumber[MACRO_LENGHT_OF_BUFFER_FOR_INT]; mpz_get_str((macro_mpzNumber), 10, (macro_mpz)); 	\
		snprintf(__macroTemporaryBufferC, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo %d [%s] %d \t\t %s %s >> pnd.log",	\
		 g_iCurrentPID, __FILE__, __LINE__, (macro_sString), (macro_mpzNumber)); system(__macroTemporaryBufferC);	\
		}		/*" I add this to save the syntax coloration under nano - shame on me */
#define LOG_WRITE_LONG(macro_lLong)						if(TRACE_EXEC) {	\
		char __macroTemporaryBufferD[MACRO_LENGHT_OF_BUFFER_FOR_INT]; 				\
		snprintf(__macroTemporaryBufferD, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo %d [%s] %d \t\t %ld >> pnd.log", 	\
		g_iCurrentPID, __FILE__, __LINE__, (macro_lLong)); system(__macroTemporaryBufferD);			\
		}		/*" I add this to save the syntax coloration under nano - shame on me */
#define LOG_WRITE_STRING_LONG(macro_sString, macro_lLong)			if(TRACE_EXEC) {	\
		char __macroTemporaryBufferE[MACRO_LENGHT_OF_BUFFER_FOR_INT]; 				\
		snprintf(__macroTemporaryBufferE, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo %d [%s] %d \t\t %s  %ld >> pnd.log", 	\
		g_iCurrentPID, __FILE__, __LINE__, (macro_sString), (macro_lLong)); system(__macroTemporaryBufferE);	\
		}		/*" I add this to save the syntax coloration under nano - shame on me */
#define LOG_WRITE_LONG_LONG(macro_lLong1, macro_lLong2)				if(TRACE_EXEC) {	\
		char __macroTemporaryBufferF[MACRO_LENGHT_OF_BUFFER_FOR_INT]; 				\
		snprintf(__macroTemporaryBufferF, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo %d [%s] %d \t\t %ld  %ld >> pnd.log", \
		g_iCurrentPID, __FILE__, __LINE__, (macro_lLong1), (macro_lLong2)); system(__macroTemporaryBufferF);	\
		}		/*" I add this to save the syntax coloration under nano - shame on me */
#define LOG_WRITE_STRING_LONG_LONG(macro_sString, macro_lLong1, macro_lLong2)	if(TRACE_EXEC) {	\
		char __macroTemporaryBufferG[MACRO_LENGHT_OF_BUFFER_FOR_INT]; 				\
		snprintf(__macroTemporaryBufferG, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo %d [%s] %d \t\t %s *%ld* *%ld* >> pnd.log",	\
		g_iCurrentPID,  __FILE__, __LINE__, (macro_sString), (macro_lLong1), (macro_lLong2)); system(__macroTemporaryBufferG);	\
		}		/*" I add this to save the syntax coloration under nano - shame on me */



/**
  * @struct structProgramInfo_
  * @var structProgramInfo_::iMersenneOrder
  * Member 'iMersenneOrder' contains current mersenne order to compute
  * @var structProgramInfo_::iCol
  * Member 'iCol' contains number of colonne in the screen
  * @var structProgramInfo_::iRow
  * Member 'iRow' contains number of row in the screen
  * @var structProgramInfo_::iThreadProgressionTable
  * Member 'iThreadProgressionTable' contains a table with all threads progression. This table have as size the number of row because it's the maximum thread displayable
  * @var structProgramInfo_::iModerationTime
  * Member 'iModerationTime' contains time between two calculs. In order to slow down the program
  * @var structProgramInfo_::iThreadNumber
  * Member 'iThreadNumber' contains number of trhreads working
  * @var structProgramInfo_::bIsComputing
  * Member 'bIsComputing' contains a bool to say if we are computing or not
  * @var structProgramInfo_::bNeedToRedrawProgressBar
  * Member 'bNeedToRedrawProgressBar' contains a flag to redraw progressbar. Maybee useless by the way...
  * @var structProgramInfo_::bDead
  * Member 'bDead' contains a flag to say if at least one thread have found a divider
  * @var structProgramInfo_::bAutoSearch
  * Member 'bAutoSearch' contains a flag to say if user wants an auto search or not
  * @var structProgramInfo_::bLoaded
  * Member 'bLoaded' contains a flag to say if current config is loaded from a hotsave file or not
  * @var structProgramInfo_::iWantedMRCheck
  * Member 'iWantedMRCheck' contains number of wanted check per tested number and per thread in case of Miller Rabin algo
  * @var structProgramInfo_::bFastDisp
  * Member 'bFastDisp' is a flag setting up the fast display of message in autosearch mode. Setted by command line. FALSE by default
  * @var structProgramInfo_::iUsedAlgo
  * Member 'iUsedAlgo' contains value from -128 to +127 in order to store the kind of algo used during computation. There is
  * two avaible values : ALGO_MILLER_RABIN and ALGO_STD. And you can switch in the main menu by using the 7 touch function.
  */
typedef struct structProgramInfo_
{
        int iMersenneOrder;
        int  iCol;
        int  iRow;
        int* iThreadProgressionTable;
        int  iModerationTime;                                           /* Wait a little between each test in order to slow down and avoid CPU overloadinf */
        int  iWantedMRCheck;
        unsigned char iThreadNumber;                                    /* we can put it in char, more than 256 thread is suspicious... */
        char bIsComputing;
        char bNeedToRedrawProgressBar;
        char bDead;                                                     /* when at least one thread found at least one divider */
        char bAutoSearch;
        char bLoaded;
        char iUsedAlgo;
	char bFastDisp;
	char bFastMode;
	char bPrintTime;
}structProgramInfo;


/**
  * @struct structLinkedList_
  * @var structLinkedList_::structSuivant
  * Member 'structSuivant' contains the adress of the next object in the linked list
  * @var structLinkedList_::mpzNumber
  * Member 'mpzNumber' contains the adress of the number to clean with mpz_clean()
  */
typedef struct structLinkedList_
{
	struct structLinkedList_*	structSuivant;
	mpz_t*				mpzNumber;

}structLinkedList;

/* To save the current PID, in order to have understandable LOG Files */
extern pid_t	g_iCurrentPID;



/* Other includes, they need to be at the end in order to have the struct just above */
#include "main.h"
#include "compute.h"
#include "drawing.h"
#include "multithread.h"
#include "signalmanagement.h"




#endif
