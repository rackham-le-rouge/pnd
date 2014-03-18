/*
  *
  *	Set of usefull functions of GMP
  *
  * int mpz_divisible_p (const mpz_t n, const mpz_t d)			return non zero value if YES
  * mpz_congruent_p (const mpz_t n, const mpz_t c, const mpz_t d)
  * void mpz_sqrt (mpz_t rop, const mpz_t op)
  * int mpz_cmp (const mpz_t op1, const mpz_t op2)			Compare op1 and op2. Return a positive value if op1 > op2,
									zero if op1 = op2, or a negative value if op1 < op2.
  * Macro: int mpz_odd_p (const mpz_t op)
  *
  */


#include "conf.h"


/**
  * @brief
  * In the miller-rabin algorithm we only can use prime number. These numbers are not really long, thus there
  * is no need to improve this algorithm. We just need to know if the number is a prime number,
  * and after we can do 2^N - 1 and start the big computing
  *
  * Simple prime number testing -> check all number since sqrt(n) is reached
  * @param p_mpzNumber : number to test
  * @return TRUE (if the number is prime) or FALSE
  */
char isItAPrimeNumberMPZ(mpz_t p_mpzNumber)
{
	int l_bReturnOfFunction = FALSE;			/** returned number of the function */
	mpz_t l_mpzSQRT;					/** sqrt of tested number because we test all numbers from sqrt to the number itself */
	mpz_t l_mpzIterator;					/** to check all numbers */

	mpz_init(l_mpzSQRT);
	mpz_init(l_mpzIterator);

	#ifdef DEBUG_VERBOSE
	LOG_WRITE("Compute: Try to find if a simple number is prime number or not")
	#endif

	/* Copy number in iterator. We are going to modify Iterator in order to  try to be a diviser of Number. */
	mpz_set(l_mpzIterator, p_mpzNumber);

	/* Do the SQRT */
	mpz_sqrt (l_mpzSQRT, p_mpzNumber);

	/* Check if this an odd number */
	l_bReturnOfFunction = mpz_divisible_ui_p(p_mpzNumber, (unsigned long int)2);
	if(l_bReturnOfFunction)
	{
		mpz_clear(l_mpzSQRT);
		mpz_clear(l_mpzIterator);
		return FALSE;
	}	/* else, we continue... */


	/* Start to divide by all people between the number his own sqrt */
	for(;;)
	{
		mpz_sub_ui(l_mpzIterator,l_mpzIterator, 2);		/* p_mpzIterator - 1  -> we use _ui because 1 is not a mpz number,
									   and a cast isn't possible */
									/* we substract one and after anothyer one because it's useless
									to check the even number. an even number cannot divide an odd number. */
		l_bReturnOfFunction = mpz_divisible_p(p_mpzNumber, l_mpzIterator);

		if(l_bReturnOfFunction)	{break;}			/* else, we continue... */

		l_bReturnOfFunction = mpz_cmp(l_mpzSQRT, l_mpzIterator); /* Compare SQRT and Iterator. Return a positive value if
									    SQRT > Iterator */
		if(l_bReturnOfFunction > 0)
		{
			mpz_clear(l_mpzSQRT);
			mpz_clear(l_mpzIterator);
			return TRUE;
		}
	}
	mpz_clear(l_mpzSQRT);
	mpz_clear(l_mpzIterator);

	return FALSE;
}





/**
  * @brief
  * And the unsigned long int version of the previous function.
  * In order to test mersenne order we don't need GMP because order will remains under MAX of unsigned long int
  * This function is deprecated because it use double type.
  * @param p_dNumber : number to test in double format
  * @return TRUE (if the number is prime) or FALSE
  */
