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

#ifndef _RDTSC_H_
#define _RDTSC_H_

// Define rdtscll for x86_64 arch
#ifdef __x86_64__

#define warmup_cpuid()\
   do {\
    unsigned int cycles_low, cycles_high;\
    asm volatile ("CPUID\n\t"\
     "RDTSC\n\t"\
     "mov %%edx, %0\n\t"\
     "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");\
    asm volatile("RDTSCP\n\t"\
     "mov %%edx, %0\n\t"\
     "mov %%eax, %1\n\t"\
     "CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");\
    asm volatile ("CPUID\n\t"\
     "RDTSC\n\t"\
     "mov %%edx, %0\n\t"\
     "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");\
    asm volatile("RDTSCP\n\t"\
     "mov %%edx, %0\n\t"\
     "mov %%eax, %1\n\t"\
     "CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");\
   } while(0)


#define sync_rdtsc1(val) \
   do {\
      unsigned int cycles_low, cycles_high;\
      asm volatile ("CPUID\n\t"\
      "RDTSC\n\t"\
      "mov %%edx, %0\n\t"\
      "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx"); \
      (val) = ((unsigned long) cycles_low) | (((unsigned long) cycles_high) << 32);\
   } while (0)

#define sync_rdtsc2(val) \
   do {\
      unsigned int cycles_low, cycles_high;\
      asm volatile("RDTSCP\n\t"\
      "mov %%edx, %0\n\t"\
      "mov %%eax, %1\n\t"\
      "CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");\
      (val) = ((unsigned long) cycles_low) | (((unsigned long) cycles_high) << 32);\
   } while (0)

#else
#error "Precise time stamp counter reading not implemented for this architecture"
#endif

#endif

