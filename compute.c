/**
  *
  *	Set of usefull functions of GMP
  *
  * int mpz_divisible_p (const mpz_t n, const mpz_t d)			return non zero value if YES
  * mpz_congruent_p (const mpz_t n, const mpz_t c, const mpz_t d)
  * void mpz_sqrt (mpz_t rop, const mpz_t op)
  * int mpz_cmp (const mpz_t op1, const mpz_t op2)			Compare op1 and op2. Return a positive value if op1 > op2, zero if op1 = op2, or a negative value if op1 < op2. 
  * Macro: int mpz_odd_p (const mpz_t op)
  *
  */


#include "conf.h"


/**
  *
  * In the miller-rabin algorithm we only can use prime number. These numbers are not really long, thus there
  * is no need to improve this algorithm. We just need to know if the number is a prime number,
  * and after we can do 2^N - 1 and start the big computing
  *
  * Simple prime number testing -> check all number since sqrt(n) is reached
  */
char isItAPrimeNumberMPZ(mpz_t p_mpzNumber)
{
	int l_bReturnOfFunction = FALSE;
	mpz_t l_mpzSQRT;
	mpz_t l_mpzIterator;

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
	if(l_bReturnOfFunction)	{return FALSE;}			/* else, we continue... */


	/* Start to divide by all people between the number his own sqrt */
	for(;;)
	{
		mpz_sub_ui(l_mpzIterator,l_mpzIterator, 2);		/* p_mpzIterator - 1  -> we use _ui because 1 is not a mpz number, and a cast isn't possible */
									/* we substract one and after anothyer one because it's useless to check the even number. an even number cannot divide an odd number. */
		l_bReturnOfFunction = mpz_divisible_p(p_mpzNumber, l_mpzIterator);

		if(l_bReturnOfFunction)	{break;}			/* else, we continue... */

		l_bReturnOfFunction = mpz_cmp(l_mpzSQRT, l_mpzIterator); /*Compare SQRT and Iterator. Return a positive value if SQRT > Iterator */
		if(l_bReturnOfFunction > 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}





/**
  *
  * And the unsigned long int version of the previous function.
  * In order to test mersenne order we don't need GMP because order will remains under MAX of unsigned long int
  *
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
		l_dIterator -= 2;					/* p_mpzIterator - 1  -> we use _ui because 1 is not a mpz number, and a cast isn't possible */
									/* we substract one and after anothyer one because it's useless to check the even number. an even number cannot divide an odd number. */
		l_dReturnOfFunction = fmod(p_dNumber, l_dIterator);

		if((int)l_dReturnOfFunction == 0) {break;}			/* else, we continue... */

		/*Compare SQRT and Iterator. Return a positive value if SQRT > Iterator */
		if(l_dIterator < l_dSQRT)
		{
			return TRUE;
		}
	}
	return FALSE;
}






