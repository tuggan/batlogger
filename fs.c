/*
 *    Copyright 2022 Dennis Vesterlund
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

#include "fs.h"

#if _WIN32 || _WIN64
#define DIR_SEPARATOR '\\'
#endif

#if __GNUC__
#define DIR_SEPARATOR '/'
#include <linux/limits.h>
#endif

#ifndef DIR_SEPARATOR
#error "DIR_SEPARATOR must be defined"
#endif

#include <dirent.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char **init_folders() {
  char **folders = calloc(MAX_FOLDERS, sizeof(char*));
  for (int i = 0; i < MAX_FOLDERS; i++)
    folders[i] = calloc(sizeof(char), PATH_MAX);
  return folders;
}

int getFolders(char** folders, char *path) {
  DIR *d;
  struct dirent *dir;
  d = opendir(path);
  int loop = 0;
  if (d) {
    while ((dir = readdir(d)) != NULL) {
        if (dir->d_type == DT_LNK) {
          strncpy(folders[loop], dir->d_name, PATH_MAX);
        } else {
          continue;
        }
        loop++;
      }
      closedir(d);
  }
  return loop;
}

int getPrefixedFolders(char** folders, char *prefix, int prefixLen, char *path) {
  int num_folders = getFolders(folders, path);
  int matches = 0;
  for (int i = 0; i < num_folders; i++) {
    if (folders[i] == NULL)
      break;
    // Check if folder has prefix
    if (strncmp(prefix, folders[i], prefixLen) == 0){
      // Match!
      strncpy(folders[matches], folders[i], PATH_MAX);
      matches++;
    }
  }
  for(int i = matches; i < num_folders; i++) {
    memset(folders[i], '\0', PATH_MAX);
  }
  return matches;
}

void freeFolders(char** folders) {
    for (int i = 0; i < MAX_FOLDERS; i++)
      free(folders[i]);
  free(folders);
}

bool exists(char *path) {
  struct stat st;
  if (stat(path, &st) == 0) {
    return true;
  }
  return false;
}

bool is_folder(char *path) {
  struct stat st;
  if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
    return true;
  }
  return false;
}

bool is_file(char *path) {
  struct stat st;
  if (stat(path, &st) == 0 && S_ISREG(st.st_mode)) {
    return true;
  }
  return false;
}


char *join_path(char *start, char *end) {
  char *pathbuf = calloc(sizeof(char), PATH_MAX);

  if (start == NULL && end == NULL) { return pathbuf; }

  if (start == NULL || strlen(start) == 0) {
    strcpy(pathbuf, end);
    return pathbuf;
  }

  if (end == NULL || strlen(end) == 0) {
    strcpy(pathbuf, start);
    return pathbuf;
  }

  const char *last_char = start;
  while (*last_char != '\0')
    last_char++;
  int index = 0;
  strcpy(pathbuf, start);
  index = strlen(pathbuf);
  if (*last_char != DIR_SEPARATOR){
    pathbuf[index] = DIR_SEPARATOR;
    index++;
  }
  strcpy((pathbuf+index), end);
  return pathbuf;
}

size_t get_value_from_file(char *filepath, char *buffer, size_t buffsize) {
  FILE *f = fopen(filepath, "r");
  if (!f) return 0;
  size_t read = fread(buffer, sizeof(char), buffsize, f);
  fclose(f);
  return read;
}

int get_int_from_file(char *filepath) {
  char buf[BUF_INT];
  size_t read = get_value_from_file(filepath, buf, BUF_INT);
  if (read == 0) return 0;
  return atoi(buf);
}

long get_long_from_file(char *filepath) {
  char buf[BUF_LONG];
  size_t read = get_value_from_file(filepath, buf, BUF_LONG);
  if (read == 0) return 0;
  return atol(buf);
}

