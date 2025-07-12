#define _XOPEN_SOURCE // Needed for strptime on Linux
#define USERS_JSON_PATH "test/users_create_duration.json"

#include "unity.h"
#include "../../inc/logic/logic.h"
#include "../../inc/data/data.h"
#include "../../inc/error.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void setUp(void) {
    // Clean up test file before each test
    remove(USERS_JSON_PATH);
}

void tearDown(void) {
    // Clean up test file after each test
    remove(USERS_JSON_PATH);
}

// Helper function to get date string relative to today
static void get_relative_date(int days_offset, char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    tm_info->tm_mday += days_offset;
    mktime(tm_info); // Normalize the date
    strftime(buffer, size, "%d.%m.%Y", tm_info);
}

// Helper function to verify user was saved correctly
static int verify_user_saved(const char* expected_gamertag, const char* expected_full_name, 
                           const char* expected_ssn, const char* expected_email, 
                           int expected_subscribed) {
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    if (!users || !cJSON_IsArray(users)) {
        if (users) cJSON_Delete(users);
        return 0;
    }
    
    int found = 0;
    for (int i = 0; i < cJSON_GetArraySize(users); i++) {
        cJSON *user = cJSON_GetArrayItem(users, i);
        cJSON *gamertag = cJSON_GetObjectItem(user, "gamertag");
        if (gamertag && strcmp(gamertag->valuestring, expected_gamertag) == 0) {
            found = 1;
            
            // Verify fields
            cJSON *full_name = cJSON_GetObjectItem(user, "full_name");
            TEST_ASSERT_EQUAL_STRING(expected_full_name, full_name->valuestring);
            
            cJSON *ssn = cJSON_GetObjectItem(user, "ssn");
            TEST_ASSERT_EQUAL_STRING(expected_ssn, ssn->valuestring);
            
            cJSON *email = cJSON_GetObjectItem(user, "email");
            TEST_ASSERT_EQUAL_STRING(expected_email, email->valuestring);
            
            cJSON *is_subscribed = cJSON_GetObjectItem(user, "is_subscribed");
            TEST_ASSERT_EQUAL_INT(expected_subscribed, is_subscribed->valueint);
            
            break;
        }
    }
    
    cJSON_Delete(users);
    return found;
}

void test_logic_create_user_with_duration_use_today_1_month(void) {
    // Act: Create user with today as start and 1 month duration
    int result = logic_create_user_with_duration(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john.doe@example.com",
        NULL,  // sub_start is ignored when use_today_as_start is 1
        "true",
        1,     // 1 month duration
        1      // use today as start
    );
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_TRUE(verify_user_saved("johndoe123", "John Doe", "1234-567890", "john.doe@example.com", 1));
    
    // Verify dates
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    cJSON *user = cJSON_GetArrayItem(users, 0);
    
    // Check start date is today
    char today[11];
    get_relative_date(0, today, sizeof(today));
    cJSON *start_date = cJSON_GetObjectItem(user, "subscription_start_date");
    TEST_ASSERT_EQUAL_STRING(today, start_date->valuestring);
    
    // Check end date is 1 month from today
    cJSON *end_date = cJSON_GetObjectItem(user, "subscription_end_date");
    TEST_ASSERT_NOT_NULL(end_date);
    
    // Parse dates to verify 1 month difference
    struct tm start_tm = {0}, end_tm = {0};
    strptime(start_date->valuestring, "%d.%m.%Y", &start_tm);
    strptime(end_date->valuestring, "%d.%m.%Y", &end_tm);
    
    // Should be 1 month difference (accounting for month wraparound)
    int expected_month = (start_tm.tm_mon + 1) % 12;
    int expected_year = start_tm.tm_year + ((start_tm.tm_mon + 1) / 12);
    TEST_ASSERT_EQUAL_INT(expected_month, end_tm.tm_mon);
    TEST_ASSERT_EQUAL_INT(expected_year, end_tm.tm_year);
    
    cJSON_Delete(users);
}

