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

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define BUFFER_PATH_SIZE 100

#define CPU_PATH_FORMAT "/sys/devices/system/cpu/cpu%d/cpufreq/%s"

/**
 * Easy to use function to open any file related to a CPU core
 * (files are located in /sys/devices/system/cpu/cpu[coreID]/cpufreq/)
 * \param coreID the id of the core to look at
 * \param fileName the name of the file to open
 * \param mode the opening mode
 * \return the file
 * \warning don't forget to close the pointeur returned with fclose
 */
FILE* openCPUFreqFile(unsigned int coreID, const char* fileName, const char* mode);

/**
 * Pin the current program to a specific core using CPU_SET and
 * sched_setaffinity
 * \param cpu the core id to which one the function pin the program
 */
void pinCPU(int cpu);

#endif
