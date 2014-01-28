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


	#pragma omp parallel private(l_iCurrentThread) num_threads(l_iThreadNumber)
	{
		l_iCurrentThread = omp_get_thread_num();

		mpz_t l_mpzPrimeNumberToTest;
		mpz_init(l_mpzPrimeNumberToTest);

		/* Create the Mersenne number 2^n - 1 */
		mpz_ui_pow_ui(l_mpzPrimeNumberToTest, 2, p_structCommon->iMersenneOrder);
		mpz_sub_ui(l_mpzPrimeNumberToTest, l_mpzPrimeNumberToTest, 1);

 		#pragma omp critical (writeLogSection) 			/* just a name for the section */
		{
			LOG_WRITE("Multithread starting. Number is :")
			LOG_WRITE_LONG((long int)l_iCurrentThread)
		}

		/* Using a special function in order to work in multithread. All calculation are splitted in l_iThreadNumber parts, and
		 * the current part is l_iCurrentThread */
		if(isItAPrimeNumberMultiThread(l_mpzPrimeNumberToTest, l_iCurrentThread, l_iThreadNumber) == FALSE)
		{
			/* We found at least one divider */
			p_structCommon->bDead = TRUE;
		}
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
		WRITE_LOG("This is a prime number ! :)")
	}
	else
	{
		WRITE_LOG("This is not a prime number :(")
	}
}
