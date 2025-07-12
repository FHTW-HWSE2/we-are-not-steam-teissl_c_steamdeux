#include <stdio.h>
#include <assert.h>
#include "../src/simulation/simulation_helpers.h"
#include "../cJSON/cJSON.h"

void test_calculate_days() {
    assert(calculate_days(1, 5) == 5);
    assert(calculate_days(2, 3) == 21);
    assert(calculate_days(3, 2) == 60);
    assert(calculate_days(99, 10) == -1);
    printf("✅ test_calculate_days passed\n");
}

void test_simulate_sessions() {
    cJSON *users = cJSON_CreateArray();
    cJSON *games = cJSON_CreateArray();

    cJSON *user = cJSON_CreateObject();
    cJSON_AddStringToObject(user, "gamertag", "TestUser");
    cJSON_AddItemToArray(users, user);

    cJSON *game = cJSON_CreateObject();
    cJSON_AddStringToObject(game, "title", "TestGame");
    cJSON_AddItemToArray(games, game);

    cJSON *reports = simulate_sessions(users, games, time(NULL), 1);
    assert(cJSON_GetArraySize(reports) == 1);
    cJSON *session = cJSON_GetArrayItem(reports, 0);
    assert(cJSON_GetObjectItem(session, "gamertag"));
    assert(cJSON_GetObjectItem(session, "game"));
    assert(cJSON_GetObjectItem(session, "duration_minutes"));

    cJSON_Delete(users);
    cJSON_Delete(games);
    cJSON_Delete(reports);
    printf("✅ test_simulate_sessions passed\n");
}

int main() {
    test_calculate_days();
    test_simulate_sessions();
    return 0;
}
