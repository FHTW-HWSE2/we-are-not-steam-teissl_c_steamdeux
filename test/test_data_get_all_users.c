#define USERS_JSON_PATH "test/users_get.json"

#include "unity.h"
#include "cJSON.h"
#include "../../inc/data/data.h"
#include "../inc/error.h"
#include <stdio.h>
#include <stdlib.h>

void setUp(void) {
    // default: leeres valides Array reinschreiben
    FILE *f = fopen(USERS_JSON_PATH, "w");
    fputs("[]", f);
    fclose(f);
}

void tearDown(void) {}


// === Test: gültiges Array mit zwei Usern ===
void test_data_get_all_users_valid_file_returns_array(void) {
    FILE *f = fopen(USERS_JSON_PATH, "w");
    fputs("[{\"gamertag\":\"user1\"}, {\"gamertag\":\"user2\"}]", f);
    fclose(f);

    cJSON *result_array = NULL;
    int result = data_get_all_users(&result_array);

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(result_array);
    TEST_ASSERT_TRUE(cJSON_IsArray(result_array));
    TEST_ASSERT_EQUAL_INT(2, cJSON_GetArraySize(result_array));

    cJSON_Delete(result_array);
}


// === Test: ungültige JSON-Datei (kein Array) ===
void test_data_get_all_users_invalid_file_returns_empty_array(void) {
    FILE *f = fopen(USERS_JSON_PATH, "w");
    fputs("{\"not\": \"an array\"}", f); // absichtlich kein Array
    fclose(f);

    cJSON *result_array = NULL;
    int result = data_get_all_users(&result_array);

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(result_array);
    TEST_ASSERT_TRUE(cJSON_IsArray(result_array));
    TEST_ASSERT_EQUAL_INT(0, cJSON_GetArraySize(result_array));

    cJSON_Delete(result_array);
}


int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_data_get_all_users_valid_file_returns_array);
    RUN_TEST(test_data_get_all_users_invalid_file_returns_empty_array);

    return UNITY_END();
}