void test_logic_create_user_with_duration_use_today_6_months(void) {
    // Act: Create user with today as start and 6 months duration
    int result = logic_create_user_with_duration(
        "Jane Smith",
        "janesmith456",
        "2222-333333",
        "jane.smith@example.com",
        "ignored_date",  // Should be ignored
        "true",
        6,     // 6 months duration
        1      // use today as start
    );
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_TRUE(verify_user_saved("janesmith456", "Jane Smith", "2222-333333", "jane.smith@example.com", 1));
    
    // Verify 6 months duration
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    cJSON *user = cJSON_GetArrayItem(users, 0);
    
    struct tm start_tm = {0}, end_tm = {0};
    cJSON *start_date = cJSON_GetObjectItem(user, "subscription_start_date");
    cJSON *end_date = cJSON_GetObjectItem(user, "subscription_end_date");
    
    strptime(start_date->valuestring, "%d.%m.%Y", &start_tm);
    strptime(end_date->valuestring, "%d.%m.%Y", &end_tm);
    
    // Calculate expected end date
    int expected_month = (start_tm.tm_mon + 6) % 12;
    int expected_year = start_tm.tm_year + ((start_tm.tm_mon + 6) / 12);
    
    TEST_ASSERT_EQUAL_INT(expected_month, end_tm.tm_mon);
    TEST_ASSERT_EQUAL_INT(expected_year, end_tm.tm_year);
    
    cJSON_Delete(users);
}

void test_logic_create_user_with_duration_use_today_12_months(void) {
    // Act: Create user with today as start and 12 months duration
    int result = logic_create_user_with_duration(
        "Bob Johnson",
        "bobjohnson789",
        "3333-444444",
        "bob.johnson@example.com",
        NULL,
        "false",  // Not subscribed
        12,       // 12 months duration
        1         // use today as start
    );
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_TRUE(verify_user_saved("bobjohnson789", "Bob Johnson", "3333-444444", "bob.johnson@example.com", 0));
    
    // Verify 12 months (1 year) duration
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    cJSON *user = cJSON_GetArrayItem(users, 0);
    
    struct tm start_tm = {0}, end_tm = {0};
    cJSON *start_date = cJSON_GetObjectItem(user, "subscription_start_date");
    cJSON *end_date = cJSON_GetObjectItem(user, "subscription_end_date");
    
    strptime(start_date->valuestring, "%d.%m.%Y", &start_tm);
    strptime(end_date->valuestring, "%d.%m.%Y", &end_tm);
    
    // Should be exactly 1 year later
    TEST_ASSERT_EQUAL_INT(start_tm.tm_mon, end_tm.tm_mon);
    TEST_ASSERT_EQUAL_INT(start_tm.tm_year + 1, end_tm.tm_year);
    TEST_ASSERT_EQUAL_INT(start_tm.tm_mday, end_tm.tm_mday);
    
    cJSON_Delete(users);
}

void test_logic_create_user_with_duration_custom_start_date(void) {
    // Arrange: Custom future start date
    char future_start[11];
    get_relative_date(30, future_start, sizeof(future_start)); // 30 days from now
    
    // Act: Create user with custom start date and 3 months duration
    int result = logic_create_user_with_duration(
        "Alice Cooper",
        "alicecooper321",
        "4444-555555",
        "alice.cooper@example.com",
        future_start,
        "true",
        3,     // 3 months duration
        0      // use provided start date
    );
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_TRUE(verify_user_saved("alicecooper321", "Alice Cooper", "4444-555555", "alice.cooper@example.com", 1));
    
    // Verify custom start date was used
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    cJSON *user = cJSON_GetArrayItem(users, 0);
    cJSON *start_date = cJSON_GetObjectItem(user, "subscription_start_date");
    TEST_ASSERT_EQUAL_STRING(future_start, start_date->valuestring);
    
    cJSON_Delete(users);
}

void test_logic_create_user_with_duration_past_start_date_returns_error(void) {
    // Arrange: Past start date
    char past_start[11];
    get_relative_date(-30, past_start, sizeof(past_start)); // 30 days ago
    
    // Act: Try to create user with past start date
    int result = logic_create_user_with_duration(
        "Past User",
        "pastuser123",
        "5555-666666",
        "past.user@example.com",
        past_start,
        "true",
        1,
        0      // use provided start date
    );
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_PAST_DATE, result);
    
    // Verify user was not saved
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    TEST_ASSERT_NULL(users); // File should not exist
}