char isItAPrimeNumberULI(double  p_dNumber)
{
	double l_dReturnOfFunction;
	double l_dSQRT;
	double l_dIterator;

	l_dReturnOfFunction = FALSE;

	#ifdef DEBUG_VERBOSE
	LOG_WRITE("Compute: Try to find if a simple number is prime number or not")
	#endif

	/* Copy number in iterator. We are going to modify Iterator in order to  try to be a diviser of Number. */
	l_dIterator = p_dNumber;

	/* Do the SQRT */
	l_dSQRT = sqrt(p_dNumber);

	/* Check if this is an odd number */
	l_dReturnOfFunction = fmod(p_dNumber, (double)2);
	if((int)l_dReturnOfFunction == 0)
	{
		return FALSE;
	}

	/* Start to divide by all people between the number his own sqrt */
	for(;;)
	{
		l_dIterator -= 2;

		l_dReturnOfFunction = fmod(p_dNumber, l_dIterator);

		if((int)l_dReturnOfFunction == 0) {break;}			/* else, we continue... */

		/* Compare SQRT and Iterator. Return a positive value if SQRT > Iterator */
		if(l_dIterator < l_dSQRT)
		{
			return TRUE;
		}
	}
	return FALSE;
}






/**
  * @brief An other basic function in order to find if it is a prime number.
  *
  * This one is design to be used with multithread calls. We are able to
  * split all computation in some many parts in order to execute each one
  * in their own thread.
  *
  * But it is still the same poor algo than the function isItAPrimeNumber
  * and thus, there is any optimisation here...
  * @param p_mpzNumber : number to test in GMP format
  * @param p_iSectionNumber : we have N threads, thus N sections. Numbers between p_mpzNumber and sqrt(p_mpzNumber)
  *			      are divided in N section. This parameter select the section to compute. One function
  *			      like this one per thread is executed. And there is N threads
  * @param p_iTotalSection : number of sections, thus, number of computing threads
  * @param p_structStructure : all interesting data of the program
  * @return TRUE or FALSE
  */


