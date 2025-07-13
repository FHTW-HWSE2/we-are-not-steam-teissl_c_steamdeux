#define _XOPEN_SOURCE
#define USERS_JSON_PATH "test/users_all_valid.json"

#include "unity.h"
#include "cJSON.h"
#include "../../inc/data/data.h"
#include "mocks/mock_data.h"
#include "../inc/error.h"

void setUp(void) {
    const char *filepath = USERS_JSON_PATH;

    // Erzeuge JSON mit einem gültigen User (Abo endet in der Zukunft)
    cJSON *users = cJSON_CreateArray();

    cJSON *active = cJSON_CreateObject();
    cJSON_AddStringToObject(active, "full_name", "Active User");
    cJSON_AddStringToObject(active, "gamertag", "active123");
    cJSON_AddNumberToObject(active, "player_hours", 0);
    cJSON_AddStringToObject(active, "ssn", "1234-567890");
    cJSON_AddStringToObject(active, "email", "active@example.com");
    cJSON_AddStringToObject(active, "subscription_start_date", "01.01.2023");
    cJSON_AddStringToObject(active, "subscription_end_date", "01.01.2030"); // Zukunft
    cJSON_AddBoolToObject(active, "is_subscribed", 1);
    cJSON_AddItemToArray(users, active);

    char *json_str = cJSON_Print(users);
    FILE *f = fopen(filepath, "w");
    fputs(json_str, f);
    fclose(f);
    free(json_str);
    cJSON_Delete(users);
}

void tearDown(void) {}

void test_data_remove_expired_users_keeps_valid_users(void) {
    int removed = -1;
    int result = data_remove_expired_users(&removed);

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_EQUAL_INT(0, removed);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_data_remove_expired_users_keeps_valid_users);
    return UNITY_END();
}
