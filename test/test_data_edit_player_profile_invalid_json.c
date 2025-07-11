#define USERS_JSON_PATH "test/users_invalid_object.json"

#include "unity.h"
#include "../../inc/data/data.h"
#include "../inc/error.h"
#include <stdio.h>

void setUp(void) {
    FILE *f = fopen(USERS_JSON_PATH, "w");
    fputs("{\"not\": \"an array\"}", f);  // absichtlich kein Array!
    fclose(f);
}

void tearDown(void) {}

void test_data_edit_profile_invalid_json_returns_ERR_STORAGE_FAILURE(void) {
    int result = data_edit_player_profile("editme123", "X", "", "", "", "", -1);
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_data_edit_profile_invalid_json_returns_ERR_STORAGE_FAILURE);
    return UNITY_END();
}
