#include "unity.h"
#include "../../inc/logic/logic.h"
#include "../../inc/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void setUp(void) {
    // No setup needed
}

void tearDown(void) {
    // No teardown needed
}

// Helper function to get date string relative to today
static void get_relative_date(int days_offset, char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    tm_info->tm_mday += days_offset;
    mktime(tm_info); // Normalize the date
    strftime(buffer, size, "%d.%m.%Y", tm_info);
}

void test_logic_validate_player_profile_all_valid_returns_success(void) {
    // Arrange: All valid fields
    char future_date[11];
    get_relative_date(30, future_date, sizeof(future_date)); // 30 days in future
    
    // Act
    int result = logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john.doe@example.com",
        future_date,
        future_date,
        "true"
    );
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
}

void test_logic_validate_player_profile_empty_full_name_returns_empty_field(void) {
    char future_date[11];
    get_relative_date(30, future_date, sizeof(future_date));
    
    // Empty string
    TEST_ASSERT_EQUAL_INT(ERR_EMPTY_FIELD, logic_validate_player_profile(
        "",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        future_date,
        future_date,
        "true"
    ));
    
    // NULL
    TEST_ASSERT_EQUAL_INT(ERR_EMPTY_FIELD, logic_validate_player_profile(
        NULL,
        "johndoe123",
        "1234-567890",
        "john@example.com",
        future_date,
        future_date,
        "true"
    ));
    
    // Only spaces
    TEST_ASSERT_EQUAL_INT(ERR_EMPTY_FIELD, logic_validate_player_profile(
        "   ",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        future_date,
        future_date,
        "true"
    ));
}

void test_logic_validate_player_profile_empty_gamertag_returns_empty_field(void) {
    char future_date[11];
    get_relative_date(30, future_date, sizeof(future_date));
    
    TEST_ASSERT_EQUAL_INT(ERR_EMPTY_FIELD, logic_validate_player_profile(
        "John Doe",
        "",
        "1234-567890",
        "john@example.com",
        future_date,
        future_date,
        "true"
    ));
}

void test_logic_validate_player_profile_empty_ssn_returns_empty_field(void) {
    char future_date[11];
    get_relative_date(30, future_date, sizeof(future_date));
    
    TEST_ASSERT_EQUAL_INT(ERR_EMPTY_FIELD, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "",
        "john@example.com",
        future_date,
        future_date,
        "true"
    ));
}

void test_logic_validate_player_profile_empty_email_returns_empty_field(void) {
    char future_date[11];
    get_relative_date(30, future_date, sizeof(future_date));
    
    TEST_ASSERT_EQUAL_INT(ERR_EMPTY_FIELD, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "",
        future_date,
        future_date,
        "true"
    ));
}

void test_logic_validate_player_profile_empty_dates_returns_empty_field(void) {
    char future_date[11];
    get_relative_date(30, future_date, sizeof(future_date));
    
    // Empty start date
    TEST_ASSERT_EQUAL_INT(ERR_EMPTY_FIELD, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        "",
        future_date,
        "true"
    ));
    
    // Empty end date
    TEST_ASSERT_EQUAL_INT(ERR_EMPTY_FIELD, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        future_date,
        "",
        "true"
    ));
}

void test_logic_validate_player_profile_invalid_ssn_formats(void) {
    char future_date[11];
    get_relative_date(30, future_date, sizeof(future_date));
    
    // Wrong length - too short
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SSN, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "123-45678",
        "john@example.com",
        future_date,
        future_date,
        "true"
    ));
    
    // Wrong length - too long
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SSN, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "12345-6789012",
        "john@example.com",
        future_date,
        future_date,
        "true"
    ));
    
    // No hyphen
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SSN, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "12345678901",
        "john@example.com",
        future_date,
        future_date,
        "true"
    ));
    
    // Wrong hyphen position
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SSN, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "12345-67890",
        "john@example.com",
        future_date,
        future_date,
        "true"
    ));
    
    // Non-numeric characters
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SSN, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "ABCD-567890",
        "john@example.com",
        future_date,
        future_date,
        "true"
    ));
    
    // Space instead of hyphen
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SSN, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234 567890",
        "john@example.com",
        future_date,
        future_date,
        "true"
    ));
}

void test_logic_validate_player_profile_invalid_email_formats(void) {
    char future_date[11];
    get_relative_date(30, future_date, sizeof(future_date));
    
    // No @ symbol
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_EMAIL, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "johndoe.example.com",
        future_date,
        future_date,
        "true"
    ));
    
    // No dot after @
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_EMAIL, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example",
        future_date,
        future_date,
        "true"
    ));
    
    // Just @
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_EMAIL, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "@",
        future_date,
        future_date,
        "true"
    ));
    
    // Multiple @ symbols (still valid according to implementation)
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@@example.com",
        future_date,
        future_date,
        "true"
    ));
}

