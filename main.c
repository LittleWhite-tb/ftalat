/*
 * ftalat - Frequency Transition Latency Estimator
 * Copyright (C) 2013 Universite de Versailles
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

#include "CoreRelation.h"
#include "FreqGetter.h"
#include "FreqSetter.h"

#include "loop.h"
#include "measure.h"
#include "rdtsc.h"
#include "utils.h"

#ifdef _DUMP
   #include "dumpResults.h"
#endif

#include "ConfInterval.h"

#define NB_BENCH_META_REPET 10000
#define NB_VALIDATION_REPET 100
#define NB_TRY_REPET_LOOP 1000
#define NB_TRY_REPET 5
const float DIFF_OFFSET_PERCENTAGE = 25.0f;

unsigned long times[NB_BENCH_META_REPET] ;

/* easier impelemntation */
int msrValues[]=
{0, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 0};

void usage()
{
   fprintf(stdout,"./ftalat [-c coreID] startFreq targetFreq\n");
   fprintf(stdout,"\t-c coreID\t:\tto run the test on a precise core (default 0)\n");
}

double measureLoop(unsigned int nbMetaRepet)
{
   unsigned int i = 0;
   double mediumTime = 0;
   
   for ( i = 0 ; i < nbMetaRepet ; i++ )
   {
    	times[i] = loop();
   }
  
   /*printf("Calibration :\n");
   for ( i = 0 ; i < nbMetaRepet ; i++ )
   {
    	printf("%lu ", times[i]);
   }
   printf("\n");*/

   mediumTime = average(nbMetaRepet, times);
   return mediumTime;
}

