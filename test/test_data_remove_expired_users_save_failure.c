// test/test_data_remove_expired_users_save_failure.c

#define _XOPEN_SOURCE
#define USERS_JSON_PATH "test/users_save_fail.json"

#include "unity.h"
#include "cJSON.h"
#include "../../inc/data/data.h"
#include "../inc/error.h"
#include <sys/stat.h>  // Für chmod

void setUp(void) {
    const char *filepath = USERS_JSON_PATH;

    // Erzeuge JSON mit expired User
    cJSON *users = cJSON_CreateArray();
    cJSON *expired = cJSON_CreateObject();
    cJSON_AddStringToObject(expired, "full_name", "Expired User");
    cJSON_AddStringToObject(expired, "subscription_end_date", "01.01.2022");
    cJSON_AddBoolToObject(expired, "is_subscribed", 0);
    cJSON_AddItemToArray(users, expired);

    char *json_str = cJSON_Print(users);
    FILE *f = fopen(filepath, "w");
    if (f) {
        fputs(json_str, f);
        fclose(f);
    }
    free(json_str);
    cJSON_Delete(users);

    // Setze Datei read-only (simuliert save-fail)
    chmod(filepath, 0444);  // Read-only für alle
}

void tearDown(void) {
    // Revert read-only
    chmod(USERS_JSON_PATH, 0644);
    remove(USERS_JSON_PATH);
}

void test_data_remove_expired_users_save_failure(void) {
    int removed = -1;
    int result = data_remove_expired_users(&removed);

    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
    TEST_ASSERT_EQUAL_INT(1, removed);  // Removed gezählt, aber save fail
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_data_remove_expired_users_save_failure);
    return UNITY_END();
}