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
long oldSize;

int main(int argc, char *argv[]) {
    bool daemonize;
    int opt;
    char logfile[] = LOGFILE, pidfile[] = PIDFILE, batfile[] = BATPATH;
    int sleeptime = SLEEPTIME;
    while((opt = getopt(argc, argv, "dS:P:b:")) != -1) {
        switch(opt) {
        case 'd':
            daemonize = true;
            break;
        case 'S':
            sleeptime = atoi(optarg);
            if(sleeptime < 1)
                sleeptime = SLEEPTIME;
            break;
        }
    }

    if(daemonize) {
        int pid = fork();
        if(pid == 0) {
            int fd = open("/dev/null", O_RDWR);
            if(fd == -1) {
                fprintf(stderr, "Could not open /dev/null "\
                        "for output redirecting.\n");
            } else {
                dup2(fd, STDIN_FILENO);
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
            }
            printf("I hope this worked!\n");
        } else {
            printf("Daemonized process.\n");
            exit(0);
        }
    }
    signal(SIGINT, sighandler);
    signal(SIGTERM, sighandler);
    signal(SIGCONT, sighandler);
    writePid(pidfile);
    logLoop(batfile, logfile, sleeptime * 1000L);
    deletePid(pidfile);
    return 0;
}

void logLoop(char *batfile, char *logfile, long sleeptime) {
    char batstr[10];
    long read, old[10];
    oldSize = 0;
    while(running) {
        read = getValueFromFile(batfile, batstr, 10);
        if(oldSize < 10)
            oldSize += 1;
        moveBackOld(old, oldSize);
        old[0] = atol(batstr);
        saveToFile(logfile, getAverage(old, oldSize));
        usleep(sleeptime);
    }
}

void moveBackOld(long *old, long size) {
    long i;
    for(i = size-1; i > 0; i--)
        old[i] = old[i-1];
}

long getAverage(long *values, long size) {
    long long sum = 0;
    long i;
    for(i = 0; i < size; i++)
        sum += values[i];
    long ret = sum/size;
    return ret;
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
    switch(signo) {
    case SIGTERM:
    case SIGINT:
        running = 0;
        break;
    case SIGCONT:
        oldSize = 0;
    }
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

char saveToFile(char *filename, long batcharge) {
    FILE *f = fopen(filename, "a+");
    if(f == NULL)
        return -1;

    time_t rawtime;
    struct tm *ti;

    time(&rawtime);
    ti = localtime(&rawtime);

    int ret = fprintf(f, "[%i-%0.2i-%0.2i %0.2i:%0.2i:%0.2i] %ld\n",
                      ti->tm_year+1900,
                      ti->tm_mon,
                      ti->tm_mday,
                      ti->tm_hour,
                      ti->tm_min,
                      ti->tm_sec,
                      batcharge);
    fclose(f);
    return ret;
}



















