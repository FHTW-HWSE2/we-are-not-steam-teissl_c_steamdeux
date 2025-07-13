// test/test_data_remove_expired_users_invalid_cases.c

#define _XOPEN_SOURCE
#define USERS_JSON_PATH "test/users_invalid_cases.json"

#include "unity.h"
#include "cJSON.h"
#include "../../inc/data/data.h"
#include "../inc/error.h"

void setUp(void) {
    const char *filepath = USERS_JSON_PATH;

    cJSON *users = cJSON_CreateArray();

    // User mit missing sub_end
    cJSON *missing_end = cJSON_CreateObject();
    cJSON_AddStringToObject(missing_end, "full_name", "Missing End");
    cJSON_AddBoolToObject(missing_end, "is_subscribed", 0);
    cJSON_AddItemToArray(users, missing_end);

    // User mit missing sub_flag
    cJSON *missing_flag = cJSON_CreateObject();
    cJSON_AddStringToObject(missing_flag, "full_name", "Missing Flag");
    cJSON_AddStringToObject(missing_flag, "subscription_end_date", "01.01.2022");
    cJSON_AddItemToArray(users, missing_flag);

    // User mit invalid date format
    cJSON *invalid_date = cJSON_CreateObject();
    cJSON_AddStringToObject(invalid_date, "full_name", "Invalid Date");
    cJSON_AddStringToObject(invalid_date, "subscription_end_date", "invalid");
    cJSON_AddBoolToObject(invalid_date, "is_subscribed", 0);
    cJSON_AddItemToArray(users, invalid_date);

    // User mit flag=true (obwohl expired)
    cJSON *flag_true = cJSON_CreateObject();
    cJSON_AddStringToObject(flag_true, "full_name", "Flag True");
    cJSON_AddStringToObject(flag_true, "subscription_end_date", "01.01.2022");
    cJSON_AddBoolToObject(flag_true, "is_subscribed", 1);
    cJSON_AddItemToArray(users, flag_true);

    // User mit end <2 Jahre her (z.B. 01.01.2024)
    cJSON *recent_expired = cJSON_CreateObject();
    cJSON_AddStringToObject(recent_expired, "full_name", "Recent Expired");
    cJSON_AddStringToObject(recent_expired, "subscription_end_date", "01.01.2024");
    cJSON_AddBoolToObject(recent_expired, "is_subscribed", 0);
    cJSON_AddItemToArray(users, recent_expired);

    char *json_str = cJSON_Print(users);
    FILE *f = fopen(filepath, "w");
    if (f) {
        fputs(json_str, f);
        fclose(f);
    }
    free(json_str);
    cJSON_Delete(users);
}

void tearDown(void) {
    remove(USERS_JSON_PATH);
}

void test_data_remove_expired_users_keeps_invalid_cases(void) {
    int removed = -1;
    int result = data_remove_expired_users(&removed);

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_EQUAL_INT(0, removed);  // Keiner entfernt

    // Überprüfe, dass Datei unverändert (alle 5 User)
    cJSON *updated = load_json_from_file(USERS_JSON_PATH);
    TEST_ASSERT_NOT_NULL(updated);
    TEST_ASSERT_TRUE(cJSON_IsArray(updated));
    TEST_ASSERT_EQUAL_INT(5, cJSON_GetArraySize(updated));
    cJSON_Delete(updated);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_data_remove_expired_users_keeps_invalid_cases);
    return UNITY_END();
}