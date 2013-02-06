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

#include "dumpResults.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

FILE* pDumpFile = NULL;

unsigned int* pBuf = NULL;
unsigned int bufCapacity = 0;
unsigned int bufSize = 0;


void openDump(const char* pFileName, unsigned int bufferSize)
{
   assert(pFileName);
   assert(!pDumpFile);
   
   pDumpFile = fopen(pFileName,"w+");
   if ( pDumpFile == NULL )
   {
      fprintf(stderr,"Error to open dump file %s\n",pFileName);
      return;
   }
   
   pBuf = (unsigned int*) malloc(sizeof(unsigned int) * bufferSize);
   if ( pBuf == NULL )
   {
      fprintf(stderr,"Fail to allocate memory for buffer\n");
      return;
   }
   
   bufCapacity = bufferSize;
   
   return;
}

// This function is not visible from outside
void flushDump()
{
   if ( pDumpFile == NULL || pBuf == NULL )
   {
      return;
   }
   
   unsigned int i = 0;
   for ( i = 0 ; i < bufSize ; i++ )
   {
      fprintf(pDumpFile,"%u\n",pBuf[i]);
   }
   
   bufSize = 0;
}

void writeDump(unsigned long time)
{
   if ( pBuf != NULL )
   {
      pBuf[bufSize] = time;
      bufSize++;
      if ( bufSize >= bufCapacity )
      {
         flushDump();
      }
   }
}

void resetDump()
{
   /*
    * For safety, I would keep it
   pDumpFile = freopen(NULL,"w+",pDumpFile);
   if ( pDumpFile == NULL )
   {
      fprintf(stderr,"Error to reopen the file\n");
   }
   */
   
   bufSize = 0;
}

void closeDump()
{
   flushDump();
   if ( pDumpFile )
   {
      fclose(pDumpFile);
   }
   free(pBuf);
}
