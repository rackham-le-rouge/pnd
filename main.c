/**
 * Program:     Prime Number Discovery
 * File:        main.c
 * Brief:       Main part of the program
 * Description: There is the main function (start the screen and display the splash)
 *              and after run the program
 * Date:        1/2014
 * Licence:     Beerware (rev 42)
 * Author:      Jerome GRARD - A bored engineering student...
 */


/**
 *   ERRATUM
 *
 *i.e : l_cCaractereEnCours   == l is for local (g : global and p : parameter) // c is for char (i : integer, f : float etc...)
 *                              CaractereEnCours == Caractere En Cours ==> Current Letter (dictionnary is your friend)
 */

#define PRIME_TEST 8191


#include "conf.h"


char rev[] = "0.0";
char ver[] = "1.0";


int g_iLigne	=		0;
int g_iColonne  =		0;











/**
  * This function set default value of the main structure of the program.
  * In this structure there is all of important informations and
  * shared by threads and the commandeur function
  */
void setDefaultValueToTheProgramStructure(structProgramInfo* p_structStructure)
{
	LOG_WRITE("Writing the default configuration in the common stuctProgramInfo")
	p_structStructure->iMersenneOrder = 15;   		// Today the max is 17425170;
	p_structStructure->iThreadNumber = 1;
	p_structStructure->bIsComputing = FALSE;
	p_structStructure->bNeedToRedrawProgressBar = FALSE;
}




// Main
int main(int argc, char** argv)
{
	int l_iTmp = 0;
	int l_bAsk = TRUE;
	structProgramInfo* structCommon = NULL;

	LOG_WRITE("------------------------------------------------")
	LOG_WRITE("Starting new instance...")

	// Start the graphic mode
	initscr();

	// Hide the cursor
	curs_set(0);

	char l_cBuffer[250];
	char l_cBuffer2[250];
	noecho();
	cbreak();
	sprintf(l_cBuffer, "PND - Ver %s - Rev %s - Dev by 8m2", ver, rev);

	// Initialisation of some graphical elements
	LOG_WRITE("Screen element initialisation...")
	initColor();
        getmaxyx(stdscr,g_iLigne,g_iColonne);
	drawLoadingBar(0, -1, 0, g_iColonne, 0);		// To initialize progressBar function
	initBar();

	// Check the screen size
	if(g_iLigne < MIN_SCREEN_HEIGHT)
	{
		LOG_WRITE("Screen doesn't have enought Lines")
		mvprintw(1,1,"Screen too small");
		refresh();
		getch();
		endwin();
		return ENOMSG;
	}
	if(g_iColonne < MIN_SCREEN_LENGHT)
	{
		LOG_WRITE("Screen doesn't have enought Columns")
		printf("Screen too small");
		refresh();
		getch();
		endwin();
		return ENOMSG;
	}

	// Intro drawing
	if(argc < MIN_ARGC)
	{
		//drawIntro(g_iLigne, g_iColonne);
		initBar();
	}

	// Don't ask Enter key in order to complete a getch()
	nodelay(stdscr, 1);


	// Right message on the bottom bar
	for(l_iTmp=0; l_iTmp < g_iColonne - strlen(l_cBuffer) ; l_iTmp++)
	{
		l_cBuffer2[l_iTmp] = ' ';
		l_cBuffer2[l_iTmp+1] = '\0';
	}
	strcat(l_cBuffer2, l_cBuffer);
	botText(l_cBuffer2);


	// Setting default values for the most important state variable of the program
	structCommon = (structProgramInfo*)malloc(1*sizeof(structProgramInfo));
	setDefaultValueToTheProgramStructure(structCommon);

	// Just drawing, no command here
	drawMainMenu(g_iLigne, g_iColonne);


	createAllComputingThreads(structCommon);


	// Do what the user request
	LOG_WRITE("Main menu : Wainting for a user choice...")
	do
	{
		// Get the keyboark key
		l_iTmp = getch();
		l_iTmp -= 48;

		l_bAsk = (l_iTmp > 0 && l_iTmp < 6) ? FALSE : TRUE;
	}while(l_bAsk);


	switch(l_iTmp)
	{
		case 1:
		{
			LOG_WRITE("Start/Stop function selected")
			break;
		}
		case 2:
		{
			LOG_WRITE("Set a new order function selected")
			break;
		}
		case 3:
		{
			LOG_WRITE("Minitoring the screen function selected")
			break;
		}
		case 4:
		{
			LOG_WRITE("XX function selected")
			break;
		}
		case 5:
		{
			LOG_WRITE("XX function selected")
			break;
		}
		case 6:
		{
			LOG_WRITE("XX function selected")
			break;
		}
		default:
		{
			LOG_WRITE("Error in the menu choice. No corresponding function")
			break;
		}

	}


#ifdef	DEBUG
	// Ask Enter key in order to complete a getch()
	//LOG_WRITE("")
	nodelay(stdscr, 0);
	refresh();
//	getch();
#endif

	// Stop the program and leave the graphic mode ! Very important !
	LOG_WRITE("End of the program. See you");
	endwin();


	// Clean
	free(structCommon);


	return 0;
}
