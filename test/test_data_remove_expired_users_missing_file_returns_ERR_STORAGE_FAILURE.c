#define _XOPEN_SOURCE
#define USERS_JSON_PATH "test/missing_users.json" // Muss vor Include kommen

#include "unity.h"
#include "cJSON.h"
#include "../../inc/data/data.h"
#include "mock_data.h"
#include "../inc/error.h"

void setUp(void) {
    remove(USERS_JSON_PATH); // sicherstellen, dass Datei nicht existiert
}

void tearDown(void) {}

void test_data_remove_expired_users_missing_file_returns_ERR_STORAGE_FAILURE(void) {
    int removed = -1;
    int result = data_remove_expired_users(&removed);

    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
    TEST_ASSERT_EQUAL_INT(0, removed);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_data_remove_expired_users_missing_file_returns_ERR_STORAGE_FAILURE);
    return UNITY_END();
}
