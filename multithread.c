/**
 * Program:     pnd
 * File:        multithread.c
 * Brief:       Start / Stop / Manage all threads of this program
 * Description: Here it is all of the stuff to have many threads. We choose to have a thread per number.
 *              Here, we start our thread and we stop them
 * Date:        Somewhere in 2013 and near of 2014
 * Licence:     Beerware (rev 42)
 * Author:      Jerome GRARD - A bored engineering student...
 */


#include "conf.h"



void createAllComputingThreads(structProgramInfo* p_structCommon)
{
	int l_iThreadNumber = p_structCommon->iThreadNumber;
	int l_iCurrentThread;


	p_structCommon->bIsComputing = TRUE;
	p_structCommon->bDead = FALSE;


	/*
	 ****************************************
	 *
	 *	     PARALLEL SECTION
	 *
	 ****************************************
	 */

	#pragma omp parallel private(l_iCurrentThread), shared(p_structCommon), num_threads(l_iThreadNumber)
	{
		#ifdef DEBUG
		/*char l_cBufferNumber[200];*/
		#endif

		char l_bResultOfPrimeFunction = FALSE;
		l_iCurrentThread = omp_get_thread_num();

		mpz_t l_mpzPrimeNumberToTest;
		mpz_init(l_mpzPrimeNumberToTest);

		/* Create the Mersenne number 2^n - 1 */
		mpz_ui_pow_ui(l_mpzPrimeNumberToTest, 2, p_structCommon->iMersenneOrder);
		mpz_sub_ui(l_mpzPrimeNumberToTest, l_mpzPrimeNumberToTest, 1);

 		#pragma omp critical (writeLogSection) 			/* just a name for the section */
		{
			LOG_WRITE_STRING_LONG("Multithread starting. Thread number is :", (unsigned long)l_iCurrentThread);


			#ifdef UNDEFINED
			mpz_get_str(l_cBufferNumber, 10, l_mpzPrimeNumberToTest);
			LOG_WRITE("Tested number is : ")
			LOG_WRITE_STRING(l_cBufferNumber)
			#endif
		}

		/* Using a special function in order to work in multithread. All calculation are splitted in l_iThreadNumber parts, and
		 * the current part is l_iCurrentThread */
		l_bResultOfPrimeFunction = isItAPrimeNumberMultiThread(l_mpzPrimeNumberToTest, l_iCurrentThread, l_iThreadNumber, p_structCommon);

 		#pragma omp critical (computeSection) 			/* just a name for the section */
		{
			if(l_bResultOfPrimeFunction == FALSE)
			{
				/* We found at least one divider */
				p_structCommon->bDead = TRUE;
				LOG_WRITE_STRING_LONG("Compute: no it is not ! For the section number", (unsigned long)(l_iCurrentThread))
			}
			else
			{
				LOG_WRITE_STRING_LONG("Compute: yes it seem to be ! for the section number", (unsigned long)(l_iCurrentThread))
			}
		}
		// Clear all
		///mpz_clear(l_mpzPrimeNumberToTest);
	}

	/*
	 ****************************************
	 *
	 *	    / PARALLEL SECTION
	 *
	 ****************************************
	 */

	p_structCommon->bIsComputing = FALSE;

	if(p_structCommon->bDead == TRUE)
	{
		LOG_WRITE("This is not a prime number !")
		drawSubMenu(p_structCommon->iRow, p_structCommon->iCol, MENU_THIS_IS_NOT_A_PRIME_NUMBER);
	}
	else
	{
		LOG_WRITE("This is a prime number.")
		drawSubMenu(p_structCommon->iRow, p_structCommon->iCol, MENU_THIS_IS_A_PRIME_NUMBER);
	}

	/* Ask user key press */
	nodelay(stdscr, FALSE);
	getch();
	nodelay(stdscr, TRUE);
}
