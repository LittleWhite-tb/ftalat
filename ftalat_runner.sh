#!/bin/bash

# ftalat - Frequency Transition Latency Estimator
# Copyright (C) 2013 Universite de Versailles
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Script to bench all the frequencies couple with ftalat

OUTPUT_FILE=results
OUTPUT_EXT=.txt
OUTPUT_DIR=Results
FREQUENCIES=`cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies | tr " " "\n" | sort -g | sed '1d'`
ITER=31

mkdir -p $OUTPUT_DIR

# Check if CPU supports turbo
if [[ -n "$(cat /proc/cpuinfo | grep "flags" | grep "ida")" ]]
then
   cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies | cut -d " " -f 1 > $OUTPUT_DIR/turbo
   
else
   echo 0 > $OUTPUT_DIR/turbo
fi

for startFreq in ${FREQUENCIES}
do
   # Avoid turbo as starting seq
   if [ `cat $OUTPUT_DIR/turbo` -ne $startFreq ]
   then
      for testFreq in ${FREQUENCIES}
      do
         # Avoid turbo as starting seq
         if [ $startFreq -eq $testFreq ] 
         then
            echo "Skip $startFreq -> $testFreq (same frequency)"
            continue
         fi

         # Avoid turbo as starting seq
         if [ `cat $OUTPUT_DIR/turbo` -eq $testFreq ] 
         then
            echo "Skip $startFreq -> $testFreq (turbo)"
            continue
         fi

         echo "$startFreq -> $testFreq"
         # Test if the result file already exists
         if ( ! (test -e "${OUTPUT_DIR}/${OUTPUT_FILE}"_"${startFreq}"-"${testFreq}${OUTPUT_EXT}" ) ) 
         then
            for i in `seq $ITER`
               do
                  ./ftalat $startFreq $testFreq >> ${OUTPUT_DIR}/${OUTPUT_FILE}"_"${startFreq}"-"${testFreq}${OUTPUT_EXT}
                 echo "############################" >> ${OUTPUT_DIR}/${OUTPUT_FILE}"_"${startFreq}"-"${testFreq}${OUTPUT_EXT}
            done
         fi
      done
   fi
done