/**
  * An other basic function in order to find if it is a prime number.
  *
  * This one is design to be used with multithread calls. We are able to
  * split all computation in some many parts in order to execute each one
  * in their own thread.
  *
  * But it is still the same poor algo than the function isItAPrimeNumber
  * and thus, there is any optimisation here...
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

	/* Copy number in iterator. We are going to modify Iterator in order to  try to be a diviser of Number. - thus check earch one from end to beginOfSearchArea */
	mpz_set(l_mpzIterator, l_mpzEndOfSearchArea);

	/* If there is an autoloading, we need to put old values of progression here */
	if(p_structStructure->bLoaded)
	{
		/* We start with the previous percentage minus one in order to set a security area */
		mpz_set_ui(l_mpzPercent,(long)  (p_structStructure->iThreadProgressionTable[p_iSectionNumber] > 0) ? p_structStructure->iThreadProgressionTable[p_iSectionNumber] - 1 : 0);
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

	/* Start to divide by all people between the number his own sqrt */
	for(;;)
	{
		if((mpz_cmp(l_mpzRefreshCounter, l_mpzOnePercent) > 0) || (mpz_cmp_d(l_mpzRefreshCounter, (double)-1) == 0))	/* -1 is for the first one : to display the 0% */
		{
			if(p_structStructure->bDead == TRUE)
			{
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
		mpz_sub_ui(l_mpzIterator,l_mpzIterator, 2);		/* p_mpzIterator - 1  -> we use _ui because 1 is not a mpz number, and a cast isn't possible */
									/* we substract one and after anothyer one because it's useless to check the even number. an even number cannot divide an odd number. */
		l_bReturnOfFunction = mpz_divisible_p(p_mpzNumber, l_mpzIterator);

		if(l_bReturnOfFunction)	{break;}			/* else, we continue...*/

		l_bReturnOfFunction = mpz_cmp(l_mpzBeginOfSearchArea, l_mpzIterator); /* Compare begining of search area and Iterator. Return a positive value if Begin > Iterator */
		if(l_bReturnOfFunction > 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}


















/**
  * This function compute a statisitc algorithm in order to find if a number is
  * a pseudo prime number (certainly a prime number but it's not 100% proved)
  * or a normal number. If trhis function find a diviser, it's dead. But, if the function
  * doesn't, we are allowed to think that, maybee, this number is a prime number.
  */

int millerRabin(mpz_t p_mpzA, mpz_t p_mpzN)
{
	mpz_t p;
	mpz_t e;
	mpz_t a;
	mpz_t m;
	mpz_t i;
	mpz_t k;
	mpz_t tmp;
	gmp_randstate_t localStatus;

	mpz_init(p);
	mpz_init(e);
	mpz_init(m);
	mpz_init(i);
	mpz_init(a);
	mpz_init(k);
	mpz_init(tmp);

	mpz_sub_ui(m,p_mpzN, 1);
	mpz_set(e,m);

	/* Random number */
	gmp_randinit_default(localStatus);
	mpz_urandomm (a, localStatus, p_mpzN);

	mpz_set_ui(k,0);
	for(;;)
	{
		if(mpz_congruent_ui_p (e, 0, 2)) /* n congue c (d) <=>  e congue 0 (2) */
		{
			break;
		}
		mpz_cdiv_q_ui(tmp,e,2);	/* e / 2 -> tmp */
		mpz_set(e,tmp);		/* e = tmp; */
		mpz_add_ui(k,k,1);	/* k++ */
	}


	/*mpz_powm (mpz_t rop, const mpz_t base, const mpz_t exp, const mpz_t mod);	Set rop to (base raised to exp) modulo mod. */
	mpz_powm (p, a, e, p_mpzN);	/* there is also mpz_powm_sec but i don't know the difference with mpz_powm */

	if(mpz_cmp_ui (p, 1) == 0)
	{
	LOG_WRITE("La 0")
	mpz_clear(p);
		mpz_clear(e);
		mpz_clear(m);
		mpz_clear(a);
		mpz_clear(i);
		mpz_clear(k);
		mpz_clear(tmp);
		gmp_randclear(localStatus);
		mpz_clear(p_mpzA);
		mpz_clear(p_mpzN);
		return FALSE;
	}

	mpz_set_ui(i,0);
	for(;;)
	{
		if(mpz_cmp(i,k) < 0)
		{
		LOG_WRITE("La bese")
	mpz_clear(p);
			mpz_clear(e);
			mpz_clear(a);
			mpz_clear(m);
			mpz_clear(i);
			mpz_clear(k);
			mpz_clear(tmp);

			gmp_randclear (localStatus);
			mpz_clear(p_mpzA);
			mpz_clear(p_mpzN);
			return TRUE;
		}
		if(mpz_cmp(p,m) == 0)
		{
LOG_WRITE("La 1")
			mpz_clear(p);
			mpz_clear(e);
			mpz_clear(a);
			mpz_clear(m);
			mpz_clear(i);
			mpz_clear(k);
			mpz_clear(tmp);

			gmp_randclear (localStatus);
			mpz_clear(p_mpzA);
			mpz_clear(p_mpzN);
			return FALSE;
		}
		if(mpz_cmp_ui(p,1) == 0)
		{
LOG_WRITE("La 2")
			mpz_clear(p);
			mpz_clear(e);
			mpz_clear(m);
			mpz_clear(a);
			mpz_clear(i);
			mpz_clear(k);
			mpz_clear(tmp);

			gmp_randclear (localStatus);
			mpz_clear(p_mpzA);
			mpz_clear(p_mpzN);
			return TRUE;
		}

		/* p = (p*p)%n */
		mpz_powm_ui(tmp,p,2,p_mpzN);
		mpz_set(p,tmp);
		mpz_add_ui(i,i,1);
	}
}
