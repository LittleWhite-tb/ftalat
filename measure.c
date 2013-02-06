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

#include "measure.h"

#include <stdio.h>

void displayMeasure(Measure measure)
{
   fprintf(stdout, "\t=====\n");
   fprintf(stdout, "Min Time : %llu\n",measure.min);
   fprintf(stdout, "Max Time : %llu\n",measure.max);
   fprintf(stdout, "Medium : %llu\n",measure.medium);
   fprintf(stdout, "\t=====\n");
}
