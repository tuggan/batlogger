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

#ifndef BATTERY_H
#define BATTERY_H

#include <stdlib.h>
#include <stdbool.h>

#define BAT_PATH "/sys/class/power_supply"
// For testing TODO remove when user can specify path.
//#define BAT_PATH "/tmp/battest"
#define BAT_PREFIX "BAT"
#define BAT_PREFIX_LEN 3

// Battery files
#define BAT_CAPACITY           "capacity"
#define BAT_CYCLE_COUNT        "cycle_count"
#define BAT_ENERGY_FULL        "energy_full"
#define BAT_ENERGY_FULL_DESIGN "energy_full_design"
#define BAT_ENERGY_NOW         "energy_now"
#define BAT_POWER_NOW          "power_now"
#define BAT_VOLTAGE_MIN_DESIGN "voltage_min_design"
#define BAT_VOLTAGE_NOW        "voltage_now"
#define BAT_MANUFACTURER       "manufacturer"
#define BAT_MODEL_NAME         "model_name"
#define BAT_SERIAL_NUMBER      "serial_number"
#define BAT_TECHNOLOGY         "technology"
#define BAT_TYPE               "type"
#define BAT_STATUS             "status"

#define BAT_STR_SIZE 255

typedef struct battery {
  bool present;

  int capacity;

  long cycle_count;
  long energy_full;
  long energy_full_design;
  long energy_now;
  long power_now;
  long voltage_min_design;
  long voltage_now;

  char *name;
  char *path;
  char *manufacturer;
  size_t manufacturer_len;
  char *model_name;
  size_t model_name_len;
  char *serial_number;
  size_t serial_number_len;
  char *technology;
  size_t technology_len;
  char *type;
  size_t type_len;
  char *status;
  size_t status_len;
} battery;

typedef struct batteries {
  int num_batteries;
  struct battery **batteries;
} batteries;


struct batteries *get_batteries();
struct battery *initiate_battery(char *name);
void get_battery(struct battery *battery);
void free_batteries(struct batteries *batteries);
void free_battery(struct battery *battery);

#endif // BATTERY_H
