#include "simulation_helpers.h"
#include <stdlib.h>
#include <string.h>

int calculate_days(int choice, int duration) {
    switch (choice) {
        case 1: return duration;
        case 2: return duration * 7;
        case 3: return duration * 30;
        default: return -1;
    }
}

cJSON *simulate_sessions(cJSON *users, cJSON *games, time_t now, int total_days) {
    cJSON *report_arr = cJSON_CreateArray();
    srand(42);  // Für reproduzierbare Tests

    for (int i = 0; i < cJSON_GetArraySize(users); ++i) {
        cJSON *user = cJSON_GetArrayItem(users, i);
        const char *tag = cJSON_GetObjectItem(user, "gamertag")->valuestring;

        for (int j = 0; j < cJSON_GetArraySize(games); ++j) {
            cJSON *game = cJSON_GetArrayItem(games, j);
            const char *title = cJSON_GetObjectItem(game, "title")->valuestring;

            cJSON *session = cJSON_CreateObject();
            cJSON_AddStringToObject(session, "gamertag", tag);
            cJSON_AddStringToObject(session, "game", title);
            cJSON_AddNumberToObject(session, "duration_minutes", 60);

            cJSON_AddItemToArray(report_arr, session);
        }
    }

    return report_arr;
}
