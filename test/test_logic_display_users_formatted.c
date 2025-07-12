#include "unity.h"
#include "../../cJSON/cJSON.h"
#include "../mocks/mock_data.h"
#include "mock_presentation.h"  // Assumes mock_presentation.c is updated with the mocks below
#include "../../inc/logic/logic.h"  // For logic_display_users_formatted and helpers
#include "mock_presentation.h"

void setUp(void) {
// Reset mocks before each test
mock_data_get_all_users_called = 0;
mock_data_get_all_users_return = ERR_SUCCESS;
mock_data_get_all_users_users_out = NULL;

mock_presentation_show_error_called = 0;
if (mock_presentation_show_error_message) {
free(mock_presentation_show_error_message);
mock_presentation_show_error_message = NULL;
}

mock_presentation_display_users_formatted_called = 0;
if (mock_presentation_display_users_formatted_lines) {
for (int i = 0; i < mock_presentation_display_users_formatted_count; ++i) {
free(mock_presentation_display_users_formatted_lines[i]);
}
free(mock_presentation_display_users_formatted_lines);
mock_presentation_display_users_formatted_lines = NULL;
}
mock_presentation_display_users_formatted_count = 0;
}

void tearDown(void) {
// Cleanup any leftover JSON/mocks
if (mock_data_get_all_users_users_out) {
cJSON_Delete(mock_data_get_all_users_users_out);
mock_data_get_all_users_users_out = NULL;
}
if (mock_presentation_show_error_message) {
free(mock_presentation_show_error_message);
mock_presentation_show_error_message = NULL;
}
if (mock_presentation_display_users_formatted_lines) {
for (int i = 0; i < mock_presentation_display_users_formatted_count; ++i) {
free(mock_presentation_display_users_formatted_lines[i]);
}
free(mock_presentation_display_users_formatted_lines);
mock_presentation_display_users_formatted_lines = NULL;
}
}

// Helper to create a test user JSON object (copied/adapted from test_logic_get_top_users.c)
cJSON* create_test_user(const char* gamertag, int player_hours) {
cJSON* user = cJSON_CreateObject();
cJSON_AddStringToObject(user, "gamertag", gamertag);
cJSON_AddNumberToObject(user, "player_hours", player_hours);
cJSON_AddStringToObject(user, "full_name", "Test User");
cJSON_AddStringToObject(user, "ssn", "1234-567890");
cJSON_AddStringToObject(user, "email", "test@example.com");
cJSON_AddStringToObject(user, "subscription_start_date", "01.01.2025");
cJSON_AddStringToObject(user, "subscription_end_date", "01.01.2026");
cJSON_AddBoolToObject(user, "is_subscribed", 1);
return user;
}

// Test: data_get_all_users fails (returns error code) -> shows error, no display
void test_logic_display_users_formatted_data_fail_shows_error(void) {
mock_data_get_all_users_return = ERR_STORAGE_FAILURE;

logic_display_users_formatted();

TEST_ASSERT_EQUAL_INT(1, mock_data_get_all_users_called);
TEST_ASSERT_EQUAL_INT(1, mock_presentation_show_error_called);
TEST_ASSERT_EQUAL_STRING("Could not load users.", mock_presentation_show_error_message);
TEST_ASSERT_EQUAL_INT(0, mock_presentation_display_users_formatted_called);
}

// Test: data_get_all_users returns NULL users_out -> shows error, no display
void test_logic_display_users_formatted_null_users_shows_error(void) {
mock_data_get_all_users_users_out = NULL;  // SUCCESS but NULL out

logic_display_users_formatted();

TEST_ASSERT_EQUAL_INT(1, mock_data_get_all_users_called);
TEST_ASSERT_EQUAL_INT(1, mock_presentation_show_error_called);
TEST_ASSERT_EQUAL_STRING("Could not load users.", mock_presentation_show_error_message);
TEST_ASSERT_EQUAL_INT(0, mock_presentation_display_users_formatted_called);
}

void test_logic_display_users_formatted_successful_display(void) {
    // Setup: Erstelle ein valides User-Array mit einem User
    mock_data_get_all_users_users_out = cJSON_CreateArray();
    cJSON* user = cJSON_CreateObject();
    cJSON_AddStringToObject(user, "full_name", "Test User");
    cJSON_AddStringToObject(user, "gamertag", "test123");
    cJSON_AddStringToObject(user, "email", "test@example.com");
    cJSON_AddStringToObject(user, "ssn", "1234-567890");
    cJSON_AddBoolToObject(user, "is_subscribed", 1);
    cJSON_AddStringToObject(user, "subscription_start_date", "01.01.2025");
    cJSON_AddStringToObject(user, "subscription_end_date", "01.01.2026");
    cJSON_AddNumberToObject(user, "player_hours", 10);
    cJSON_AddItemToArray(mock_data_get_all_users_users_out, user);

    logic_display_users_formatted();

    // Prüfung: Funktion wurde aufgerufen und hat korrekt durchlaufen
    TEST_ASSERT_EQUAL_INT(1, mock_data_get_all_users_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_show_error_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_display_users_formatted_called);
    TEST_ASSERT_TRUE(mock_presentation_display_users_formatted_count > 0);  // sollte Zeilen enthalten

    // Kein double-free
    mock_data_get_all_users_users_out = NULL;
}


int main(void) {
UNITY_BEGIN();
RUN_TEST(test_logic_display_users_formatted_data_fail_shows_error);
RUN_TEST(test_logic_display_users_formatted_null_users_shows_error);
RUN_TEST(test_logic_display_users_formatted_successful_display);
return UNITY_END();
}