int isItAPrimeNumberMultiThread(mpz_t p_mpzNumber, int p_iSectionNumber, int p_iTotalSection, structProgramInfo* p_structStructure)
{
	int l_bReturnOfFunction = FALSE;
	mpz_t l_mpzEndOfSearchArea;
	mpz_t l_mpzBeginOfSearchArea;
	mpz_t l_mpzSQRT;
	mpz_t l_mpzIterator;
	mpz_t l_mpzArea;
	mpz_t l_mpzCurrentPosition;
	mpz_t l_mpzPercent;
	mpz_t l_mpzOnePercent;
	mpz_t l_mpzTmp;
	mpz_t l_mpzRefreshCounter;

	mpz_init(l_mpzBeginOfSearchArea);
	mpz_init(l_mpzEndOfSearchArea);
	mpz_init(l_mpzSQRT);
	mpz_init(l_mpzIterator);
	mpz_init(l_mpzArea);
	mpz_init(l_mpzCurrentPosition);
	mpz_init(l_mpzPercent);
	mpz_init(l_mpzOnePercent);
	mpz_init(l_mpzTmp);
	mpz_init(l_mpzRefreshCounter);

	#ifdef DEBUG_VERBOSE
	LOG_WRITE_STRING_LONG("Compute: Try to find if a simple number is prime for thread ", (long int)p_iSectionNumber)
	#endif

	/* Init of RefreshCounter in order to display the 0% */
	mpz_set_ui(l_mpzRefreshCounter, (long)-1);

	/* Do the SQRT */
	mpz_sqrt (l_mpzSQRT, p_mpzNumber);

	/* Copy number in end and beginning variables in order to create the iterator boundaries. */
	mpz_set(l_mpzEndOfSearchArea, p_mpzNumber);
	mpz_set(l_mpzBeginOfSearchArea, p_mpzNumber);

	mpz_sub(l_mpzEndOfSearchArea, l_mpzEndOfSearchArea, l_mpzSQRT);
	mpz_sub(l_mpzBeginOfSearchArea, l_mpzBeginOfSearchArea, l_mpzSQRT);

	mpz_mul_ui(l_mpzEndOfSearchArea, l_mpzEndOfSearchArea, p_iSectionNumber + 1);
	mpz_mul_ui(l_mpzBeginOfSearchArea, l_mpzBeginOfSearchArea, p_iSectionNumber);

	mpz_cdiv_q_ui(l_mpzEndOfSearchArea, l_mpzEndOfSearchArea, p_iTotalSection);
	mpz_cdiv_q_ui(l_mpzBeginOfSearchArea, l_mpzBeginOfSearchArea, p_iTotalSection);

	mpz_add(l_mpzEndOfSearchArea, l_mpzEndOfSearchArea, l_mpzSQRT);
	mpz_add(l_mpzBeginOfSearchArea, l_mpzBeginOfSearchArea, l_mpzSQRT);

	/* Compute search area */
	mpz_sub(l_mpzArea, l_mpzEndOfSearchArea, l_mpzBeginOfSearchArea);
	mpz_cdiv_q_ui(l_mpzOnePercent,l_mpzArea,(unsigned long)100);

	/* Copy number in iterator. We are going to modify Iterator in order to  try to be a diviser of Number. - thus check
	   earch one from end to beginOfSearchArea */
	mpz_set(l_mpzIterator, l_mpzEndOfSearchArea);

	/* If there is an autoloading, we need to put old values of progression here */
	if(p_structStructure->bLoaded)
	{
		/* We start with the previous percentage minus one in order to set a security area */
		mpz_set_ui(l_mpzPercent,(long)  (p_structStructure->iThreadProgressionTable[p_iSectionNumber] > 0) ?
				p_structStructure->iThreadProgressionTable[p_iSectionNumber] - 1 : 0);
		mpz_mul(l_mpzCurrentPosition, l_mpzPercent, l_mpzOnePercent);

		mpz_sub(l_mpzIterator, l_mpzIterator, l_mpzCurrentPosition);
	}

	/*  Make iterator odd number - because we jump two by two. Thus, we need to start with an odd number */
	if(!mpz_odd_p(l_mpzIterator))
	{
		mpz_sub_ui(l_mpzIterator, l_mpzIterator, 1);
	}

	#ifdef DEBUG_VERBOSE
	LOG_WRITE_STRING_LONG_LONG("Start : End  ", mpz_get_ui(l_mpzBeginOfSearchArea), mpz_get_ui(l_mpzEndOfSearchArea))
	#endif


	/* This is the same loop, but, call usleep gives a really slow computation (even if it is usleep(0)), thus, there are two loop,
	   the first ont with usleep call, and the second one without for the fast computation time */

	if(p_structStructure->iModerationTime > 0)
	{
		/* Start to divide by all people between the number his own sqrt */
		for(;;)
		{
			/* Sleep a little bit each time in order to avoid CPU overloading during day computation */
			usleep(p_structStructure->iModerationTime);

			/* -1 is for the first one : to display the 0% */
			if((mpz_cmp(l_mpzRefreshCounter, l_mpzOnePercent) > 0) || (mpz_cmp_d(l_mpzRefreshCounter, (double)-1) == 0))
			{
				if(p_structStructure->bDead == TRUE)
				{
					mpz_clear(l_mpzBeginOfSearchArea);
					mpz_clear(l_mpzEndOfSearchArea);
					mpz_clear(l_mpzSQRT);
					mpz_clear(l_mpzIterator);
					mpz_clear(l_mpzArea);
					mpz_clear(l_mpzCurrentPosition);
					mpz_clear(l_mpzPercent);
					mpz_clear(l_mpzOnePercent);
					mpz_clear(l_mpzTmp);
					mpz_clear(l_mpzRefreshCounter);
					return DONT_KNOW;
				}

				mpz_set_ui(l_mpzRefreshCounter, (long)0);

				mpz_mul_ui(l_mpzTmp, l_mpzCurrentPosition, (unsigned long)100);
				mpz_cdiv_q(l_mpzPercent, l_mpzTmp, l_mpzArea);

				#pragma omp critical (displayProgressBar)
				{
					drawLoadingBar(p_iSectionNumber + 1, mpz_get_ui(l_mpzPercent), 100, -1, PROGRESS_BAR_COLOR);
					p_structStructure->iThreadProgressionTable[p_iSectionNumber] = mpz_get_ui(l_mpzPercent);
				}
			}

			mpz_add_ui(l_mpzRefreshCounter, l_mpzRefreshCounter, 2);
			mpz_add_ui(l_mpzCurrentPosition, l_mpzCurrentPosition, 2);
			mpz_sub_ui(l_mpzIterator,l_mpzIterator, 2);

			l_bReturnOfFunction = mpz_divisible_p(p_mpzNumber, l_mpzIterator);

			if(l_bReturnOfFunction)	{break;}			/* else, we continue...*/

			/* Compare begining of search area and Iterator. Return a positive value if Begin > Iterator */
			l_bReturnOfFunction = mpz_cmp(l_mpzBeginOfSearchArea, l_mpzIterator);
			if(l_bReturnOfFunction > 0)
			{
				mpz_clear(l_mpzBeginOfSearchArea);
				mpz_clear(l_mpzEndOfSearchArea);
				mpz_clear(l_mpzSQRT);
				mpz_clear(l_mpzIterator);
				mpz_clear(l_mpzArea);
				mpz_clear(l_mpzCurrentPosition);
				mpz_clear(l_mpzPercent);
				mpz_clear(l_mpzOnePercent);
				mpz_clear(l_mpzTmp);
				mpz_clear(l_mpzRefreshCounter);
				return TRUE;
			}
		}
	}
	else
	{
		/* Start to divide by all people between the number his own sqrt */
		for(;;)
		{
			/* -1 is for the first one : to display the 0% */
			if((mpz_cmp(l_mpzRefreshCounter, l_mpzOnePercent) > 0) || (mpz_cmp_d(l_mpzRefreshCounter, (double)-1) == 0))
			{
				if(p_structStructure->bDead == TRUE)
				{
					mpz_clear(l_mpzBeginOfSearchArea);
					mpz_clear(l_mpzEndOfSearchArea);
					mpz_clear(l_mpzSQRT);
					mpz_clear(l_mpzIterator);
					mpz_clear(l_mpzArea);
					mpz_clear(l_mpzCurrentPosition);
					mpz_clear(l_mpzPercent);
					mpz_clear(l_mpzOnePercent);
					mpz_clear(l_mpzTmp);
					mpz_clear(l_mpzRefreshCounter);
					return DONT_KNOW;
				}

				mpz_set_ui(l_mpzRefreshCounter, (long)0);

				mpz_mul_ui(l_mpzTmp, l_mpzCurrentPosition, (unsigned long)100);
				mpz_cdiv_q(l_mpzPercent, l_mpzTmp, l_mpzArea);

				#pragma omp critical (displayProgressBar)
				{
					drawLoadingBar(p_iSectionNumber + 1, mpz_get_ui(l_mpzPercent), 100, -1, PROGRESS_BAR_COLOR);
					p_structStructure->iThreadProgressionTable[p_iSectionNumber] = mpz_get_ui(l_mpzPercent);
				}
			}

			mpz_add_ui(l_mpzRefreshCounter, l_mpzRefreshCounter, 2);
			mpz_add_ui(l_mpzCurrentPosition, l_mpzCurrentPosition, 2);
			mpz_sub_ui(l_mpzIterator,l_mpzIterator, 2);

			l_bReturnOfFunction = mpz_divisible_p(p_mpzNumber, l_mpzIterator);

			if(l_bReturnOfFunction)	{break;}			/* else, we continue...*/

			l_bReturnOfFunction = mpz_cmp(l_mpzBeginOfSearchArea, l_mpzIterator);
			if(l_bReturnOfFunction > 0)
			{
				mpz_clear(l_mpzBeginOfSearchArea);
				mpz_clear(l_mpzEndOfSearchArea);
				mpz_clear(l_mpzSQRT);
				mpz_clear(l_mpzIterator);
				mpz_clear(l_mpzArea);
				mpz_clear(l_mpzCurrentPosition);
				mpz_clear(l_mpzPercent);
				mpz_clear(l_mpzOnePercent);
				mpz_clear(l_mpzTmp);
				mpz_clear(l_mpzRefreshCounter);
				return TRUE;
			}
		}
	}
	mpz_clear(l_mpzBeginOfSearchArea);
	mpz_clear(l_mpzEndOfSearchArea);
	mpz_clear(l_mpzSQRT);
	mpz_clear(l_mpzIterator);
	mpz_clear(l_mpzArea);
	mpz_clear(l_mpzCurrentPosition);
	mpz_clear(l_mpzPercent);
	mpz_clear(l_mpzOnePercent);
	mpz_clear(l_mpzTmp);
	mpz_clear(l_mpzRefreshCounter);
	return FALSE;
}









