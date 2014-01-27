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
  * In the algorithm we only can use prime number. These numbers are not really long, thus there
  * is no need to improve this algorithm. We just need to know if the number is a prime number,
  * and after we can do 2^N - 1 and start the bug computing
  *
  * Simple prime number testing -> check all number since sqrt(n) is reached
  */
int isItAPrimeNumber(mpz_t p_mpzNumber)
{
	int l_bReturnOfFunction = FALSE;
	mpz_t l_mpzSQRT;
	mpz_t l_mpzIterator;

	mpz_init(l_mpzSQRT);
	mpz_init(l_mpzIterator);

	LOG_WRITE("Compute: Try to find if a simple number is prime number or not")

	// Copy number in iterator. We are going to modify Iterator in order to  try to be a diviser of Number.
	mpz_set(l_mpzIterator, p_mpzNumber);

	// Do the SQRT
	mpz_sqrt (l_mpzSQRT, p_mpzNumber);

	// Start to divide by all people between the number his own sqrt
	for(;;)
	{
		mpz_sub_ui(l_mpzIterator,l_mpzIterator, 2);		// p_mpzIterator - 1  -> we use _ui because 1 is not a mpz number, and a cast isn't possible
									// we substract one and after anothyer one because it's useless to check the even number. an even number cannot divide an odd number.
		l_bReturnOfFunction = mpz_divisible_p(p_mpzNumber, l_mpzIterator);

		//char buf[500];
		//sprintf(buf, "echo \"%s\" >> pnd.log",  mpz_get_str(NULL, 10, l_mpzIterator));
		//system(buf);
		//sprintf(buf, "echo \"%s\" >> pnd.log",  mpz_get_str(NULL, 10, p_mpzNumber));
		//system(buf);

		if(l_bReturnOfFunction)	{break;}			// else, we continue...

		l_bReturnOfFunction = mpz_cmp(l_mpzSQRT, l_mpzIterator); //Compare SQRT and Iterator. Return a positive value if SQRT > Iterator
		if(l_bReturnOfFunction > 0)
		{
			// Clean...
			mpz_clear(l_mpzSQRT);
			mpz_clear(l_mpzIterator);
			mpz_clear(p_mpzNumber);

			LOG_WRITE("Compute: yes it is !")

			return TRUE;
		}
	}

	// Clean...
	mpz_clear(l_mpzSQRT);
	mpz_clear(l_mpzIterator);
	mpz_clear(p_mpzNumber);

	LOG_WRITE("Compute: no it is not...")

	return FALSE;
}



/**
  * This function compute a statisitc algorithm in order to find if a number is
  * a pseudo prime number (certainly a prime number but it's not 100% proved)
  * or a normal number. If trhis function find a diviser, it's dead. But, if the function
  * doesn't, we are allowed to think that, maybee, this number is a prime number.
  */

int millerRabin(mpz_t p_mpzA, mpz_t p_mpzN, int p_iScreenLineNumber)
{

#ifdef	DEBUG

	int l_iIterateur = 0;

	for(l_iIterateur = 0; l_iIterateur < 100; l_iIterateur++)
	{
		drawLoadingBar(p_iScreenLineNumber, l_iIterateur, 100, -1, enumBlanc);
		usleep(30000);
	}





	return TRUE;
#endif

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

	// Random number
	gmp_randinit_default(localStatus);
	mpz_urandomm (a, localStatus, p_mpzN);

	mpz_set_ui(k,0);
	for(;;)
	{
		if(mpz_congruent_ui_p (e, 0, 2)) // n congue c (d) <=>  e congue 0 (2)
		{
			break;
		}
		mpz_cdiv_q_ui(tmp,e,2);	// e / 2 -> tmp
		mpz_set(e,tmp);		// e = tmp;
		mpz_add_ui(k,k,1);	// k++
	}


	//mpz_powm (mpz_t rop, const mpz_t base, const mpz_t exp, const mpz_t mod);	Set rop to (base raised to exp) modulo mod.
	mpz_powm (p, a, e, p_mpzN);	// there is also mpz_powm_sec but i don't know the difference with mpz_powm

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

		// p = (p*p)%n
		mpz_powm_ui(tmp,p,2,p_mpzN);
		mpz_set(p,tmp);
		mpz_add_ui(i,i,1);
	}
}
