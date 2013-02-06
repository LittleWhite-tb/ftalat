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

#include "FreqGetter.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <unistd.h>

#include "utils.h"

/**
 * \struct FreqsList
 * Keeps track of all the frequencies available for one core
 */
typedef struct FreqsList
{
   unsigned int nbFreqs;   /*!< number of frequencies available for this core */
   unsigned int* pFreqs;   /*!< the available frequencies */
}FreqsList;

FreqsList* pAvailableFreqsTable = NULL;

unsigned int getCoreNumber()
{
   static unsigned int nbCore = 0;
   
   if ( nbCore == 0 )
   {
      // Ask the system for numbers of cores
      nbCore = sysconf(_SC_NPROCESSORS_ONLN);
      if ( nbCore < 1 )
      {
         fprintf(stderr,"Fail to get the number of CPU\n");
         nbCore = 1;
      }
   }
   
   return nbCore;
}

void initFreqInfo()
{
   unsigned int nbCore = getCoreNumber();
   unsigned int i = 0;
   
   pAvailableFreqsTable = (FreqsList*) calloc(nbCore,sizeof(FreqsList));
   if ( pAvailableFreqsTable == NULL )
   {
      fprintf(stderr,"Fail to allocate memory for frequency table\n");
      return;
   }
   
   // Get all the frequencies available for all cores
   for ( i = 0 ; i < nbCore ; i++ )
   {
      FILE* pFreqsFile = openCPUFreqFile(i,"scaling_available_frequencies","r");
      if ( pFreqsFile != NULL )
      {
         size_t tabSize = 25;
         pAvailableFreqsTable[i].pFreqs = (unsigned int*) malloc(sizeof(unsigned int) * tabSize); // let's say 25 is enough for an init size
         if ( pAvailableFreqsTable[i].pFreqs != NULL )
         {
            unsigned int freq = 0;
            size_t counter = 0;
            while(fscanf(pFreqsFile,"%u",&freq) == 1 )
            {
               pAvailableFreqsTable[i].pFreqs[counter] = freq;
               counter++;
               if ( counter >= tabSize ) // Not enough space remaining
               {
                  // Double size
                  tabSize *= 2;
                  unsigned int* newFreqsTab = realloc(pAvailableFreqsTable[i].pFreqs,tabSize*sizeof(unsigned int));
                  if ( newFreqsTab != NULL )
                  {
                     pAvailableFreqsTable[i].pFreqs = newFreqsTab;
                  }
                  else
                  {
                     fprintf(stderr,"Fail to allocate more memory for frequency table\n");
                     break;
                  }
               }
            }
            
            pAvailableFreqsTable[i].nbFreqs = counter;
         }
         else
         {
            fprintf(stderr,"Fail to allocated memory for line of frequency table\n");
         }
         
         fclose(pFreqsFile);
      }
      else
      {
         pAvailableFreqsTable[i].nbFreqs = 0;
         pAvailableFreqsTable[i].pFreqs = NULL;
      }
   }
}

void freeFreqInfo()
{
   unsigned int nbCore = getCoreNumber();
   unsigned int i = 0;
   
   for ( i = 0 ; i < nbCore ; i++ )
   {
      free(pAvailableFreqsTable[i].pFreqs);
   }
   
   free(pAvailableFreqsTable);
}


unsigned int getCurFreq(unsigned int coreID)
{
   assert(coreID < getCoreNumber());
   
   unsigned int freq = 0;
   
   FILE* pFreqFile = openCPUFreqFile(coreID,"cpuinfo_cur_freq","r");
   if ( pFreqFile != NULL )
   {
      fscanf(pFreqFile,"%u",&freq);
      
      fclose(pFreqFile);
   }
   
   return freq;
}

inline void waitCurFreq(unsigned int coreID, unsigned int targetFreq)
{
   assert(coreID < getCoreNumber());
   
   unsigned int freq = 0;
   char filePathBuffer[BUFFER_PATH_SIZE]= {'\0'};
   snprintf(filePathBuffer,BUFFER_PATH_SIZE,CPU_PATH_FORMAT,coreID,"cpuinfo_cur_freq");
   
   do
   {
      FILE* pFreqFile = fopen(filePathBuffer,"r");
      if ( pFreqFile != NULL )
      {
         fscanf(pFreqFile,"%u",&freq);
         
         fclose(pFreqFile);
      }
   }while(freq != targetFreq);
}

unsigned int getMinAvailableFreq(unsigned int coreID)
{
   assert(coreID < getCoreNumber());
   
   if ( pAvailableFreqsTable[coreID].pFreqs )
   {
      return pAvailableFreqsTable[coreID].pFreqs[pAvailableFreqsTable[coreID].nbFreqs-1];
   }
   
   return 0;
}

unsigned int getMaxAvailableFreq(unsigned int coreID)
{
   assert(coreID < getCoreNumber());
   
   if ( pAvailableFreqsTable[coreID].pFreqs )
   {
      return pAvailableFreqsTable[coreID].pFreqs[0];
   }
   
   return 0;
}

int isFreqAvailable(unsigned int coreID, unsigned int freq)
{
   assert(coreID < getCoreNumber());
   
   unsigned int i = 0;
   if ( pAvailableFreqsTable[coreID].pFreqs )
   {
      for ( i = 0 ; i < pAvailableFreqsTable[coreID].nbFreqs ; i++ )
      {
         if ( pAvailableFreqsTable[coreID].pFreqs[i] == freq )
         {
            return 1;
         }
      }
   }
   
   return 0;
}

void displayAvailableFreqs(unsigned int coreID)
{
   assert(coreID < getCoreNumber());
   
   unsigned int i = 0;
   if ( pAvailableFreqsTable[coreID].pFreqs )
   {
      fprintf(stdout,"Frequencies for core %u : ",coreID);
      for ( i = 0 ; i < pAvailableFreqsTable[coreID].nbFreqs ; i++ )
      {
         fprintf(stdout,"%u ",pAvailableFreqsTable[coreID].pFreqs[i]);
      }
      fprintf(stdout,"\n");
   }
}
