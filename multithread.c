/**
 * @file        multithread.c
 * @program     pnd
 * @brief       Start / Stop / Manage all threads of this program
 * @description Here it is all of the stuff to have many threads. We choose to have a thread per number.
 *              Here, we start our thread and we stop them
 * @date        Somewhere in 2013 and near of 2014
 * @licence     Beerware (rev 42)
 * @author      Jerome GRARD - A bored engineering student...
 */


#include "conf.h"




/**
  * @brief This function call openMP and create all threads, and share the work. At the end, this function gather all datas and
  *	   says if this is a prime number
  * @param p_structCommon : all parameters of the program, number of threads to start
  */
void createAllComputingThreads(structProgramInfo* p_structCommon)
{
	int l_iThreadNumber;				/** number of threads working in the program */
	int l_iCurrentThread;				/** current thread */
	unsigned long int l_iSeconds;			/** seconds needed to check one mersenne number */
	unsigned int l_iUSecBetweenTwoAutoSearch;	/** for the waiting function, in order to free the CPU during a long computation */
	char l_bQKeyPressed;				/** Used by keyboard handler, to check and do things */
	time_t __attribute__((unused))l_timeBegin;	/** May be not used -- Begin of the computation */
	time_t __attribute__((unused))l_timeEnd;	/** May be not used -- End of the computation */

	l_iThreadNumber = p_structCommon->iThreadNumber;
	p_structCommon->bIsComputing = TRUE;
	p_structCommon->bDead = FALSE;
	l_iUSecBetweenTwoAutoSearch = USEC_BETWEEN_AUTO_SEARCH;
	l_bQKeyPressed = FALSE;
	l_iSeconds = 0;

	if(p_structCommon->bPrintTime == TRUE)
	{
		time(&l_timeBegin);				/* Get current time */
	}

	/* there is iRow lines, and iRow+1 integers in the table, thus the last one is [iRow]. And we choose that it is the
	   place of the ThreadNumber - This line is copy/pasted in main, in order to init this variable for the -i command line
	   option calling - But we called it here each time because, if user have changed the number of active thread, we need to
	   put actualize this key value */
	p_structCommon->iThreadProgressionTable[p_structCommon->iRow] = l_iThreadNumber;

	/*
	 ****************************************
	 *
	 *	     PARALLEL SECTION
	 *
	 ****************************************
	 */

	/* We add one in order to keep a thread for the keyboard handler */
	#pragma omp parallel private(l_iCurrentThread), shared(p_structCommon, l_bQKeyPressed), num_threads(l_iThreadNumber + 1)
	{
		char l_bResultOfPrimeFunction;
		char l_bKeyAccepted;
		char l_cKeyPressed;
		unsigned int l_iUSecBetweenTwoKeyCheck;

		mpz_t l_mpzPrimeNumberToTest;
		mpz_init(l_mpzPrimeNumberToTest);

		l_iCurrentThread = omp_get_thread_num();
		l_bResultOfPrimeFunction = FALSE;
		l_bKeyAccepted = FALSE;
		l_cKeyPressed = 0;
		l_iUSecBetweenTwoKeyCheck = USEC_BETWEEN_KEY_CHECK;

		if(l_iCurrentThread == l_iThreadNumber)
		{
			/* Means we are in the last thread, thus there is the keyboard handler */

			do
			{
				usleep(l_iUSecBetweenTwoKeyCheck);
				l_cKeyPressed = getch();

				/* End this thread when all other threads are over */
				if(p_structCommon->bIsComputing == FALSE)
				{
					l_bKeyAccepted = TRUE;
				}

				switch(l_cKeyPressed)
				{
					case '+':
					{
						/* One more thread. Check if there is enought lines to print it */
						p_structCommon->iThreadNumber = (p_structCommon->iThreadNumber + 1 < p_structCommon->iRow - 2) ? p_structCommon->iThreadNumber + 1 : p_structCommon->iThreadNumber;
						LOG_WRITE_STRING_LONG("One thread added by user during computing. New thread number : ", (long)p_structCommon->iThreadNumber)
						break;
					}
					case '-':
					{
						/* One less thread. */
						p_structCommon->iThreadNumber = (p_structCommon->iThreadNumber - 1 > 0) ? p_structCommon->iThreadNumber - 1 : p_structCommon->iThreadNumber;
						LOG_WRITE_STRING_LONG("One thread removed by user during computing. New thread number : ", (long)p_structCommon->iThreadNumber)
						break;
					}
					case 'q':
					case 'Q':
					{
						p_structCommon->bDead = TRUE;
						p_structCommon->bAutoSearch = FALSE;		/* If user press Q key, there is no more
					autosearch  and we quit now. To reactivate it, he needs to choose the right option in the main menu */
						l_bKeyAccepted = TRUE;
						l_bQKeyPressed = TRUE;
						break;
					}
					default:
					{
						/* All non implemented yet keys */
						break;
					}
				}
			}while(l_bKeyAccepted == FALSE);
		}
		else
		{
			/* Create the Mersenne number 2^n - 1 */
			mpz_ui_pow_ui(l_mpzPrimeNumberToTest, 2, p_structCommon->iMersenneOrder);
			mpz_sub_ui(l_mpzPrimeNumberToTest, l_mpzPrimeNumberToTest, 1);

			#ifdef DEBUG_VERBOSE
	 		#pragma omp critical (writeLogSection) 			/* just a name for the section */
			{
				LOG_WRITE_STRING_LONG("Multithread starting. Thread number is :", (unsigned long)l_iCurrentThread);
			}
			#endif


			/******************************************
					COMPUTATION
			*******************************************/


			/* Using a special function in order to work in multithread. All calculation are splitted in l_iThreadNumber parts, and
			 * the current part is l_iCurrentThread */
			if(p_structCommon->iUsedAlgo == ALGO_MILLER_RABIN)
			{
				l_bResultOfPrimeFunction = isItAPrimeNumberMRMultiThread(l_mpzPrimeNumberToTest, l_iCurrentThread, l_iThreadNumber, p_structCommon);
			}
			else if(p_structCommon->iUsedAlgo == ALGO_STD)
			{
				l_bResultOfPrimeFunction = isItAPrimeNumberMultiThread(l_mpzPrimeNumberToTest, l_iCurrentThread, l_iThreadNumber, p_structCommon);
			}
			else
			{
				LOG_WRITE("/!\\ Warning : There is no algo selected !!")
			}





			/* Dead flag is raised by the finder of the divider, the other threads remains in DONT_KNOW state.
			   If there is a thread in FALSE, the other one are in DONT_KNOW, but if all threads are in DONT_KNOW state
			   it is because the Q key is pressed. Thus check l_bQKeyPressed */
			if(l_bResultOfPrimeFunction == FALSE)
			{
				p_structCommon->bDead = TRUE;
			}

			#ifdef DEBUG_VERBOSE
 			#pragma omp critical (computeSection) 			/* just a name for the section */
			{
				if(l_bResultOfPrimeFunction == FALSE)
				{
					/* We found at least one divider */
					LOG_WRITE_STRING_LONG("Compute: no it is not ! For the section number", (unsigned long)(l_iCurrentThread))
				}
				else if(l_bResultOfPrimeFunction == DONT_KNOW)
				{
					LOG_WRITE_STRING_LONG("Compute: do not know ! For the section number", (unsigned long)(l_iCurrentThread))
				}
				else
				{
					LOG_WRITE_STRING_LONG("Compute: yes it seem to be ! for the section number", (unsigned long)(l_iCurrentThread))
				}
			}
			#endif
		}
		mpz_clear(l_mpzPrimeNumberToTest);
		p_structCommon->bIsComputing = FALSE;
	}

	/*
	 ****************************************
	 *
	 *	    / PARALLEL SECTION
	 *
	 ****************************************
	 */

	if(p_structCommon->bPrintTime == TRUE)
	{
		time(&l_timeEnd);				/* Get end time used to find how long computing takes */
		l_iSeconds = difftime(l_timeEnd, l_timeBegin);
		LOG_WRITE_STRING_LONG("Computation takes --in seconds--: ", l_iSeconds)
	}

	if(p_structCommon->bDead == TRUE && l_bQKeyPressed == FALSE)
	{
		LOG_WRITE("NO")
		if(p_structCommon->bFastDisp == FALSE)
		{
			drawSubMenu(p_structCommon->iRow, p_structCommon->iCol, MENU_THIS_IS_NOT_A_PRIME_NUMBER, p_structCommon);
		}
	}
	else if(p_structCommon->bDead == TRUE && l_bQKeyPressed == TRUE)
	{
		LOG_WRITE("Give up ! Dont know it this is a prime number or not")
		drawSubMenu(p_structCommon->iRow, p_structCommon->iCol, MENU_GIVE_UP_SEARCH, p_structCommon);
	}
	else
	{
		LOG_WRITE("================== YES ==================")
		if(p_structCommon->bFastDisp == FALSE)
		{
			drawSubMenu(p_structCommon->iRow, p_structCommon->iCol, MENU_THIS_IS_A_PRIME_NUMBER, p_structCommon);
		}
	}

	/* Desactivate the loadedConf mode --when all progression are loaded from a hotsave file */
	p_structCommon->bLoaded = FALSE;

	/* Ask user key press and if we are in auto search, leave the message 2 seconds and quit */
	if(p_structCommon->bAutoSearch == TRUE)
	{
		if(p_structCommon->bFastDisp == FALSE)
		{
			usleep(l_iUSecBetweenTwoAutoSearch);
		}
		else
		{
			if(p_structCommon->bDead == FALSE)
			{
				usleep(l_iUSecBetweenTwoAutoSearch);
			}
		}
	}
	else if(p_structCommon->bAutoSearch == FALSE && p_structCommon->bFastDisp == TRUE)
	{
		usleep(l_iUSecBetweenTwoAutoSearch);
	}
	else
	{
		nodelay(stdscr, FALSE);
		getch();
		nodelay(stdscr, TRUE);
	}
}