void test_logic_validate_player_profile_invalid_date_formats(void) {
    char future_date[11];
    get_relative_date(30, future_date, sizeof(future_date));
    
    // Wrong length
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_DATE, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        "1.1.2025",  // Too short
        future_date,
        "true"
    ));
    
    // Wrong separators
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_DATE, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        "01-01-2025",  // Hyphens instead of dots
        future_date,
        "true"
    ));
    
    // Non-numeric characters
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_DATE, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        "AB.CD.EFGH",
        future_date,
        "true"
    ));
    
    // Invalid end date format
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_DATE, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        future_date,
        "2025/12/31",  // Wrong format
        "true"
    ));
}

void test_logic_validate_player_profile_past_start_date_returns_past_date(void) {
    char past_date[11];
    char future_date[11];
    get_relative_date(-30, past_date, sizeof(past_date));    // 30 days in past
    get_relative_date(30, future_date, sizeof(future_date)); // 30 days in future
    
    TEST_ASSERT_EQUAL_INT(ERR_PAST_DATE, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        past_date,
        future_date,
        "true"
    ));
}

void test_logic_validate_player_profile_today_as_start_date_is_valid(void) {
    char today[11];
    char future_date[11];
    get_relative_date(0, today, sizeof(today));              // Today
    get_relative_date(30, future_date, sizeof(future_date)); // 30 days in future
    
    // Today is considered valid (>= 0 difference)
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        today,
        future_date,
        "true"
    ));
}

void test_logic_validate_player_profile_invalid_subscription_status(void) {
    char future_date[11];
    get_relative_date(30, future_date, sizeof(future_date));
    
    // Invalid values
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SUB_STATUS, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        future_date,
        future_date,
        "yes"
    ));
    
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SUB_STATUS, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        future_date,
        future_date,
        "1"
    ));
    
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SUB_STATUS, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        future_date,
        future_date,
        "0"
    ));
    
    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SUB_STATUS, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        future_date,
        future_date,
        ""
    ));
    
    // Valid values
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        future_date,
        future_date,
        "false"
    ));
}

void test_logic_validate_player_profile_special_characters_in_names(void) {
    char future_date[11];
    get_relative_date(30, future_date, sizeof(future_date));
    
    // Special characters are allowed in names and gamertags (no validation on content)
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, logic_validate_player_profile(
        "José María Müller-O'Brien",
        "gamer@123#special",
        "1234-567890",
        "john@example.com",
        future_date,
        future_date,
        "true"
    ));
}

void test_logic_validate_player_profile_edge_case_dates(void) {
    // Very far future date
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        "31.12.2099",
        "31.12.2099",
        "true"
    ));
    
    // Date at turn of century
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "1234-567890",
        "john@example.com",
        "01.01.2100",
        "01.01.2100",
        "true"
    ));
}

void test_logic_validate_player_profile_order_of_validation(void) {
    // Test that empty field is checked before format validation
    // Empty SSN should return ERR_EMPTY_FIELD, not ERR_INVALID_SSN
    char future_date[11];
    get_relative_date(30, future_date, sizeof(future_date));
    
    TEST_ASSERT_EQUAL_INT(ERR_EMPTY_FIELD, logic_validate_player_profile(
        "John Doe",
        "johndoe123",
        "",  // Empty SSN
        "john@example.com",
        future_date,
        future_date,
        "true"
    ));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_validate_player_profile_all_valid_returns_success);
    RUN_TEST(test_logic_validate_player_profile_empty_full_name_returns_empty_field);
    RUN_TEST(test_logic_validate_player_profile_empty_gamertag_returns_empty_field);
    RUN_TEST(test_logic_validate_player_profile_empty_ssn_returns_empty_field);
    RUN_TEST(test_logic_validate_player_profile_empty_email_returns_empty_field);
    RUN_TEST(test_logic_validate_player_profile_empty_dates_returns_empty_field);
    RUN_TEST(test_logic_validate_player_profile_invalid_ssn_formats);
    RUN_TEST(test_logic_validate_player_profile_invalid_email_formats);
    RUN_TEST(test_logic_validate_player_profile_invalid_date_formats);
    RUN_TEST(test_logic_validate_player_profile_past_start_date_returns_past_date);
    RUN_TEST(test_logic_validate_player_profile_today_as_start_date_is_valid);
    RUN_TEST(test_logic_validate_player_profile_invalid_subscription_status);
    RUN_TEST(test_logic_validate_player_profile_special_characters_in_names);
    RUN_TEST(test_logic_validate_player_profile_edge_case_dates);
    RUN_TEST(test_logic_validate_player_profile_order_of_validation);
    return UNITY_END();
}