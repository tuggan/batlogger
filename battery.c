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

#include "battery.h"
#include "fs.h"

#include <stddef.h>
#include <string.h>


struct batteries *get_batteries() {
  char **folders = init_folders();
  int num_folders = getPrefixedFolders(folders, BAT_PREFIX, BAT_PREFIX_LEN, BAT_PATH);
  struct batteries *batteries = calloc(sizeof(struct batteries), 1);

  batteries->num_batteries = num_folders;
  batteries->batteries = calloc(sizeof(struct battery*), batteries->num_batteries);

  for (int i = 0; i < batteries->num_batteries; i++) {
    if (folders[i] == NULL)
      break;
    batteries->batteries[i] = initiate_battery(folders[i]);
  }

  freeFolders(folders);

  return batteries;
}

struct battery *initiate_battery(char *battery_name) {
  struct battery *battery = calloc(sizeof(struct battery), 1);
  battery->name = calloc(sizeof(char), BAT_STR_SIZE);
  battery->manufacturer = calloc(sizeof(char), BAT_STR_SIZE);
  battery->model_name = calloc(sizeof(char), BAT_STR_SIZE);
  battery->serial_number = calloc(sizeof(char), BAT_STR_SIZE);
  battery->technology = calloc(sizeof(char), BAT_STR_SIZE);
  battery->type = calloc(sizeof(char), BAT_STR_SIZE);
  battery->status = calloc(sizeof(char), BAT_STR_SIZE);
  strncpy(battery->name, battery_name, BAT_STR_SIZE);
  battery->path = join_path(BAT_PATH, battery_name);
  return battery;
}

void get_battery(struct battery *battery) {
  // Check that the battery is initialized.
  if (battery == NULL || strlen(battery->name) < 1) { return; }

  // Check if the battery path exists.
  if (!is_folder(battery->path)) {
    battery->present = false;
    // TODO Empty battery parameters
    return;
  }

  battery->capacity = get_int(battery->path, BAT_CAPACITY);
  battery->cycle_count = get_long(battery->path, BAT_CYCLE_COUNT);
  battery->energy_full = get_long(battery->path, BAT_ENERGY_FULL);
  battery->energy_full_design = get_long(battery->path, BAT_ENERGY_FULL_DESIGN);
  battery->energy_now = get_long(battery->path, BAT_ENERGY_NOW);
  battery->power_now = get_long(battery->path, BAT_POWER_NOW);
  battery->voltage_min_design = get_long(battery->path, BAT_VOLTAGE_MIN_DESIGN);
  battery->voltage_now = get_long(battery->path, BAT_VOLTAGE_NOW);

  battery->manufacturer_len =
      get_str(battery->manufacturer, battery->manufacturer_len, battery->path, BAT_MANUFACTURER);
  battery->model_name_len =
      get_str(battery->model_name, battery->model_name_len, battery->path, BAT_MODEL_NAME);
  battery->serial_number_len =
      get_str(battery->serial_number, battery->serial_number_len, battery->path, BAT_SERIAL_NUMBER);
  battery->technology_len =
      get_str(battery->technology, battery->technology_len, battery->path, BAT_TECHNOLOGY);
  battery->type_len =
      get_str(battery->type, battery->type_len, battery->path, BAT_TYPE);
  battery->status_len =
    get_str(battery->status, battery->status_len, battery->path, BAT_STATUS);

}

static int get_int(char *path, char *filename) {
  char *cur_path = join_path(path, filename);
  int ret = (is_file(cur_path)) ? get_int_from_file(cur_path) : 0;
  free(cur_path);
  return ret;
}

static long get_long(char *path, char *filename) {
  char *cur_path = join_path(path, filename);
  long ret = (is_file(cur_path)) ? get_long_from_file(cur_path) : 0;
  free(cur_path);
  return ret;
}

// TODO what if file does not exists?
static size_t get_str(char *buf, size_t len, char *path, char *filename) {
  size_t out_len = len;
  char *cur_path = join_path(path, filename);
  out_len =
      get_value_from_file(cur_path, buf, BAT_STR_SIZE);
  if (buf[out_len - 1] == '\n') {
    buf[out_len - 1] = '\0';
    out_len--;
  }
  free(cur_path);
  return out_len;
}

void free_batteries(struct batteries *batteries) {
  for (int i = 0; i < batteries->num_batteries; i++)
    free_battery(batteries->batteries[i]);
  free(batteries->batteries);
  free(batteries);
}

void free_battery(struct battery *battery) {
  free(battery->name);
  free(battery->manufacturer);
  free(battery->model_name);
  free(battery->serial_number);
  free(battery->technology);
  free(battery->type);
  free(battery->path);
  free(battery->status);
  free(battery);
}
