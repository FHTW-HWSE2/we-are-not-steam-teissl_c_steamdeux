#ifndef SIMULATION_HELPERS_H
#define SIMULATION_HELPERS_H

#include <time.h>
#include "cJSON.h"

int calculate_days(int choice, int duration);
cJSON *simulate_sessions(cJSON *users, cJSON *games, time_t now, int total_days);

#endif
