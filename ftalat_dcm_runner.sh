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
FREQUENCIES=`seq 1 1 16`
ITER=31

mkdir -p $OUTPUT_DIR

for startFreq in ${FREQUENCIES}
do
   # Avoid turbo as starting seq
      for testFreq in ${FREQUENCIES}
      do
         if [ $startFreq -eq $testFreq ] 
         then
            echo "Skip $startFreq -> $testFreq (same frequency)"
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
         setDutyCycle-16.exe
      done
done
