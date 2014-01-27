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

	int tid, nbth, numthr;

	numthr=6;

	// Adding some useless comment
	#pragma omp parallel private(nbth, tid) num_threads(numthr)
	{
		tid = omp_get_thread_num();
		usleep(20000*tid);

		#pragma omp critical (writelog)
		{
			LOG_WRITE("I am in a Thread")
			LOG_WRITE_LONG((long int)tid)
		}
	}
}
