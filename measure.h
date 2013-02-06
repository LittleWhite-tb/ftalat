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

#ifndef MEASURE_H
#define MEASURE_H

/**
 * \struct Measure
 * Simple structure to keep track of measure
 */
typedef struct Measure
{
   unsigned long long min; /*!< */
   unsigned long long max; /*!< */
   unsigned long long medium; /*!< */
}Measure;

/**
 * Display on stdout the \a Measure
 * \param measure the measure to display
 */
void displayMeasure(Measure measure);

#endif