/** @brief
  * This function compute a statisitc algorithm in order to find if a number is
  * a pseudo prime number (certainly a prime number but it's not 100% proved)
  * or a normal number. If trhis function find a diviser, it's dead. But, if the function
  * doesn't, we are allowed to think that, maybee, this number is a prime number.
  * Code provided by : http://en.literateprograms.org/Miller-Rabin_primality_test_%28C,_GMP%29#chunk%20def:compute%20s%20and%20d
  * @param p_mpzNumber : hypothetic prime number to check
  * @param p_iSectionNumber : current thread
  * @param p_iTotalSection : total number of active thread (unused here, putted in the prototype for compatibility issues)
  * @param p_structStructure : all usefull informations about the program
  * @return TRUE if the hypothetic prime number seem to be prime, or FALSE if it in not and we can prove it
  *
  */
int isItAPrimeNumberMRMultiThread(mpz_t p_mpzNumber, int p_iSectionNumber, int p_iTotalSection __attribute__((unused)), structProgramInfo* p_structStructure)
{
	/* This code, and its dependancies are copied from http://en.literateprograms.org/Miller-Rabin_primality_test_%28C,_GMP%29#chunk%20def:compute%20s%20and%20d
	   thanks to them. This code is actually one of the GMP implementation of the Miller Rabin test. For any update
	   check GMP project */
	gmp_randstate_t rand_state;
	gmp_randinit_default(rand_state);
	gmp_randseed_ui (rand_state, time(NULL));

	if(p_structStructure->bFastMode == TRUE)
	{
		return miller_rabin_nographic(p_mpzNumber, rand_state, p_structStructure, p_iSectionNumber);
	}
	else
	{
		return miller_rabin(p_mpzNumber, rand_state, p_structStructure, p_iSectionNumber);
	}
}

