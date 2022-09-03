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

#include "main.h"

int running = 1;

int main(int argc, char *argv[]) {
    int opt;
    char logfile[] = LOGFILE;
    int sleeptime = SLEEPTIME;
    while((opt = getopt(argc, argv, "hS:")) != -1) {
        switch(opt) {
        case 'h':
          printHelp(argv[0]);
          exit(0);
        case 'S':
            sleeptime = atoi(optarg);
            if(sleeptime < 1)
                sleeptime = SLEEPTIME;
            break;
        }
    }

    signal(SIGINT, sighandler);
    signal(SIGTERM, sighandler);
    signal(SIGCONT, sighandler);
    logLoop(logfile, sleeptime * 1000L);
    return 0;
}

void printHelp(char *name) {
  printf("%s [hdS]\n\
    Log the battery level to a file.\n\n\
    Options:\n\
        -h          : Print this help.\n\
        -S <seconds>: Time to sleep in seconds?\n", name);
}

void logLoop(char *logfile, long sleeptime) {
    batteries *batteries = get_batteries();
    while(running) {
        saveToFile(logfile, batteries);
        usleep(sleeptime/10);
    }
    free_batteries(batteries);
}

void sighandler(int signo) {
    switch(signo) {
    case SIGTERM:
    case SIGINT:
        running = 0;
        break;
    case SIGCONT:
        break;
    }
}

char saveToFile(char *filename, batteries *batteries) {
    FILE *f = fopen(filename, "a+");
    if(f == NULL)
        return -1;

    time_t rawtime;
    struct tm *ti;

    time(&rawtime);
    ti = localtime(&rawtime);

    int ret = fprintf(f, "[%i-%0.2i-%0.2i %0.2i:%0.2i:%0.2i] ",
                      ti->tm_year+1900,
                      ti->tm_mon,
                      ti->tm_mday,
                      ti->tm_hour,
                      ti->tm_min,
                      ti->tm_sec);
    for (int i = 0; i < batteries->num_batteries; i++) {
      battery *bat = batteries->batteries[i];
      ret = fprintf(f, "%s %d %ld %s ", bat->name, bat->capacity, bat->energy_full, bat->status);

    }
    fprintf(f, "\n");

    fclose(f);
    return ret;
}


