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


#include "conf.h"


char rev[] = "0.0";
char ver[] = "1.0";








/**
  * Attempt to create a daemon function, in order tpo put this program in daemon mode
  *        VOTE DARKSIDE
  *
  */
int daemonizeMe(structProgramInfo* p_structCommon)
{
	pid_t	l_pidPid;
	pid_t	l_pidSid;

	LOG_WRITE("Daemon : Start the processus... I am going to kill my father")

	/* Do the fork */
	l_pidPid = fork();
	if(l_pidPid < 0)
	{
		/* Fork error, giving up the daemonization */
		LOG_WRITE("Daemon : Fail to fork me. Giving up daemonization")
		return EXIT_FAILURE;
	}
	if(l_pidPid > 0)
	{
		/* Because we are in the parent program */
		killTheApp(p_structCommon);
	}

	LOG_WRITE("Daemon : Kill my father. Done")
	/* Change umask to forget the parent's one */
	umask(0);

	/* Change the process group for the same raison */
	l_pidSid = setsid();

	if(l_pidSid < 0)
	{
		LOG_WRITE("Daemon : SID impossible to change")
		killTheApp(p_structCommon);
	}

	/* Close all of the  standart output in order to complete the 'submarine mode' */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	LOG_WRITE("Daemon : All standart output closed. Submarine OK")

	return EXIT_SUCCESS;
}




/**
  * A function to terminate the program and clean this mess up
  *
  */
void killTheApp(structProgramInfo* p_structCommon)
{
	static structProgramInfo* l_structCommon = NULL;
	static char l_bAlreadyCalled = FALSE;

	switch(l_bAlreadyCalled)
	{
		case FALSE:
		{
			l_structCommon = p_structCommon;
			l_bAlreadyCalled = TRUE;
			break;
		}
		default:
		{
	        	/* Show the cursor */
        		curs_set(TRUE);

	       		/* Stop the program and leave the graphic mode ! Very important ! */
        		LOG_WRITE("End of the program. See you");
        		endwin();

        		/* Clean */
        		free(l_structCommon->iThreadProgressionTable);
        		free(l_structCommon);

			exit(EXIT_SUCCESS);
			break;
		}
	}
}





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
	/*p_structStructure->iRow = ;
	p_structStructure->iCol = ;						Initialized in main */
	p_structStructure->iModerationTime = 0;
	p_structStructure->bLoaded = DONT_KNOW;
	p_structStructure->iThreadProgressionTable = (int*)malloc((p_structStructure->iRow + 1)*sizeof(int));	/* +1 because there is a number to save how many threads works. p_structStructure->iRow is used because it is the max threads number, wa can't display more threads progression, thus, the limit is the lines number  */
	/* bAutoSearch is not initialized here. Init is in main, just after argv analysing */

	/* Init memory */
	memset(p_structStructure->iThreadProgressionTable, 0, (p_structStructure->iRow + 1)*sizeof(int));

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



/**
  * Extract all usefull function from command line, and configure the program to
  * apply wanted parameters
  */
