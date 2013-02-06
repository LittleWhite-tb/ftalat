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

# Calculate the median value of an array
def median(values):
   sortedValues = sorted(values)
   
   length = len(sortedValues)
   midLength = length / 2
   
   if length % 2 == 1: # odd, so the can get directly the value
      return sortedValues[midLength]
   else:
      return (sortedValues[midLength-1]+sortedValues[midLength]) / 2