void runTest(unsigned int startFreq, unsigned int targetFreq, unsigned int coreID)
{
   double startBenchTime=0;
   double targetBenchTime=0;
   unsigned long lowBoundTime=0;
   unsigned long highBoundTime=0;
   unsigned long time =0;
   
   unsigned long startLoopTime = 0;
   unsigned long lateStartLoopTime = 0;
   unsigned long endLoopTime = 0;

   /* Build the confidence interval of the sample mean */    
   double startBenchSD=0;
   double targetBenchSD=0;
   unsigned long startLowBoundTime=0;
   unsigned long startHighBoundTime=0;
   unsigned long targetLowBoundTime=0;
   unsigned long targetHighBoundTime=0;
   unsigned long targetQ1=0;
   unsigned long targetQ3=0;

#ifdef DVFS
   setFreq(coreID,targetFreq);
   waitCurFreq(coreID,targetFreq);
#else
   setDCM(coreID, targetFreq);
   /* do I need to waitCurDCM? */ 
#endif
   targetBenchTime = measureLoop(NB_BENCH_META_REPET);
   fprintf(stdout,"Bench %d %.2f\n",targetFreq, targetBenchTime); 
   targetBenchSD = sd(NB_BENCH_META_REPET, targetBenchTime, times);

   // Build the inter-quartile range for the target frequency
   interQuartileRange(NB_BENCH_META_REPET, times, 
		&targetQ1, &targetQ3);
#ifdef DVFS   
   setFreq(coreID,startFreq);
   waitCurFreq(coreID,startFreq);
#else
   setDCM(coreID, startFreq);
#endif
   startBenchTime = measureLoop(NB_BENCH_META_REPET);  
   fprintf(stdout,"Bench %d %.2f\n",startFreq, startBenchTime);
   startBenchSD = sd(NB_BENCH_META_REPET, startBenchTime, times);
   

   // Build the confidence interval for the target frequency
   confidenceInterval(NB_BENCH_META_REPET, targetBenchTime, targetBenchSD,
		&targetLowBoundTime, &targetHighBoundTime);

   fprintf(stdout,"targetLowbound : %lu ; targetHighbound : %lu\n",
			targetLowBoundTime,targetHighBoundTime);
   fprintf(stdout,"targetQ1 : %lu ; targetQ3 : %lu\n",
			targetQ1, targetQ3);


   // Build the confidence interval for the start frequency
   confidenceInterval(NB_BENCH_META_REPET, startBenchTime, startBenchSD,
		&startLowBoundTime, &startHighBoundTime);
   fprintf(stdout,"startLowbound : %lu ; startHighbound : %lu\n",
			startLowBoundTime,startHighBoundTime);

   // Check if the confidence intervals overlap
   if ( startLowBoundTime >= targetHighBoundTime || targetLowBoundTime >= startHighBoundTime )
   {
   		fprintf(stdout,"Confidence intervals do not overlap, alternatives are statistically different with selected confidence level\n");
   }
   else if( startLowBoundTime < targetHighBoundTime || targetLowBoundTime > startHighBoundTime )
   {
		if( ( startBenchTime >= targetLowBoundTime && startBenchTime <= targetHighBoundTime ) ||
		    ( targetBenchTime >= startLowBoundTime && targetBenchTime <= startHighBoundTime ) )
		{
   			fprintf(stdout,"Warning: confidence intervals overlap considerably, alternatives are equal with selected confidence level\n");
			return;
		}else
		{
   			fprintf(stdout,"Warning: confidence intervals overlap, we can not state any thing, need to do the t-test\n");
		}
   }

   //lowBoundTime  = targetLowBoundTime ;
   //highBoundTime = targetHighBoundTime;

   // Now we use the inter-quartile range
   lowBoundTime  = targetQ1;
   highBoundTime = targetQ3;

   sync();
   loop();
   warmup_cpuid();

   {
      unsigned int i = 0;
      unsigned int j = 0;
      unsigned int niters = 0;
      char validated = 0;
      double validateBenchTime=0;
      double validateBenchSD=0;
      unsigned long validateLowBoundTime=0;
      unsigned long validateHighBoundTime=0;

      do
      {
         startLoopTime = 0;
         lateStartLoopTime = 0;
         endLoopTime = 0;
         niters = 0;
         
#ifdef _DUMP
         resetDump();
#endif

         sync_rdtsc1(startLoopTime);
#ifdef DVFS
         setFreq(coreID,targetFreq);
#else 
         setDCM(coreID, targetFreq);
#endif
         sync_rdtsc1(lateStartLoopTime);
         do
         {
            time = loop();
#ifdef _DUMP
            writeDump(time);
#endif
         } while ( ( time < lowBoundTime || time > highBoundTime ) && ++niters < NB_TRY_REPET_LOOP );

         sync_rdtsc2(endLoopTime);

         // Validation
         validated = 1;
         times[0] = time;
         for ( i = 1 ; i < NB_VALIDATION_REPET ; i++ )
         {
            times[i] = loop();
#ifdef _DUMP
            writeDump(times[i]);
#endif            
         }

         // Build a confidence interval for the new time value
         validateBenchTime = average(NB_VALIDATION_REPET, times); 
         validateBenchSD   = sd(NB_VALIDATION_REPET, validateBenchTime, times);
         confidenceInterval(NB_VALIDATION_REPET, validateBenchTime, validateBenchSD,
                            &validateLowBoundTime, &validateHighBoundTime);

         if ( validateHighBoundTime < targetLowBoundTime  || validateLowBoundTime > targetHighBoundTime )
         {
            validated = 0;
#ifdef DVFS
            setFreq(coreID,startFreq);
            waitCurFreq(coreID,startFreq);
#else
            setDCM(coreID, startFreq);
#endif
         }
      }while(!validated && ++j < NB_TRY_REPET);

      fprintf(stdout,"Number of iterations to solution : %d ;  Number of attempts : %d\n", niters, j+1);	
      if ( j >= NB_TRY_REPET || validated == 0 )
         fprintf(stdout,"Warning: The computed change time may not be accurate\n"); 

      fprintf(stdout,"LastTime : %lu ; validateLowbound : %lu ; validateHighbound : %lu\n",
              time, validateLowBoundTime,validateHighBoundTime);
   }
   
   fprintf(stdout,"Change time (with write) : %lu\n" ,endLoopTime-startLoopTime);
   fprintf(stdout,"Change time : %lu\n" ,endLoopTime-lateStartLoopTime);
   fprintf(stdout,"Write cost : : %lu\n" ,lateStartLoopTime-startLoopTime);
}