/**
  * @brief This function just do some unary test on a prime number.
  * This function is the no-graphic version of the miller_rabin_pass
  * Thanks to http://en.literateprograms.org/Miller-Rabin_primality_test_%28C,_GMP%29#chunk%20def:compute%20s%20and%20d
  * for the algorithm...
  * @param p_mpzRandom : a random number gived in order to check a wide area of number. It is a probabilistic algo
  * @param p_mpzNumber : hypothetic prime number to check
  * @return TRUE if the number could be a prime number or FALSE if it is not and we are sure of that
  *
  */
int miller_rabin_pass(mpz_t p_mpzRandom, mpz_t p_mpzNumber)
{
	int i, s;
	mpz_t a_to_power, d, n_minus_one;

	mpz_init(n_minus_one);
	mpz_sub_ui(n_minus_one, p_mpzNumber, 1);

	s = 0;
	mpz_init_set(d, n_minus_one);
	while (mpz_even_p(d))
	{
		mpz_fdiv_q_2exp(d, d, 1);
		s++;
	}

	mpz_init(a_to_power);
	mpz_powm(a_to_power,p_mpzRandom, d, p_mpzNumber);
	if (mpz_cmp_ui(a_to_power, 1) == 0)
	{
		mpz_clear(a_to_power);
		mpz_clear(d);
		mpz_clear(n_minus_one);
		return TRUE;
	}

	for(i=0; i < s-1; i++)
	{
		if (mpz_cmp(a_to_power, n_minus_one) == 0)
		{
			mpz_clear(a_to_power);
			mpz_clear(d);
			mpz_clear(n_minus_one);
			return TRUE;
		}
		mpz_powm_ui (a_to_power, a_to_power, 2,  p_mpzNumber);
	}

	if (mpz_cmp(a_to_power, n_minus_one) == 0)
	{
		mpz_clear(a_to_power);
		mpz_clear(d);
		mpz_clear(n_minus_one);
		return TRUE;
	}
	mpz_clear(a_to_power);
	mpz_clear(d);
	mpz_clear(n_minus_one);
	return FALSE;
}







/**
  * @brief
  * This function monitor unary check on the supposed prime number. This function display the progress bar
  * and manage how many check there is on a number per thread
  * Thanks to http://en.literateprograms.org/Miller-Rabin_primality_test_%28C,_GMP%29#chunk%20def:compute%20s%20and%20d for
  * the algorithm.
  * @param p_mpz_Number : checked number, in mpz format
  * @param rand_state : in order to manage the random in GMP
  * @param p_structCommon : all usefull data on the program
  * @param p_iSectionNumber : number of the section, it is actually the number of the currrent thread
  * @return TRUE (if the number MAY BE prime) or FALSE if it is not and it is proved
  */
