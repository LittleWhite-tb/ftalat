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

#ifndef CORERELATION_H
#define CORERELATION_H

/**
 * Read the cpufreq files to find the relations between cores
 */
void initCoreRelations();

/**
 * Free the memmory used for core relations
 */
void freeCoreRelations();

/**
 * Get the head (or directing) core for \a coreID
 * \param coreID the core id
 * \return the coreID directing \a coreID
 */
unsigned int getHeadCore(unsigned int coreID);

/**
 * Change the frequency for all cores of the chip (determined by the relations
 * net)
 * \param coreID the id of the core to change
 * \param freq the new freq to set
 */
void setFreqForAllRelatedCore(unsigned int coreID, unsigned int freq);

/**
 * Debug function to display the cores linked to the identified by \a coredID
 */
void displayCoreRelations(unsigned int coreID);

#endif