void *thfn(void *arg)
{
   struct sched_param sp;

   (void) arg;

   // cancel me at anytime
   pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

   // go batch, min prio
   sched_getparam(0, &sp);
   sp.sched_priority = sched_get_priority_min(SCHED_BATCH);
   if (sched_setscheduler(0, SCHED_BATCH, &sp))
   {
      perror("setscheduler background");
   }

   while(1);

   return NULL;
}


void cleanup()
{
   // Put back a "neutral" governor
   setCPUGovernor("ondemand");
   
   closeFreqSetterFiles();
   
   freeCoreRelations();
   freeFreqInfo();
   
#ifdef _DUMP
   closeDump();
#endif
}


int main(int argc, char** argv)
{
   struct sched_param sp;
   pthread_t bgth;

   if ( argc != 3 && argc != 5 )
   {
      usage();
      return -1;
   }

   unsigned int coreID = 0;
   unsigned int startFreq = 0;
   unsigned int targetFreq = 0;
   
   unsigned int argcCounter = 1;
   
   // Option for core specification
   if ( strcmp(argv[1],"-c")==0 )
   {
      if ( sscanf(argv[2],"%u",&coreID) != 1 )
      {
         fprintf(stderr,"Fail to get the core ID argument\n");
         return -2;
      }
      argcCounter +=2;
      
      if ( argc != 5 )
      {
         fprintf(stderr,"Missing frequencies arguments\n");
         usage();
         return -1;
      }
   }

   if ( sscanf(argv[argcCounter],"%u",&startFreq) != 1 )
   {
      fprintf(stderr,"Fail to get the start frequency argument\n");
      return -3;
   }

   if ( sscanf(argv[argcCounter+1],"%u",&targetFreq) != 1 )
   {
      fprintf(stderr,"Fail to get the target freq argument\n");
      return -4;
   }
   
   // Additional checks
   if ( coreID >= getCoreNumber() )
   {
      fprintf(stdout,"The core ID that user gave is invalid\n");
      fprintf(stdout,"Core ID is set to 0\n");
      coreID = 0;
   }
#ifdef DVFS   
   initFreqInfo();
#endif
#ifdef DVFS   
   if ( isFreqAvailable(coreID,startFreq) == 0 )
   {
      fprintf(stdout,"The starting frequency that you have entered (%d) is not available for the core %d\n",startFreq,coreID);
      fprintf(stdout,"Aborting");
      cleanup();
      return -6;
   }
   
   if ( isFreqAvailable(coreID,targetFreq) == 0 )
   {
      fprintf(stdout,"The target frequency that you have entered (%d) is not available for the core %d\n",targetFreq,coreID);
      fprintf(stdout,"Aborting");
      cleanup();
      return -7;
   }
   
   initCoreRelations();
#endif 
   
#ifdef _DUMP
   openDump("./results.dump",NB_TRY_REPET_LOOP*NB_VALIDATION_REPET);
#endif

   // stay on cpu 2
   pinCPU(coreID);

   // create a background task to keep CPU awaken
   pthread_create(&bgth, NULL, thfn, NULL);
      
   // go realtime, max prio
   sched_getparam(0, &sp);
   sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
   if (sched_setscheduler(0, SCHED_FIFO, &sp))
   {
      perror("setscheduler background");
   }
#ifdef DVFS
   if ( setCPUGovernor("userspace") != 0 )
   {
      fprintf(stderr,"We are unable to set \"userspace\" governor. Do you have cpufreq and permissions ?\n");
      cleanup();
      return -5;
   }
#endif
#ifdef DVFS
   // Set the minimal frequency
   if ( openFreqSetterFiles() != 0 )
   {
      cleanup();
      return -3;
   }
#else
   if ( openDCMSetterMSRs() != 0) {
      /* cleanup(); ? */
      return -3;
   }
#endif

#ifdef DVFS
   setFreqForAllRelatedCore(coreID,getMinAvailableFreq(coreID));
   runTest(startFreq, targetFreq, coreID);
#else
   setDCM(coreID, msrValues[1]);
   runTest(msrValues[startFreq], msrValues[targetFreq], coreID);
#endif

   // kill bg thread
   pthread_cancel(bgth);
#ifdef DVFS
   cleanup();
#endif
   return 0;
}
