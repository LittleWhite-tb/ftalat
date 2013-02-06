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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* Compute the average sample execution time */
double average(unsigned int n, unsigned long *times)
{
	unsigned int i = 0;
	double averageTime = 0;

	for ( i = 0 ; i < n ; i++ )
	{
		averageTime += times[i];;	
	}
	
	averageTime /= n;

	return averageTime;
}

/* Compute the sample standard deviation */
double sd(unsigned int n, double average, unsigned long *times)
{
	unsigned int i  = 0;
	double variance = 0, sum;

	for ( i = 0 ; i < n ; i++ )
	{
		sum = times[i] - average;
		variance += sum * sum;	
	}

	variance = variance / (n-1) ;
	/* sample standard deviation is equal the square root of the variance */
	variance = sqrt(variance);
	return variance;
}

void confidenceInterval(unsigned int n, 
						double average, 
						double sd, 
						unsigned long *lowBoundTime, 
						unsigned long *highBoundTime 
						)
{
   // Zp for 99.9% confidence
   //float z_value = 3.291;
   // Zp for 99% confidence
   float z_value = 2.346;
   // Zp for 97.5% confidence
   //float z_value = 2.257;
	// A Zp value for 95% confidence level
	//float z_value = 1.960;
   // at 90% confidence
   // float z_value = 1.645;
   // at 85% confidence
   // float z_value = 1.440;
   // at 80% confidence
   // float z_value = 1.282;
   // at 50%
   //float z_value = 0.674;

	double standardError = 0.0;

	standardError = (z_value * sd) / sqrt(n);

   //printf("stderr = %f\n", standardError);
	
	*lowBoundTime  = (unsigned long int) floor(average - standardError);
	*highBoundTime = (unsigned long int) ceil(average + standardError);
}

int comparer(const void* a, const void* b)
{
   unsigned int uiA = *((unsigned int*)a);
   unsigned int uiB = *((unsigned int*)b);
   
   if ( uiA < uiB )
   {
      return -1;
   }
   else if ( uiA == uiB )
   {
      return 0;
   }
   // else
   return 1;
}

void interQuartileRange(unsigned int n, 
						unsigned long *times,
						unsigned long *lowBoundTime, 
						unsigned long *highBoundTime 
						)
{
	unsigned int minIndex, maxIndex;

   qsort(times,n,sizeof(unsigned long),comparer); 

	minIndex = (unsigned int) ceil(n / 4); // First quartile index
	maxIndex = (unsigned int) floor(3 * n / 4); // Third quartile index

	*lowBoundTime  = times[minIndex] ;
	*highBoundTime = times[maxIndex] ;
   
   assert(*lowBoundTime <= *highBoundTime);
}

