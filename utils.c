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

#include "utils.h"

#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>
#include <sched.h>

FILE* openCPUFreqFile(unsigned int coreID, const char* fileName, const char* mode)
{
   char filePathBuffer[BUFFER_PATH_SIZE]= {'\0'};
   snprintf(filePathBuffer,BUFFER_PATH_SIZE,CPU_PATH_FORMAT,coreID,fileName);
   
   FILE* pFile = fopen(filePathBuffer,mode);
   if ( pFile == NULL )
   {
      fprintf(stderr,"Fail to open %s\n",filePathBuffer);
   }
   
   return pFile;
}

void pinCPU(int cpu)
{
   cpu_set_t cpuset;
   pid_t myself = getpid();
 
   CPU_ZERO(&cpuset);
   CPU_SET(cpu,&cpuset);
 
   int ret = sched_setaffinity(myself, sizeof(cpu_set_t), &cpuset);
   if(ret != 0)
   {
      perror("sched_setaffinity");
      exit(0);
   }
}
