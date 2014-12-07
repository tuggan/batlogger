/*
 *    Copyright 2014 Dennis Vesterlund
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
    bool daemonize;
    int opt;
    while((opt = getopt(argc, argv, "d")) != -1) {
        switch(opt) {
        case 'd':
            daemonize = true;
        }
    }

    if(daemonize && daemon(1, 1) != 0) {
        fprintf(stderr,"Could not daemonize process!");
        exit(1);
    }
    signal(SIGINT, sighandler);
    writePid(PIDFILE);
    logLoop();
    deletePid(PIDFILE);
    return 0;
}

void logLoop() {
    char batstr[10];
    long read;
    while(running) {
        read = getValueFromFile(BATPATH, batstr, 10);
        batstr[read-1] = '\0';
        saveToFile(LOGFILE, batstr);
        usleep(SLEEPTIME);
    }
}

size_t getValueFromFile(char *filepath, char *buffer, size_t buffsize) {
    FILE *f = fopen(filepath, "r");
    if(!f)
        return 0;
    size_t read = fread(buffer, sizeof(char), buffsize, f);
    fclose(f);
    return read;
}

void sighandler(int signo) {
    running = 0;
}

void writePid(char *file) {
    FILE *f = fopen(file, "w");
    if(f != NULL) {
        fprintf(f, "%i", getpid());
        fclose(f);
    }
}

void deletePid(char *file) {
    remove(file);
}

char saveToFile(char *filename, char *batstr) {
    FILE *f = fopen(filename, "a+");
    if(f == NULL)
        return -1;

    time_t rawtime;
    struct tm *ti;

    time(&rawtime);
    ti = localtime(&rawtime);

    int ret = fprintf(f, "[%i-%0.2i-%0.2i %0.2i:%0.2i:%0.2i] %s\n",
                      ti->tm_year+1900,
                      ti->tm_mon,
                      ti->tm_mday,
                      ti->tm_hour,
                      ti->tm_min,
                      ti->tm_sec,
                      batstr);
    fclose(f);
    return ret;
}



















