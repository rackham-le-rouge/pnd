/**
 * @file        signalmanagement.c
 * @program     pnd
 * @brief       Signal handling
 * @description In order to handle all signals of this program
 * @date        1/ 2014
 * @licence     Beerware (rev 42)
 * @author      Jerome GRARD - A bored engineering student...
 */


#include "conf.h"




/** @brief
  * initialisation of the signal routing, in order to handle the SIGTERM, SIGINT and some SIGUSR
  *
  */
void initialisationOfTheSignal(void)
{


	if (signal(SIGUSR1, signalHandler) == SIG_ERR)
	{
		LOG_WRITE("Toogle speed")
	}

	if (signal(SIGUSR2, signalHandler) == SIG_ERR)
	{
		killTheApp(NULL);
	}

	if (signal(SIGTERM, signalHandler) == SIG_ERR)
	{
		LOG_WRITE("Anormal termination of the program... Saving context NOW !")
		killTheApp(NULL);
	}

	if (signal(SIGINT, signalHandler) == SIG_ERR)
	{
		killTheApp(NULL);
	}
}


/** @brief
  * Signal handler. When signal is launched -> this function is called
  * @param p_iSignalNo : number of the signal sended
  */
void signalHandler(int p_iSignalNo)
{
        pid_t   l_pidPid;

	/* Personal signal 1 - Used to toogle computation speed */
	if(p_iSignalNo == SIGUSR1)
	{
		LOG_WRITE("Toogle speed")
		toogleProgramSpeed(MODE_TOOGLE, NULL);
	        LOG_WRITE("--------------------------------RESTART--------------------------------")
	        LOG_WRITE("Kill the old program to force it to save its data")

		/* We are going to restart program with a big cheat. A fork is launched, he kills program and restart it using autosave
		   function */
	        /* Do the fork */
	        l_pidPid = fork();
	        if(l_pidPid < 0)
	        {
       	         	/* Fork error, giving up the restart */
	                LOG_WRITE("Fork problem. Cant restart program. We are going to be killed")

			/* Suicide is better. When user restart it manually from command line, program will resume his activity */
			killTheApp(NULL);
        	}
        	if(l_pidPid > 0)
        	{
                	/* Because we are in the parent program */
			killTheApp(NULL);
        	}

        	LOG_WRITE("Kill my father. Done. Now i am restarting.")

		popen("./pnd -d", "w");				/* Start another PND in daemon mode.  Because we currently are in daemon mode.
								   If you use 'r' option it don't work */
		usleep(5000);					/* Sleep a while - Leave him time to save all and kill his father */
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
		saveCurrentContext(MODE_SAVE, NULL);
		commonSignalEnding();
	}
}


/** @brief
  * Function called in order to toogle computation speed.
  * User use this function when program is in deamon mode.
  * There is no GUI, thus, signal are the only way
  *
  * @param p_cMode : MODE_ INIT or MODE_ TOOGLE in order to toogle speed of the program
  * @param p_structCommon : all usefull data about this program
  */
void toogleProgramSpeed(char p_cMode, structProgramInfo* p_structCommon)
{
	static structProgramInfo* l_structCommon;
	static int l_iModerationTime;

	if(p_cMode == MODE_INIT)
	{
		LOG_WRITE("Init structCommon pointer for the toogling function")
		l_structCommon = p_structCommon;
		l_iModerationTime = (l_structCommon->iModerationTime > 0) ? l_structCommon->iModerationTime : DEFAULT_MODERATION_TIME;
	}
	if(p_cMode == MODE_TOOGLE)
	{
		if(l_structCommon->iModerationTime == 0)
		{
			l_structCommon->iModerationTime = l_iModerationTime;
		}
		else
		{
			l_structCommon->iModerationTime = 0;
		}
	}
}




/** @brief
  * Saving function, this function needs to be initialized in order to set the adress of the common structure
  * This function is also called by main in order to get back the old context. I put it here to let these functions gathered in order
  * to be more understandable. Even if it is not the right file name (basically reserved for signal handling). But io functions is
  * only used by and for signal handling. If the program close properly, there is no need to save the context.
  *
  * @param p_cMode : MODE_ INIT or MODE_ SAVE or MODE_ LOAD in order to select the right mode
  * @param p_structCommon : all usefull data about this program
  */
