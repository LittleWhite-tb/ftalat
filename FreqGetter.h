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

#ifndef FREQGETTER_H
#define FREQGETTER_H

/**
 * Get the number of online cores
 * \return
 */
unsigned int getCoreNumber();

/**
 * Read the frequencies available for each cores
 */
void initFreqInfo();

/**
 */
void freeFreqInfo();

/**
 * Get the current frequency for core identified by \a coreID
 * \param coreID core identifier 
 * \return the frequency actually set
 */
unsigned int getCurFreq(unsigned int coreID);

/**
 * Wait the core identified by \a coreID to be at \a targetFreq frequency
 * \param coreID core identifier 
 * \param targetFreq
 */
inline void waitCurFreq(unsigned int coreID, unsigned int targetFreq);

/**
 * Get the minimum frequency available for the core identified by \a coreID
 * \param coreID core identifier
 * \return
 */
unsigned int getMinAvailableFreq(unsigned int coreID);

/**
 * Get the maximum frequency available for the core identified by \a coreID
 * \param coreID core identifier
 * \return
 */
unsigned int getMaxAvailableFreq(unsigned int coreID);

/**
 * \return 1 the freq is available for this core, 0 if not
 */
int isFreqAvailable(unsigned int coreID, unsigned int freq);

/**
 * Debug function to nicely print available frequencies for the core
 * identified by \a coreID
 */
void displayAvailableFreqs(unsigned int coreID);

#endif
