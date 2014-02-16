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


unsigned int g_iLigne	=		0;
unsigned int g_iColonne  =		0;











/**
  * This function set default value of the main structure of the program.
  * In this structure there is all of important informations and
  * shared by threads and the commandeur function
  */
void setDefaultValueToTheProgramStructure(structProgramInfo* p_structStructure)
{
	FILE* l_fileReturnOfCommand;
	char l_cBuffer[POPEN_BUFFER_LENGHT];

	LOG_WRITE("Writing the default configuration in the common stuctProgramInfo")
	p_structStructure->iMersenneOrder = DEFAULT_MERSENNE_ORDER;   		/* Today the max is 17425170; */
	p_structStructure->bIsComputing = FALSE;
	p_structStructure->bNeedToRedrawProgressBar = FALSE;
	p_structStructure->bDead = FALSE;
	p_structStructure->iRow = g_iLigne;
	p_structStructure->iCol = g_iColonne;
	/* bAutoSearch is not initialized here. Init is in main, just after argv analysing */

	/* Try to find how many cores the computer have. */
	l_fileReturnOfCommand = popen("cat /proc/cpuinfo | grep processor | wc -l", "r");	/* POSIX function ;) */

	if(l_fileReturnOfCommand == NULL)
	{
		p_structStructure->iThreadNumber = 2;						/* We assume that today all computers have at least two cores */
		LOG_WRITE_STRING_LONG("Number of thread detected --default-- : ", (long)p_structStructure->iThreadNumber);
	}
	else
	{
		if(fgets(l_cBuffer, (POPEN_BUFFER_LENGHT - 1)*sizeof(char), l_fileReturnOfCommand) == NULL)
		{
			p_structStructure->iThreadNumber = 2;						/* We assume that today all computers have at least two cores */
			LOG_WRITE_STRING_LONG("Number of thread detected --unsuccessfully-- : ", (long)p_structStructure->iThreadNumber);
		}
		else
		{
			p_structStructure->iThreadNumber = atoi(l_cBuffer);
			LOG_WRITE_STRING_LONG("Number of thread detected --successfully-- : ", (long)p_structStructure->iThreadNumber);
		}
		pclose(l_fileReturnOfCommand);
	}
}