void saveCurrentContext(char p_cMode, structProgramInfo* p_structCommon)
{
	int l_iIterator;
	int l_iLoadedRow;
	int l_iTemporaryLoadedData;
	static structProgramInfo* l_structCommon;
	FILE* l_fileOutputFile;

	if(p_cMode == MODE_INIT)
	{
		LOG_WRITE("Saving function : Init of commonstruct pointer")
		l_structCommon = p_structCommon;

	}
	else if(p_cMode == MODE_SAVE)
	{
		LOG_WRITE("Saving function : Saving context")
		l_fileOutputFile = fopen("pnd.hotsave", "w");
		if(l_fileOutputFile != NULL)
		{
			LOG_WRITE("Saving function : output file opened successfully")

			/*
			We save the whole table because this table is refill at each new mersenne number check with the new parameters.
			We set new ThreadNumber (current working thread). And during computation, active threads actualize their
			percentage in the table when they do it on the screen. Thus, we take an exact picture of the situation when we
			save these data
			*/
			fprintf(l_fileOutputFile, "%d\n", l_structCommon->iRow);				/* Saving this data, if we
			open the pnd app withn a better screen, the program are going to crash with some kind of segfault */
			fprintf(l_fileOutputFile, "%d\n", l_structCommon->iMersenneOrder);
			fprintf(l_fileOutputFile, "%d\n", l_structCommon->iModerationTime);

			for(l_iIterator = 0; l_iIterator < l_structCommon->iRow + 1; l_iIterator++)		/* +1 because there is one more
														integer for the ThreadNumber */
			{
				fprintf(l_fileOutputFile, "%d\n", l_structCommon->iThreadProgressionTable[l_iIterator]);
			}

			fclose(l_fileOutputFile);
		}
		else
		{
			LOG_WRITE("Saving function : fail to open file")
		}
	}
	else if(p_cMode == MODE_LOAD)
	{
		LOG_WRITE("Load function : Try to load context, if it exist")
		l_fileOutputFile = fopen("pnd.hotsave", "r");
		if(l_fileOutputFile != NULL)
		{
			LOG_WRITE("Load function : there is a context saved. Loading...")
			l_structCommon->bLoaded = TRUE;

			/*** the first line -> The previous screen size, and so, the previous max thread number ***/
			fscanf(l_fileOutputFile, "%d\n", &l_iLoadedRow);
			fscanf(l_fileOutputFile, "%d\n", &(l_structCommon->iMersenneOrder));
			fscanf(l_fileOutputFile, "%d\n", &(l_structCommon->iModerationTime));

			if(l_iLoadedRow > l_structCommon->iRow)
			{
				LOG_WRITE("Load function : screen size have changed and the new screen dont allow us to load all threads parameters")
			}

			for(l_iIterator = 0; l_iIterator < l_iLoadedRow + 1; l_iIterator++)		/* +1 because there is one more integer
													   for the ThreadNumber */
			{
				fscanf(l_fileOutputFile, "%d\n", &l_iTemporaryLoadedData);

				/*** All lines == each thread progression ***/
				if(l_iIterator < l_structCommon->iRow)
				{
					/* We load each thread progression, and save only those we can. (if number of thread is < iRow) */
					l_structCommon->iThreadProgressionTable[l_iIterator] = l_iTemporaryLoadedData;
				}

				/*** The last line -> Thread number ***/
				if(l_iIterator == l_iLoadedRow)
				{
					/* And finally, the last value of the file is saved in the last int of the current table */
					if(l_iTemporaryLoadedData <= l_structCommon->iRow - 2)
					{
						/* In the previous configuration there is a correct number of threads. We can save them all
						   with the current screen size (and thus memory reserved) */
						l_structCommon->iThreadProgressionTable[l_iIterator] = l_iTemporaryLoadedData;
					}
					else
					{
						/* In the previous configuration there is more thread than we can display now (and than we
						   had reserved memory). So, cut their number and let a warning in the log file */
						l_structCommon->iThreadProgressionTable[l_iIterator] = l_structCommon->iRow - 2;
					}
					/* Apply modification and set the new thread number */
					l_structCommon->iThreadNumber = l_structCommon->iThreadProgressionTable[l_iIterator];
				}
			}

			fclose(l_fileOutputFile);

			/* We delete the file in order to avoid loading of outdated data during the next start */
			remove("pnd.hotsave");
		}
		else
		{
			LOG_WRITE("Load function : There is no context saved. Default values are going to be loaded")
			l_structCommon->bLoaded = FALSE;
		}

	}
	else
	{
		LOG_WRITE("Saving function : unknow mode is used with this function. Rejected. Nothing to do.")
	}
}



/** @brief
  * Commong signal ending : close the graphic screen and quit the program
  */
void commonSignalEnding(void)
{
	/* Killing properly the application */

	LOG_WRITE("Anormal end of the program... Hope to see you again !")
	killTheApp(NULL);
}