void extractConfigFromCommandLine(int argc, char** argv, structProgramInfo* p_structCommon, char* p_bAutoAction, int* p_iAutoActionChoice)
{
	int l_iTmp;
	int l_iPid;				/* Supposed pid of the other pnd process, for the change speed case  */
	int l_iChangeSpeedOfThisPID;		/* We are sure that is this PID */
	FILE* l_fileReturnOfCommand;
	char l_cBuffer[POPEN_BUFFER_LENGHT];


	l_iPid = 0;
	l_iChangeSpeedOfThisPID = 0;

	if(argc > 1)
	{
		for(l_iTmp = 1; l_iTmp < argc ; l_iTmp++)
		{
			if(strlen(argv[l_iTmp]) > 1)
			{
				/* autosearch feature */
				p_structCommon->bAutoSearch = (!strcmp(argv[l_iTmp], "-a")) ? TRUE : p_structCommon->bAutoSearch;
				*p_bAutoAction = (!strcmp(argv[l_iTmp], "-a")) ? TRUE : *p_bAutoAction;
				*p_iAutoActionChoice = (!strcmp(argv[l_iTmp], "-a")) ? 4 : *p_iAutoActionChoice;
				if(!strcmp(argv[l_iTmp], "-a")) {LOG_WRITE("C.LINE : Prospecting mode selected")}

				/* Change mersenne order */
				p_structCommon->iMersenneOrder = (!strcmp(argv[l_iTmp], "-m")) ? atoi(argv[l_iTmp + 1]) : p_structCommon->iMersenneOrder;
				if(!strcmp(argv[l_iTmp], "-m")) {LOG_WRITE_STRING_LONG("C.LINE : Change Mersenne order to ", (long)p_structCommon->iMersenneOrder)}

				/* Change moderation time */
				p_structCommon->iModerationTime = (!strcmp(argv[l_iTmp], "-w")) ? atoi(argv[l_iTmp + 1]) : p_structCommon->iModerationTime;
				if(!strcmp(argv[l_iTmp], "-w")) {LOG_WRITE_STRING_LONG("C.LINE : Change moderation time to ", (long)p_structCommon->iModerationTime)}

				/* Change thread number */
				p_structCommon->iThreadNumber = (!strcmp(argv[l_iTmp], "-t")) ? atoi(argv[l_iTmp + 1]) : p_structCommon->iThreadNumber;
				if(!strcmp(argv[l_iTmp], "-t")) {LOG_WRITE_STRING_LONG("C.LINE : Change thread number to ", (long)p_structCommon->iThreadNumber)}

				/* Change speed of the program */
				if(!strcmp(argv[l_iTmp], "-s"))
				{
					LOG_WRITE("C.LINE : Change computing speed of the program ")
					l_fileReturnOfCommand = popen("ps -C syslogd -o pid=", "r");
					if(l_fileReturnOfCommand != NULL)
					{
						while(fgets(l_cBuffer, (POPEN_BUFFER_LENGHT - 1)*sizeof(char), l_fileReturnOfCommand) != NULL)
						{
							l_iPid = atoi(l_cBuffer);
							if(l_iPid != getpid())
							{
								LOG_WRITE_STRING_LONG("C.LINE : Good PID found and saved : ", (long)l_iPid)
								l_iChangeSpeedOfThisPID = l_iPid;
							}
							else
							{
								LOG_WRITE_STRING_LONG("C.LINE : Wrong PID found : ", (long)l_iPid)
							}
						}
						pclose(l_fileReturnOfCommand);
					}
					else
					{
						LOG_WRITE("C.LINE : Failed to load PID of the current pnd session")
					}

					if(l_iChangeSpeedOfThisPID != 0)
					{
						/* We have found one valid PID */
						LOG_WRITE_STRING_LONG("C.LINE : Slowing down pnd at PID : ", (long)l_iChangeSpeedOfThisPID)
						kill((pid_t)l_iChangeSpeedOfThisPID, SIGUSR1);
					}
					*p_bAutoAction = TRUE;
					*p_iAutoActionChoice = 6;
				}

				/* No windows displayed. submarine mode */
				p_structCommon->bAutoSearch = (!strcmp(argv[l_iTmp], "-d")) ? TRUE : p_structCommon->bAutoSearch;
				*p_bAutoAction = (!strcmp(argv[l_iTmp], "-d")) ? TRUE : *p_bAutoAction;
				*p_iAutoActionChoice = (!strcmp(argv[l_iTmp], "-d")) ? 4 : *p_iAutoActionChoice;
				if(!strcmp(argv[l_iTmp], "-d"))
				{
					LOG_WRITE("C.LINE : Daemon mode selected. Starting prospecting mode.");
					daemonizeMe(p_structCommon);
				}


				/* Display help */
				if(!strcmp(argv[l_iTmp], "-h"))
				{
					LOG_WRITE("C.LINE : Help is displayed")
					endwin();
					printf("PND - Command line use : pnd [-h{help}] [-a{auto}] [-d{daemon}] [-s{speed toogle}] [[-m] [wanted mersenne order]] [[-t] [wanted number of threads]] [[-w] [moderation time]]\n");
					*p_bAutoAction = TRUE;
					*p_iAutoActionChoice = 6;
				}
			}
			/* Else the parameter is ignored, use strcmp on it cause a segfault */
		}

		/* Check some values, in order to control user choices and put default values instead of if there is an error */
		if(p_structCommon->iThreadNumber > p_structCommon->iRow - 2)
		{
			p_structCommon->iThreadNumber = p_structCommon->iRow - 2;
		}
		if(isItAPrimeNumberULI((double)p_structCommon->iMersenneOrder) == FALSE)
		{
			p_structCommon->iMersenneOrder = DEFAULT_MERSENNE_ORDER;
		}
	}
}












