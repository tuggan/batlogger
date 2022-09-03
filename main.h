/*
 *    Copyright 2014,2022 Dennis Vesterlund
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef MAIN_H
#define MAIN_H

#define BATPATH "/sys/class/power_supply"
#define BATNOW "energy_now"
#define BATMAX "energy_full"
#define BATPREFIX "BAT"
#define LOGFILE "./bat.log"
#define PIDFILE "./run.pid"
#define SLEEPTIME 10000

#include "battery.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>


void printHelp(char *name);
void logLoop(char *logfile, long sleeptime);
void sighandler(int signo);
char saveToFile(char *filename, batteries *batteries);




#endif // MAIN_H
















