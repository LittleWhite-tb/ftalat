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

#include "CoreRelation.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "FreqGetter.h"
#include "FreqSetter.h"
#include "utils.h"

/**
 * \struct CoreRelations 
 * \brief Internal struct to keep the information about cores relations
 * The structs contains all the core linked to one.
 */
typedef struct CoreRelations
{
   unsigned int nbRelations;  /*!< number of elements in coreIDs */
   unsigned int* coreIDs;     /*!< IDs related to the current core */
}CoreRelations;

CoreRelations* pRelationsTable = NULL;

void initCoreRelations()
{
   assert(pRelationsTable == NULL);
   
   unsigned int nbCore = getCoreNumber();
   unsigned int i = 0;
   
   // Allocate memory for the table
   pRelationsTable = (CoreRelations*) calloc(nbCore, sizeof(CoreRelations));
   if ( pRelationsTable == NULL )
   {
      fprintf(stderr,"Error to allocate memory for relation table\n");
      return;
   }
   
   for ( i = 0 ; i < nbCore ; i++ )
   {
      FILE* pRelationFile = openCPUFreqFile(i,"related_cpus","r");
      if ( pRelationFile )
      {
         size_t tabSize = nbCore+1;
         
         pRelationsTable[i].coreIDs = (unsigned int*) malloc(sizeof(unsigned int) * tabSize); // let's say 25 is enough for an init size
         if ( pRelationsTable[i].coreIDs != NULL )
         {
            unsigned int coreID = 0;
            size_t counter = 0;
            while(fscanf(pRelationFile,"%u",&coreID) == 1 )
            {
               pRelationsTable[i].coreIDs[counter] = coreID;
               counter++;
               if ( counter >= tabSize ) // Not enough space remaining
               {
                  // Double size
                  tabSize *= 2;
                  unsigned int* newFreqsTab = realloc(pRelationsTable[i].coreIDs,tabSize*sizeof(unsigned int));
                  if ( newFreqsTab != NULL )
                  {
                     pRelationsTable[i].coreIDs = newFreqsTab;
                  }
                  else
                  {
                     fprintf(stderr,"Fail to allocate more memory for relations table\n");
                     break;
                  }
               }
            }
            
            pRelationsTable[i].nbRelations = counter;
         }
         else
         {
            fprintf(stderr,"Fail to allocated memory for line of core relation table\n");
         }
         
         fclose(pRelationFile);
      }
   }
}

void freeCoreRelations()
{
   unsigned int nbCore = getCoreNumber();
   unsigned int i = 0;
   
   if ( pRelationsTable )
   {
      for ( i= 0 ; i < nbCore ; i++ )
      {
         free(pRelationsTable[i].coreIDs);
      }
   }
   
   free(pRelationsTable);
}

unsigned int getHeadCore(unsigned int coreID)
{
   assert(coreID < getCoreNumber());
   
   if ( pRelationsTable[coreID].coreIDs )
   {
      return pRelationsTable[coreID].coreIDs[0]; // I think the first is always master
   }
   
   return 0;
}

void setFreqForAllRelatedCore(unsigned int coreID, unsigned int freq)
{
   assert(coreID < getCoreNumber());
   
   if ( pRelationsTable[coreID].coreIDs )
   {
      unsigned int i = 0;
      for ( i = 0 ; i < pRelationsTable[coreID].nbRelations ; i++ )
      {
         setFreq(pRelationsTable[coreID].coreIDs[i],freq);
      }
   }
}

void displayCoreRelations(unsigned int coreID)
{
   assert(coreID < getCoreNumber());
   
   unsigned int i = 0;
   
   if ( pRelationsTable[coreID].coreIDs )
   {
      fprintf(stdout,"Relations for core %u : ",coreID);
      for ( i = 0 ; i < pRelationsTable[coreID].nbRelations ; i++ )
      {
         fprintf(stdout,"%u ",pRelationsTable[coreID].coreIDs[i]);
      }
      fprintf(stdout,"\n");
   }
}
