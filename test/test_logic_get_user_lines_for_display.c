// test/test_logic_get_user_lines_for_display.c

#include "unity.h"
#include "../../cJSON/cJSON.h"
#include "../mocks/mock_data.h"
#include "../../inc/logic/logic.h"  // For declaration of logic_get_user_lines_for_display

void setUp(void) {
    // Reset mocks before each test
    mock_data_get_all_users_called = 0;
    mock_data_get_all_users_return = ERR_SUCCESS;
    mock_data_get_all_users_users_out = NULL;
}

void tearDown(void) {
    // NICHT nochmal freigeben, wenn logic das schon erledigt
    mock_data_get_all_users_users_out = NULL;
}

// Helper to create a test user JSON object
cJSON* create_test_user(const char* full_name, const char* gamertag, const char* email, const char* ssn, int is_subscribed, const char* sub_start, const char* sub_end, int player_hours) {
    cJSON* user = cJSON_CreateObject();
    cJSON_AddStringToObject(user, "full_name", full_name);
    cJSON_AddStringToObject(user, "gamertag", gamertag);
    cJSON_AddStringToObject(user, "email", email);
    cJSON_AddStringToObject(user, "ssn", ssn);
    cJSON_AddBoolToObject(user, "is_subscribed", is_subscribed);
    cJSON_AddStringToObject(user, "subscription_start_date", sub_start);
    cJSON_AddStringToObject(user, "subscription_end_date", sub_end);
    cJSON_AddNumberToObject(user, "player_hours", player_hours);
    return user;
}

// Helper to free the lines_out array
void free_lines(char** lines, int count) {
    for (int i = 0; i < count; i++) {
        free(lines[i]);
    }
    free(lines);
}

// Test: data_get_all_users fails (returns error code)
void test_logic_get_user_lines_for_display_data_fail_returns_error(void) {
    mock_data_get_all_users_return = ERR_STORAGE_FAILURE;
    char** lines = NULL;
    int count = 0;
    int result = logic_get_user_lines_for_display(&lines, &count);
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
    TEST_ASSERT_NULL(lines);
    TEST_ASSERT_EQUAL_INT(0, count);
    TEST_ASSERT_EQUAL_INT(1, mock_data_get_all_users_called);
    mock_data_get_all_users_users_out = NULL;  // Prevent double-free in tearDown
}

// Test: data_get_all_users returns NULL users_out
void test_logic_get_user_lines_for_display_null_users_returns_error(void) {
    mock_data_get_all_users_users_out = NULL;
    char** lines = NULL;
    int count = 0;
    int result = logic_get_user_lines_for_display(&lines, &count);
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
    TEST_ASSERT_NULL(lines);
    TEST_ASSERT_EQUAL_INT(0, count);
    mock_data_get_all_users_users_out = NULL;  // Prevent double-free in tearDown
}

// Test: Empty array (count=0)
void test_logic_get_user_lines_for_display_empty_array_returns_success_zero_count(void) {
    mock_data_get_all_users_users_out = cJSON_CreateArray();
    char** lines = NULL;
    int count = 0;
    int result = logic_get_user_lines_for_display(&lines, &count);
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_EQUAL_INT(0, count);
    free_lines(lines, count);  // Safe even if lines NULL or count 0
    mock_data_get_all_users_users_out = NULL;  // Prevent double-free in tearDown
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_get_user_lines_for_display_data_fail_returns_error);
    RUN_TEST(test_logic_get_user_lines_for_display_null_users_returns_error);
    RUN_TEST(test_logic_get_user_lines_for_display_empty_array_returns_success_zero_count);
    return UNITY_END();
}