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

# return True if the CPU has turbo
def hasTurbo():
   cpuInfoFile = open("/proc/cpuinfo")
   fileLines = cpuInfoFile.readlines()
   for line in fileLines:
      if line.find("flags") != -1:
         if line.find(" ida ") != -1:
            return True
   return False
   
# return the highest frequency available on this CPU
def getHighestFreq(coreID):
   cpuInfoFile = open("/sys/devices/system/cpu/cpu{0}/cpufreq/scaling_available_frequencies".format(coreID))
   return int(cpuInfoFile.read().split(" ")[0])