void test_logic_create_user_with_duration_invalid_start_date_format(void) {
    // Act: Try to create user with invalid date format
    int result = logic_create_user_with_duration(
        "Invalid Date User",
        "invaliddate123",
        "6666-777777",
        "invalid.date@example.com",
        "2025-01-01",  // Wrong format (should be DD.MM.YYYY)
        "true",
        1,
        0
    );
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_DATE, result);
}

void test_logic_create_user_with_duration_year_wraparound(void) {
    // Arrange: Start date in November
    char november_date[11] = "15.11.2025";
    
    // Act: Create user with 3 months duration (should wrap to next year)
    int result = logic_create_user_with_duration(
        "Year Wrap User",
        "yearwrap123",
        "7777-888888",
        "yearwrap@example.com",
        november_date,
        "true",
        3,     // November + 3 months = February next year
        0
    );
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify year wraparound
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    cJSON *user = cJSON_GetArrayItem(users, 0);
    cJSON *end_date = cJSON_GetObjectItem(user, "subscription_end_date");
    
    // Should be February 15, 2026
    TEST_ASSERT_EQUAL_STRING("15.02.2026", end_date->valuestring);
    
    cJSON_Delete(users);
}

void test_logic_create_user_with_duration_validation_errors_propagate(void) {
    char future_date[11];
    get_relative_date(30, future_date, sizeof(future_date));
    
    // Test empty field error
    TEST_ASSERT_EQUAL_INT(ERR_EMPTY_FIELD, logic_create_user_with_duration(
        "",  // Empty name
        "test123",
        "1234-567890",
        "test@example.com",
        future_date,
        "true",
        1,
        0
    ));
    
    // Test invalid SSN
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SSN, logic_create_user_with_duration(
        "Test User",
        "test123",
        "invalid-ssn",
        "test@example.com",
        future_date,
        "true",
        1,
        0
    ));
    
    // Test invalid email
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_EMAIL, logic_create_user_with_duration(
        "Test User",
        "test123",
        "1234-567890",
        "invalid.email",  // No @
        future_date,
        "true",
        1,
        0
    ));
    
    // Test invalid subscription status
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SUB_STATUS, logic_create_user_with_duration(
        "Test User",
        "test123",
        "1234-567890",
        "test@example.com",
        future_date,
        "invalid",  // Not "true" or "false"
        1,
        0
    ));
}

void test_logic_create_user_with_duration_february_edge_cases(void) {
    // Test with a future date in January
    char feb_date[11];
    get_relative_date(180, feb_date, sizeof(feb_date));  // 180 days in future (should be safe)
    
    // Extract just the day and month, set to January 29
    struct tm future_tm = {0};
    strptime(feb_date, "%d.%m.%Y", &future_tm);
    future_tm.tm_mon = 0;  // January
    future_tm.tm_mday = 29;
    strftime(feb_date, sizeof(feb_date), "%d.%m.%Y", &future_tm);
    
    // Act: Add 1 month (should become February)
    int result = logic_create_user_with_duration(
        "February User",
        "febuser123",
        "8888-999999",
        "feb@example.com",
        feb_date,
        "true",
        1,
        0
    );
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    cJSON *user = cJSON_GetArrayItem(users, 0);
    cJSON *end_date = cJSON_GetObjectItem(user, "subscription_end_date");
    
    // mktime normalizes dates, so Jan 29 + 1 month = Feb 29 (or March 1 if not leap year)
    struct tm end_tm = {0};
    strptime(end_date->valuestring, "%d.%m.%Y", &end_tm);
    
    // In 2025 (not a leap year), Feb would have 28 days
    // mktime should normalize this correctly
    TEST_ASSERT_TRUE(end_tm.tm_mon == 1 || end_tm.tm_mon == 2); // February or March
    
    cJSON_Delete(users);
}

void test_logic_create_user_with_duration_large_duration(void) {
    // Use a future date to avoid past date error
    char start_date[11];
    get_relative_date(30, start_date, sizeof(start_date));  // 30 days in future
    
    // Act: Create user with 24 months (2 years) duration
    int result = logic_create_user_with_duration(
        "Long Sub User",
        "longsub123",
        "9999-000000",
        "longsub@example.com",
        start_date,
        "true",
        24,    // 24 months
        0
    );
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify 2 years were added
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    cJSON *user = cJSON_GetArrayItem(users, 0);
    
    struct tm start_tm = {0}, end_tm = {0};
    cJSON *start = cJSON_GetObjectItem(user, "subscription_start_date");
    cJSON *end = cJSON_GetObjectItem(user, "subscription_end_date");
    
    strptime(start->valuestring, "%d.%m.%Y", &start_tm);
    strptime(end->valuestring, "%d.%m.%Y", &end_tm);
    
    // Should be 2 years difference
    TEST_ASSERT_EQUAL_INT(start_tm.tm_mon, end_tm.tm_mon);
    TEST_ASSERT_EQUAL_INT(start_tm.tm_year + 2, end_tm.tm_year);
    
    cJSON_Delete(users);
}