/* Main */
int main(int argc, char** argv)
{
	char l_cBuffer[250];
	char l_cBuffer2[250];
	unsigned int l_iCol;
	unsigned int l_iRow;
	int l_iTmp;
	char l_bAutoAction;				/* Autoaction do the selected choice wrote in autoactionchoice variable instead of wainting for a user choice */
	int l_iAutoActionChoice;
	structProgramInfo* structCommon;

	l_iTmp = 0;
	l_iRow = 0;
	l_iCol = 0;
	l_bAutoAction = FALSE;
	l_iAutoActionChoice = -1;
	structCommon = NULL;

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
        getmaxyx(stdscr,l_iRow ,l_iCol);
	initBar();

	/* Check the screen size */
	if(l_iRow < MIN_SCREEN_HEIGHT)
	{
		LOG_WRITE("Screen doesn't have enought Lines")
		mvprintw(1,1,"Screen too small");
		refresh();
		getch();
		endwin();
		return ENOMSG;
	}
	if(l_iCol < MIN_SCREEN_LENGHT)
	{
		LOG_WRITE("Screen doesn't have enought Columns")
		printf("Screen too small");
		refresh();
		getch();
		endwin();
		return ENOMSG;
	}

	initBar();


	/* Don't ask Enter key in order to complete a getch() */
	nodelay(stdscr, TRUE);


	/* Right message on the bottom bar -- We need to signed them all because there is a substraction. But it's useless because l_cBuffer is too small */
	for(l_iTmp=0; (signed)l_iTmp < (signed)l_iCol - (signed)strlen(l_cBuffer) ; l_iTmp++)
	{
		l_cBuffer2[l_iTmp] = ' ';
		l_cBuffer2[l_iTmp+1] = '\0';
	}
	strcat(l_cBuffer2, l_cBuffer);
	botText(l_cBuffer2);


	/* Setting default values for the most important state variable of the program */
	structCommon = (structProgramInfo*)malloc(1*sizeof(structProgramInfo));

	/* Init screen size */
	structCommon->iRow = l_iRow;
	structCommon->iCol = l_iCol;

	/* We can use it only after setting the screen size */
	setDefaultValueToTheProgramStructure(structCommon);

	/* Configure program according to the command line */
	extractConfigFromCommandLine(argc, argv, structCommon, &l_bAutoAction, &l_iAutoActionChoice);

	/* Gives a copy of commonStruct adress to the last function executed if user kill this program. And to the toogle speed function.
	   Thus, saveCurrentContext function can save all parameters and resume computing later */
	saveCurrentContext(MODE_INIT, structCommon);

	/* And now, try to load the previous config, let here if program have been killed */
	saveCurrentContext(MODE_LOAD, structCommon);

	/* Then, we can init the toogleSpeed part of the program */
	toogleProgramSpeed(MODE_INIT, structCommon);

	/* Initialize the killTheApp function - It copy pointer adress in a static variable */
	killTheApp(structCommon);

	/* Initialize progressbar */
	drawLoadingBar(0, -1, 0, structCommon->iCol, 0);

	/* Re-routing signals of the system */
	initialisationOfTheSignal();

	/* Print current mersenne order at the screen bottom */
	drawCurrentMersenneOrder(structCommon);

	if(structCommon->bLoaded == TRUE)
	{
		/* There is a hot save file, and it was loaded -- We need to apply parameters and resume computing */
		l_bAutoAction = TRUE;
		l_iAutoActionChoice = 4;
	}

	mainMenu(structCommon, &l_bAutoAction, &l_iAutoActionChoice);

	killTheApp(NULL);
}










