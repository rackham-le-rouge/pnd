/**
 * Program:     pnd
 * File:        signalmanagement.c
 * Brief:       Signal handling
 * Description: In order to handle all signals of this program
 * Date:        1/ 2014
 * Licence:     Beerware (rev 42)
 * Author:      Jerome GRARD - A bored engineering student...
 */


#include "conf.h"




/**
  * initialisation of the signal routing, in order to handle the SIGTERM, SIGINT and some SIGUSR
  */
void initialisationOfTheSignal(void)
{


	if (signal(SIGUSR1, signalHandler) == SIG_ERR)
	{
		exit(1);
	}

	if (signal(SIGUSR2, signalHandler) == SIG_ERR)
	{
		exit(1);
	}

	if (signal(SIGTERM, signalHandler) == SIG_ERR)
	{
		LOG_WRITE("Anormal termination of the program... Saving context NOW !")
		exit(1);
	}

	if (signal(SIGINT, signalHandler) == SIG_ERR)
	{
		exit(1);
	}



}


/**
  * Signal handler. When signal is launched -> this function is called
  */
void signalHandler(int p_iSignalNo)
{

	/* Personal signal 1 - Used to */
	if(p_iSignalNo == SIGUSR1)
	{

		commonSignalEnding();
	}
	if(p_iSignalNo == SIGUSR2)
	{
		commonSignalEnding();

	}

	/* In order to quit the program properly */
	if(p_iSignalNo == SIGTERM || p_iSignalNo == SIGINT)
	{
		LOG_WRITE("Sigterm or sigint send against me. Saving context")
		commonSignalEnding();
	}
}



/**
  * Commong signal ending : close the graphic screen and quit the program
  */
void commonSignalEnding(void)
{
	/* Show the cursor */
        curs_set(1);

	/* close windows */
	endwin();

	LOG_WRITE("Anormal end of the program... Hope to see you again !")

	/* kill the program */
	exit(0);
}
