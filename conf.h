/*******************************************************

		Program configuration
		Set parameters here and
		recompile in order to apply

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

// Some includes
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <gmp.h>
#include <omp.h>
#include <signals.h>


// Some constants
#define	DEBUG			1
#define TRACE_EXEC		1

#define MIN_SCREEN_HEIGHT	10
#define	MIN_SCREEN_LENGHT	40
#define MIN_ARGC		3

// logic stuff
#define TRUE			1
#define FALSE			0


// Colors of lines (Bottom and Top)
#define COLOR_LINE_BG_BOTTOM  	COLOR_RED
#define COLOR_LINE_BG_TOP 	COLOR_BLUE
#define COLOR_LINE_FG_BOTTOM	COLOR_BLACK
#define COLOR_LINE_FG_TOP	COLOR_BLACK


// Details about progress bar
#define PROGRESS_BAR_BODY_CHARACTER	'-'
#define PROGRESS_BAR_HEAD_CHARACTER	'>'


// Colors of element in a single matrix
#define COLOR_ELEMENT_FG	COLOR_BLACK
#define GRAPHIC_MODE		0		// 1 background is colored 0 background id black and letter is colored

typedef enum
{
	enumNoir = 	3,
	enumRouge =	4,
	enumVert =	5,
	enumJaune =	6,
	// 7 is reserved don't use it !
	enumBleu = 	8,
	enumMagenta =	9,
	enumCyan =	10,
	enumBlanc =	11
}g_enumJeuxDeCouleursDispo;

#define MACRO_LENGHT_OF_BUFFER_FOR_INT	500

#define	DEBUG_T1	{mvprintw(1, 1, "/%d/", 314);refresh();getch();}
#define	DEBUG_T2	{mvprintw(1, 1, "/%d/", 235);refresh();getch();}

char __macroTemporaryBuffer[MACRO_LENGHT_OF_BUFFER_FOR_INT];

#define LOG_WRITE(macro_sString)	if(TRACE_EXEC) {system("echo " macro_sString " >> pnd.log");}
#define LOG_WRITE_STRING(macro_sString)	if(TRACE_EXEC) {snprintf(__macroTemporaryBuffer, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo %s >> pnd.log", macro_sString); system(__macroTemporaryBuffer);}
#define LOG_WRITE_LONG(macro_lLong)	if(TRACE_EXEC) {snprintf(__macroTemporaryBuffer, MACRO_LENGHT_OF_BUFFER_FOR_INT*sizeof(char), "echo %ld >> pnd.log", macro_lLong); system(__macroTemporaryBuffer);}


typedef struct structProgramInfo_
{
	int iMersenneOrder;
	int iThreadNumber;
	int bIsComputing;
	int bNeedToRedrawProgressBar;
}structProgramInfo;





// Other includes, they need to be at the end in order to have the struct just above
#include "compute.h"
#include "main.h"
#include "drawing.h"
#include "multithread.h"





#endif