void test_logic_create_user_with_duration_zero_months(void) {
    // Use a future date
    char start_date[11];
    get_relative_date(60, start_date, sizeof(start_date));  // 60 days in future
    
    // Act: Create user with 0 months duration
    int result = logic_create_user_with_duration(
        "Zero Duration User",
        "zerodur123",
        "1111-222222",
        "zero@example.com",
        start_date,
        "true",
        0,     // 0 months - end date should equal start date
        0
    );
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify end date equals start date
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    cJSON *user = cJSON_GetArrayItem(users, 0);
    
    cJSON *start = cJSON_GetObjectItem(user, "subscription_start_date");
    cJSON *end = cJSON_GetObjectItem(user, "subscription_end_date");
    
    TEST_ASSERT_EQUAL_STRING(start->valuestring, end->valuestring);
    
    cJSON_Delete(users);
}

void test_logic_create_user_with_duration_special_characters(void) {
    // Use a future date
    char future_date[11];
    get_relative_date(90, future_date, sizeof(future_date));  // 90 days in future
    
    // Act: Create user with special characters in fields
    int result = logic_create_user_with_duration(
        "José María O'Connor-Smith",
        "spëcîál_üsér123",
        "1234-567890",
        "josé.maría@ñoño-domain.com",
        future_date,
        "true",
        6,
        0
    );
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_TRUE(verify_user_saved(
        "spëcîál_üsér123", 
        "José María O'Connor-Smith", 
        "1234-567890", 
        "josé.maría@ñoño-domain.com", 
        1
    ));
}

void test_logic_create_user_with_duration_multiple_users(void) {
    // Use future dates
    char date1[11], date2[11];
    get_relative_date(30, date1, sizeof(date1));   // 30 days in future
    get_relative_date(60, date2, sizeof(date2));   // 60 days in future
    
    // Create first user
    int result1 = logic_create_user_with_duration(
        "First User",
        "first123",
        "1111-111111",
        "first@example.com",
        date1,
        "true",
        1,
        0
    );
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result1);
    
    // Create second user
    int result2 = logic_create_user_with_duration(
        "Second User",
        "second456",
        "2222-222222",
        "second@example.com",
        date2,
        "false",
        3,
        0
    );
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result2);
    
    // Verify both users exist
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    TEST_ASSERT_EQUAL_INT(2, cJSON_GetArraySize(users));
    
    // Verify both users have correct data
    TEST_ASSERT_TRUE(verify_user_saved("first123", "First User", "1111-111111", "first@example.com", 1));
    TEST_ASSERT_TRUE(verify_user_saved("second456", "Second User", "2222-222222", "second@example.com", 0));
    
    cJSON_Delete(users);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_create_user_with_duration_use_today_1_month);
    RUN_TEST(test_logic_create_user_with_duration_use_today_6_months);
    RUN_TEST(test_logic_create_user_with_duration_use_today_12_months);
    RUN_TEST(test_logic_create_user_with_duration_custom_start_date);
    RUN_TEST(test_logic_create_user_with_duration_past_start_date_returns_error);
    RUN_TEST(test_logic_create_user_with_duration_invalid_start_date_format);
    RUN_TEST(test_logic_create_user_with_duration_year_wraparound);
    RUN_TEST(test_logic_create_user_with_duration_validation_errors_propagate);
    RUN_TEST(test_logic_create_user_with_duration_february_edge_cases);
    RUN_TEST(test_logic_create_user_with_duration_large_duration);
    RUN_TEST(test_logic_create_user_with_duration_zero_months);
    RUN_TEST(test_logic_create_user_with_duration_special_characters);
    RUN_TEST(test_logic_create_user_with_duration_multiple_users);
    return UNITY_END();
}