int miller_rabin(mpz_t p_mpzNumber, gmp_randstate_t rand_state, structProgramInfo* p_structCommon, int p_iSectionNumber)
{
	int l_iProgression;
	int l_iOnePercent;

	mpz_t a;
	int repeat;
	mpz_init(a);
	l_iOnePercent = (p_structCommon->iWantedMRCheck < 100) ? 1 : p_structCommon->iWantedMRCheck / 100;

	/* Init the default starting point of the loop - If there is an autoloading, the starting point is changed */
	repeat = 0;

	/* If there is an autoloading, we need to put old values of progression here */
	if(p_structCommon->bLoaded)
	{
		/* We start with the previous percentage minus one in order to set a security area */
		l_iProgression =  (p_structCommon->iThreadProgressionTable[p_iSectionNumber] > 0) ?
				p_structCommon->iThreadProgressionTable[p_iSectionNumber] - 1 : 0;

		/* We only saved the current percentage, thus, we need to recompute to repeat value */
		repeat = l_iProgression * l_iOnePercent;
	}


	for(; repeat < p_structCommon->iWantedMRCheck; repeat++)
	{
		if( repeat % l_iOnePercent == 0)
		{
			if(p_structCommon->bDead == TRUE)
			{
				mpz_clear(a);
				return DONT_KNOW;
			}

			/* Compute percentage and avoid overflow of the int by putting the 'repeat' value in a long int */
			l_iProgression = (repeat);
			l_iProgression *= 100;
			l_iProgression /= (p_structCommon->iWantedMRCheck - 1);
			#pragma omp critical (displayProgressBar)
			{
				drawLoadingBar(p_iSectionNumber + 1, l_iProgression, 100, -1, PROGRESS_BAR_COLOR);
				p_structCommon->iThreadProgressionTable[p_iSectionNumber] = l_iProgression;
			}
		}

		do
		{
			mpz_urandomm(a, rand_state, p_mpzNumber);
		} while (mpz_sgn(a) == 0);

		if (miller_rabin_pass(a, p_mpzNumber) == FALSE)
		{
			mpz_clear(a);
			return FALSE;
		}
	}
	mpz_clear(a);
	return TRUE;
}






/**
  * @brief
  * Difference is the remove of all graphic parts. For the really fast computation cycle and daemon mode
  * This function monitor unary check on the supposed prime number. This function display the progress bar
  * and manage how many check there is on a number per thread
  * Thanks to http://en.literateprograms.org/Miller-Rabin_primality_test_%28C,_GMP%29#chunk%20def:compute%20s%20and%20d for
  * the algorithm.
  * @param p_mpz_Number : checked number, in mpz format
  * @param rand_state : in order to manage the random in GMP
  * @param p_structCommon : all usefull data on the program
  * @param p_iSectionNumber : unused but still here for compatibility reasons (we need to keep quite the same prototype)
  * @return TRUE (if the number MAY BE prime) or FALSE if it is not and it is proved
  */
int miller_rabin_nographic(mpz_t p_mpzNumber, gmp_randstate_t rand_state, structProgramInfo* p_structCommon, int p_iSectionNumber __attribute__((unused)))
{
	mpz_t a;
	int repeat;
	mpz_init(a);

	/* Init the default starting point of the loop - If there is an autoloading, the starting point is changed */
	repeat = 0;

	/* We put the check here to avoid to check it each time in the loop. But, you can't stop the program
	   during it execution, you need to wait the end of the current number testing */
	if(p_structCommon->bDead == TRUE)
	{
		mpz_clear(a);
		return DONT_KNOW;
	}

	for(; repeat < p_structCommon->iWantedMRCheck; repeat++)
	{
		do
		{
			mpz_urandomm(a, rand_state, p_mpzNumber);
		} while (mpz_sgn(a) == 0);

		if (miller_rabin_pass(a, p_mpzNumber) == FALSE)
		{
			mpz_clear(a);
			return FALSE;
		}
	}
	mpz_clear(a);
	return TRUE;
}