char mainMenu(structProgramInfo* p_structCommon, char* p_bAutoAction, int* p_iAutoActionChoice)
{
	char l_bQuitProgram;
	char l_bAsk;
	long int l_iUserValue;
	int l_iTmp;


	l_bAsk = TRUE;
	l_bQuitProgram = FALSE;
	l_iUserValue = 0;
	l_iTmp = 0;

	while(!l_bQuitProgram)
	{
		/* Do what the user request */
		LOG_WRITE("Main menu : Wainting for a user choice...")

		l_bAsk = TRUE;

		/* Just erase screen and drawing menu, no command here */
		eraseWorkingScreen(p_structCommon->iRow, p_structCommon->iCol);
		drawMainMenu(p_structCommon->iRow, p_structCommon->iCol);

		/* Reactivate delay for getch calling -- in order to avoid the killing-cpu-process loop */
		nodelay(stdscr, FALSE);
		if(*p_bAutoAction == TRUE) {l_bAsk = FALSE;}
		while(l_bAsk)
		{
			/* Get the keyboark key */
			l_iTmp = getch();
			l_iTmp -= 48;

			l_bAsk = (l_iTmp > 0 && l_iTmp < 7) ? FALSE : TRUE;
		}
		nodelay(stdscr, TRUE);

		if(*p_bAutoAction == TRUE) {l_iTmp = *p_iAutoActionChoice; *p_bAutoAction = FALSE;}


		switch(l_iTmp)
		{
			case 1:
			{
				LOG_WRITE("Start function selected")
				p_structCommon->bAutoSearch = FALSE;
				eraseWorkingScreen(p_structCommon->iRow, p_structCommon->iCol);
				createAllComputingThreads(p_structCommon);
				break;
			}
			case 2:
			{
				LOG_WRITE("Set a new order function selected")
				drawSubMenu(p_structCommon->iRow, p_structCommon->iCol, MENU_NEW_ORDER, p_structCommon);
				if(scanf("%li", &l_iUserValue) == EOF)
				{
					/* Fail -- Keep the old value*/
					LOG_WRITE_STRING_LONG("New Mersenne order --failed-- Keep the old value : ", (long)p_structCommon->iMersenneOrder);
				}
				else
				{
					/* Sucess typing */
					drawSubMenu(p_structCommon->iRow, p_structCommon->iCol, MENU_WAIT_CHECK_MERSENNE_ORDER, p_structCommon);
					if(isItAPrimeNumberULI((double)l_iUserValue) == TRUE)
					{
						/* And order is a prime number, thus it is allowed */
						p_structCommon->iMersenneOrder = l_iUserValue;
						LOG_WRITE_STRING_LONG("New Mersenne order changed to : ", (long)p_structCommon->iMersenneOrder);
					}
					else
					{
						/* if order is not a prime number it not allowed. It is useless to waste time with it */
						LOG_WRITE_STRING_LONG("New Mersenne order --failed-- Keep the old value : ", (long)p_structCommon->iMersenneOrder);
					}
				}
				drawCurrentMersenneOrder(p_structCommon);
				break;
			}
			case 3:
			{
				LOG_WRITE("Set new thread number function selected")
				drawSubMenu(p_structCommon->iRow, p_structCommon->iCol, MENU_SET_THREAD_NUMBER, p_structCommon);
				if(scanf("%li", &l_iUserValue) == EOF)
				{
					/* Fail -- Keep the old value*/
					LOG_WRITE_STRING_LONG("New thread number --failed-- Keep the old value : ", (long)p_structCommon->iThreadNumber);
				}
				else
				{
					/* Sucess typing */
					if(l_iUserValue > p_structCommon->iRow - 2)
					{
						/* We don't have enought lines to display threads progression */
						LOG_WRITE_STRING_LONG("New thread number --failed-- Dont have enought lines. New value : ", (long)(p_structCommon->iRow - 2));
						p_structCommon->iThreadNumber = (unsigned char)(p_structCommon->iRow - 2);
					}
					else
					{
						/* We have enought lines to display all threads progression */
						p_structCommon->iThreadNumber = (unsigned char)l_iUserValue;
						LOG_WRITE_STRING_LONG("New thread number changed to : ", (long)p_structCommon->iThreadNumber);
					}
				}
				break;
			}
			case 4:
			{
				LOG_WRITE("Prospecting mode selected")
				p_structCommon->bAutoSearch = TRUE;

				while(p_structCommon->bAutoSearch == TRUE)
				{
					eraseWorkingScreen(p_structCommon->iRow, p_structCommon->iCol);
					createAllComputingThreads(p_structCommon);

					/* We are now going to find the new Mersenne order. It needs to be prime, thus, we need to check. For really great number this computation can take some time. Thus, we display a message */
					drawSubMenu(p_structCommon->iRow, p_structCommon->iCol, MENU_WAIT_CHECK_MERSENNE_ORDER, p_structCommon);

					/* Jump to the new mersenne number -- This new order needs to be prime in order to have a chance to give a prime mersenne numnber */
					do
					{
						p_structCommon->iMersenneOrder += 2;		/* jump to the next odd number */
					}while(isItAPrimeNumberULI((double)p_structCommon->iMersenneOrder) == FALSE);

					LOG_WRITE_STRING_LONG("New Mersenne order changed to : ", (long)p_structCommon->iMersenneOrder);
					drawCurrentMersenneOrder(p_structCommon);
				}
				break;
			}
			case 5:
			{
				LOG_WRITE("About menu selected")
				eraseWorkingScreen(p_structCommon->iRow, p_structCommon->iCol);
				drawSubMenu(p_structCommon->iRow, p_structCommon->iCol, MENU_ABOUT, p_structCommon);
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
	return EXIT_SUCCESS;
}
