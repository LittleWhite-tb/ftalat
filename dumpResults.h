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

#ifndef DUMPRESULTS_H
#define DUMPRESULTS_H

/**
 * Open the dump system
 * Open the file where the output will be printed
 * Allocate memory for the buffer
 * \param pFilename the filename where to write datas
 * \param bufferSize the maximal size of the buffer
 */
void openDump(const char* pFileName, unsigned int bufferSize);

/**
 * Write the time in the dump system
 * Technically, the time is just stored in the buffer
 * When the buffer is full, all the data stored will be printed in the file
 * \param time the time to store
 */
void writeDump(unsigned long time);

/**
 * Reset the dump be removing all elements in the buffer and rewinding the file
 */
void resetDump();

/**
 * Closes and free the dump system
 */
void closeDump();

#endif