/* Main */
int main(int argc, char** argv)
{
	char l_cBuffer[250];
	char l_cBuffer2[250];

	int l_iTmp;
	char l_bAsk;
	char l_bQuitProgram;
	structProgramInfo* structCommon;
	long int l_iUserValue;

	l_iTmp = 0;
	l_bAsk = TRUE;
	l_bQuitProgram = FALSE;
	structCommon = NULL;
	l_iUserValue = 0;

	LOG_WRITE(" ")
	LOG_WRITE(" ")
	LOG_WRITE("---------------------------------------------------------------------")
	LOG_WRITE("                    Starting new instance")
	LOG_WRITE("---------------------------------------------------------------------")

	/* Start the graphic mode */
	initscr();

	/* Hide the cursor */
	curs_set(0);

	noecho();
	cbreak();
	sprintf(l_cBuffer, "PND - Ver %s - Rev %s - Dev by Géo", ver, rev);

	/* Initialisation of some graphical elements */
	LOG_WRITE("Screen element initialisation...")
	initColor();
        getmaxyx(stdscr,g_iLigne,g_iColonne);
	drawLoadingBar(0, -1, 0, g_iColonne, 0);		/* To initialize progressBar function */
	initBar();

	/* Check the screen size */
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

	/* Intro drawing */
	if(argc < MIN_ARGC)
	{
		/*drawIntro(g_iLigne, g_iColonne); */
		initBar();
	}

	/* Find if user want an autosearch */
	if(argc > 1)
	{
		if(!strcmp(argv[1], "-a"))
		{
			structCommon->bAutoSearch = TRUE;
		}
	}

	/* Don't ask Enter key in order to complete a getch() */
	nodelay(stdscr, TRUE);


	/* Right message on the bottom bar -- We need to signed them all because there is a substraction. But it's useless because l_cBuffer is too small */
	for(l_iTmp=0; (signed)l_iTmp < (signed)g_iColonne - (signed)strlen(l_cBuffer) ; l_iTmp++)
	{
		l_cBuffer2[l_iTmp] = ' ';
		l_cBuffer2[l_iTmp+1] = '\0';
	}
	strcat(l_cBuffer2, l_cBuffer);
	botText(l_cBuffer2);


	/* Setting default values for the most important state variable of the program */
	structCommon = (structProgramInfo*)malloc(1*sizeof(structProgramInfo));
	setDefaultValueToTheProgramStructure(structCommon);

	/* Re-routing signals of the system */
	initialisationOfTheSignal();

	/* Print current mersenne order at the screen bottom */
	drawCurrentMersenneOrder(structCommon);



	while(!l_bQuitProgram)
	{
		/* Do what the user request */
		LOG_WRITE("Main menu : Wainting for a user choice...")

		/* Just erase screen and drawing menu, no command here */
		eraseWorkingScreen(g_iLigne, g_iColonne);
		drawMainMenu(g_iLigne, g_iColonne);

		/* Reactivate delay for getch calling -- in order to avoid the killing-cpu-process loop */
		nodelay(stdscr, FALSE);
		do
		{
			/* Get the keyboark key */
			l_iTmp = getch();
			l_iTmp -= 48;

			l_bAsk = (l_iTmp > 0 && l_iTmp < 7) ? FALSE : TRUE;
		}while(l_bAsk);
		nodelay(stdscr, TRUE);


		switch(l_iTmp)
		{
			case 1:
			{
				LOG_WRITE("Start function selected")
				eraseWorkingScreen(g_iLigne, g_iColonne);
				createAllComputingThreads(structCommon);
				break;
			}
			case 2:
			{
				LOG_WRITE("Set a new order function selected")
				drawSubMenu(g_iLigne, g_iColonne, MENU_NEW_ORDER, structCommon);
				if(scanf("%li", &l_iUserValue) == EOF)
				{
					/* Fail -- Keep the old value*/
					LOG_WRITE_STRING_LONG("New Mersenne order --failed-- Keep the old value : ", (long)structCommon->iMersenneOrder);
				}
				else
				{
					/* Sucess typing */
					structCommon->iMersenneOrder = l_iUserValue;
					LOG_WRITE_STRING_LONG("New Mersenne order changed to : ", (long)structCommon->iMersenneOrder);
				}
				drawCurrentMersenneOrder(structCommon);
				break;
			}
			case 3:
			{
				LOG_WRITE("Set new thread number function selected")
				drawSubMenu(g_iLigne, g_iColonne, MENU_SET_THREAD_NUMBER, structCommon);
				if(scanf("%li", &l_iUserValue) == EOF)
				{
					/* Fail -- Keep the old value*/
					LOG_WRITE_STRING_LONG("New thread number --failed-- Keep the old value : ", (long)structCommon->iThreadNumber);
				}
				else
				{
					/* Sucess typing */
					if(l_iUserValue > g_iLigne - 2)
					{
						/* We don't have enought lines to display threads progression */
						LOG_WRITE_STRING_LONG("New thread number --failed-- Dont have enought lines. New value : ", (long)(g_iLigne - 2));
						structCommon->iThreadNumber = (unsigned char)(g_iLigne - 2);
					}
					else
					{
						/* We have enought lines to display all threads progression */
						structCommon->iThreadNumber = (unsigned char)l_iUserValue;
						LOG_WRITE_STRING_LONG("New thread number changed to : ", (long)structCommon->iThreadNumber);
					}
				}
				break;
			}
			case 4:
			{
				LOG_WRITE("Prospecting mode selected")
				structCommon->bAutoSearch = TRUE;

				while(structCommon->bAutoSearch == TRUE)
				{
					eraseWorkingScreen(g_iLigne, g_iColonne);
					createAllComputingThreads(structCommon);

					/* Jump to the new mersenne number */
					structCommon->iMersenneOrder++;

					LOG_WRITE_STRING_LONG("New Mersenne order changed to : ", (long)structCommon->iMersenneOrder);
					drawCurrentMersenneOrder(structCommon);
				}
				break;
			}
			case 5:
			{
				LOG_WRITE("About menu selected")
				eraseWorkingScreen(g_iLigne, g_iColonne);
				drawSubMenu(g_iLigne, g_iColonne, MENU_ABOUT, structCommon);
				break;
			}
			case 6:
			{
				LOG_WRITE("Quit the program")
				l_bQuitProgram = TRUE;
				break;
			}
			default:
			{
				LOG_WRITE("Error in the menu choice. No corresponding function")
				break;
			}
		}
	}


	/* Show the cursor */
	curs_set(TRUE);

	/* Stop the program and leave the graphic mode ! Very important ! */
	LOG_WRITE("End of the program. See you");
	endwin();

	/* Clean */
	free(structCommon);

	return 0;
}
