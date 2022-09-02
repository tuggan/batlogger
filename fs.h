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

#ifndef FS_H
#define FS_H
// Check windows
#if _WIN32 || _WIN64
  #if _WIN64
    #define ENV64BIT
  #else
    #define ENV32BIT
  #endif
#endif

// Check GCC
#if __GNUC__
  #if __x86_64__ || __ppc64__
    #define ENV64BIT
  #else
    #define ENV32BIT
  #endif
#endif

#if defined(ENV64BIT)
#define BUF_INT 10
#define BUF_LONG 20
#elif defined(ENV32BIT)
#define BUF_INT 10
#define BUF_LONG 10
#else
#error "Must define either ENV32BIT or ENV64BIT"
#endif

#define MAX_FOLDERS 10

#include <stdio.h>
#include <stdbool.h>

char **init_folders();
int getFolders(char **folders, char *path);
int getPrefixedFolders(char **folders, char *prefix, int prefixLen, char *path);
void freeFolders(char **folders);
bool exists(char *path);
bool is_folder(char *path);
bool is_file(char *path);
char *join_path(char *start, char *end);
size_t get_value_from_file(char *filepath, char *buffer, size_t buffsize);
int get_int_from_file(char *filepath);
long get_long_from_file(char *filepath);

#endif // FS_H
