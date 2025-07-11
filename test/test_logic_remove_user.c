#define USERS_JSON_PATH "test/users_remove_test.json"

#include "unity.h"
#include "../../inc/logic/logic.h"
#include "../../inc/data/data.h"
#include "../../inc/error.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setUp(void) {
    // Clean up test file before each test
    remove(USERS_JSON_PATH);
}

void tearDown(void) {
    // Clean up test file after each test
    remove(USERS_JSON_PATH);
}

// Helper function to create test users file
static void create_test_users_file(const char *json_content) {
    FILE *f = fopen(USERS_JSON_PATH, "w");
    fputs(json_content, f);
    fclose(f);
}

// Helper function to verify user exists
static int user_exists(const char *gamertag) {
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    if (!users || !cJSON_IsArray(users)) {
        if (users) cJSON_Delete(users);
        return 0;
    }
    
    int found = 0;
    for (int i = 0; i < cJSON_GetArraySize(users); i++) {
        cJSON *user = cJSON_GetArrayItem(users, i);
        cJSON *tag = cJSON_GetObjectItem(user, "gamertag");
        if (tag && strcmp(tag->valuestring, gamertag) == 0) {
            found = 1;
            break;
        }
    }
    
    cJSON_Delete(users);
    return found;
}

// Helper function to count users
static int count_users(void) {
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    if (!users || !cJSON_IsArray(users)) {
        if (users) cJSON_Delete(users);
        return 0;
    }
    
    int count = cJSON_GetArraySize(users);
    cJSON_Delete(users);
    return count;
}

void test_logic_remove_user_success_single_user(void) {
    // Arrange: Create file with single user
    const char *json = "["
        "{"
            "\"full_name\": \"John Doe\","
            "\"gamertag\": \"johndoe123\","
            "\"player_hours\": 100,"
            "\"ssn\": \"1234-567890\","
            "\"email\": \"john@example.com\","
            "\"subscription_start_date\": \"01.01.2025\","
            "\"subscription_end_date\": \"01.07.2025\","
            "\"is_subscribed\": true"
        "}"
    "]";
    create_test_users_file(json);
    
    // Verify user exists
    TEST_ASSERT_TRUE(user_exists("johndoe123"));
    
    // Act
    int result = logic_remove_user("johndoe123");
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_FALSE(user_exists("johndoe123"));
    TEST_ASSERT_EQUAL_INT(0, count_users());
}

void test_logic_remove_user_success_from_multiple_users(void) {
    // Arrange: Create file with multiple users
    const char *json = "["
        "{"
            "\"full_name\": \"User One\","
            "\"gamertag\": \"user1\","
            "\"player_hours\": 100,"
            "\"ssn\": \"1111-111111\","
            "\"email\": \"user1@example.com\","
            "\"subscription_start_date\": \"01.01.2025\","
            "\"subscription_end_date\": \"01.07.2025\","
            "\"is_subscribed\": true"
        "},"
        "{"
            "\"full_name\": \"User Two\","
            "\"gamertag\": \"user2\","
            "\"player_hours\": 200,"
            "\"ssn\": \"2222-222222\","
            "\"email\": \"user2@example.com\","
            "\"subscription_start_date\": \"01.02.2025\","
            "\"subscription_end_date\": \"01.08.2025\","
            "\"is_subscribed\": true"
        "},"
        "{"
            "\"full_name\": \"User Three\","
            "\"gamertag\": \"user3\","
            "\"player_hours\": 300,"
            "\"ssn\": \"3333-333333\","
            "\"email\": \"user3@example.com\","
            "\"subscription_start_date\": \"01.03.2025\","
            "\"subscription_end_date\": \"01.09.2025\","
            "\"is_subscribed\": false"
        "}"
    "]";
    create_test_users_file(json);
    
    // Act: Remove middle user
    int result = logic_remove_user("user2");
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_TRUE(user_exists("user1"));
    TEST_ASSERT_FALSE(user_exists("user2"));
    TEST_ASSERT_TRUE(user_exists("user3"));
    TEST_ASSERT_EQUAL_INT(2, count_users());
}

void test_logic_remove_user_not_found(void) {
    // Arrange: Create file with users
    const char *json = "["
        "{"
            "\"full_name\": \"John Doe\","
            "\"gamertag\": \"johndoe123\","
            "\"player_hours\": 100,"
            "\"ssn\": \"1234-567890\","
            "\"email\": \"john@example.com\","
            "\"subscription_start_date\": \"01.01.2025\","
            "\"subscription_end_date\": \"01.07.2025\","
            "\"is_subscribed\": true"
        "}"
    "]";
    create_test_users_file(json);
    
    // Act: Try to remove non-existent user
    int result = logic_remove_user("nonexistent");
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_USER_NOT_FOUND, result);
    TEST_ASSERT_TRUE(user_exists("johndoe123")); // Original user still exists
    TEST_ASSERT_EQUAL_INT(1, count_users());
}

void test_logic_remove_user_empty_gamertag(void) {
    // Arrange: Create file with users
    const char *json = "["
        "{"
            "\"full_name\": \"John Doe\","
            "\"gamertag\": \"johndoe123\","
            "\"player_hours\": 100,"
            "\"ssn\": \"1234-567890\","
            "\"email\": \"john@example.com\","
            "\"subscription_start_date\": \"01.01.2025\","
            "\"subscription_end_date\": \"01.07.2025\","
            "\"is_subscribed\": true"
        "}"
    "]";
    create_test_users_file(json);
    
    // Act: Try to remove with empty gamertag
    int result = logic_remove_user("");
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_USER_NOT_FOUND, result);
    TEST_ASSERT_EQUAL_INT(1, count_users()); // No users removed
}

void test_logic_remove_user_case_sensitive(void) {
    // Arrange: Create file with user
    const char *json = "["
        "{"
            "\"full_name\": \"John Doe\","
            "\"gamertag\": \"JohnDoe123\","
            "\"player_hours\": 100,"
            "\"ssn\": \"1234-567890\","
            "\"email\": \"john@example.com\","
            "\"subscription_start_date\": \"01.01.2025\","
            "\"subscription_end_date\": \"01.07.2025\","
            "\"is_subscribed\": true"
        "}"
    "]";
    create_test_users_file(json);
    
    // Act: Try to remove with different case
    int result = logic_remove_user("johndoe123"); // lowercase instead of JohnDoe123
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_USER_NOT_FOUND, result);
    TEST_ASSERT_TRUE(user_exists("JohnDoe123")); // Original still exists
}

void test_logic_remove_user_special_characters_gamertag(void) {
    // Arrange: Create user with special characters in gamertag
    const char *json = "["
        "{"
            "\"full_name\": \"Special User\","
            "\"gamertag\": \"sp€cîál_üsér@123\","
            "\"player_hours\": 100,"
            "\"ssn\": \"1234-567890\","
            "\"email\": \"special@example.com\","
            "\"subscription_start_date\": \"01.01.2025\","
            "\"subscription_end_date\": \"01.07.2025\","
            "\"is_subscribed\": true"
        "}"
    "]";
    create_test_users_file(json);
    
    // Act
    int result = logic_remove_user("sp€cîál_üsér@123");
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_FALSE(user_exists("sp€cîál_üsér@123"));
    TEST_ASSERT_EQUAL_INT(0, count_users());
}

void test_logic_remove_user_from_empty_array(void) {
    // Arrange: Create empty array
    create_test_users_file("[]");
    
    // Act
    int result = logic_remove_user("anyuser");
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_USER_NOT_FOUND, result);
    TEST_ASSERT_EQUAL_INT(0, count_users());
}

void test_logic_remove_user_missing_file(void) {
    // Arrange: Ensure file doesn't exist
    remove(USERS_JSON_PATH);
    
    // Act
    int result = logic_remove_user("anyuser");
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
}

void test_logic_remove_user_invalid_json(void) {
    // Arrange: Create invalid JSON
    create_test_users_file("{\"not\": \"an array\"}");
    
    // Act
    int result = logic_remove_user("anyuser");
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
}

void test_logic_remove_user_remove_first_user(void) {
    // Arrange: Create multiple users
    const char *json = "["
        "{\"gamertag\": \"first\", \"full_name\": \"First User\"},"
        "{\"gamertag\": \"second\", \"full_name\": \"Second User\"},"
        "{\"gamertag\": \"third\", \"full_name\": \"Third User\"}"
    "]";
    create_test_users_file(json);
    
    // Act: Remove first user
    int result = logic_remove_user("first");
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_FALSE(user_exists("first"));
    TEST_ASSERT_TRUE(user_exists("second"));
    TEST_ASSERT_TRUE(user_exists("third"));
    TEST_ASSERT_EQUAL_INT(2, count_users());
}

void test_logic_remove_user_remove_last_user(void) {
    // Arrange: Create multiple users
    const char *json = "["
        "{\"gamertag\": \"first\", \"full_name\": \"First User\"},"
        "{\"gamertag\": \"second\", \"full_name\": \"Second User\"},"
        "{\"gamertag\": \"third\", \"full_name\": \"Third User\"}"
    "]";
    create_test_users_file(json);
    
    // Act: Remove last user
    int result = logic_remove_user("third");
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_TRUE(user_exists("first"));
    TEST_ASSERT_TRUE(user_exists("second"));
    TEST_ASSERT_FALSE(user_exists("third"));
    TEST_ASSERT_EQUAL_INT(2, count_users());
}

void test_logic_remove_user_duplicate_gamertags_removes_first(void) {
    // Arrange: Create users with duplicate gamertags
    const char *json = "["
        "{"
            "\"full_name\": \"First Duplicate\","
            "\"gamertag\": \"duplicate123\","
            "\"player_hours\": 100"
        "},"
        "{"
            "\"full_name\": \"Second Duplicate\","
            "\"gamertag\": \"duplicate123\","
            "\"player_hours\": 200"
        "},"
        "{"
            "\"full_name\": \"Third User\","
            "\"gamertag\": \"unique456\","
            "\"player_hours\": 300"
        "}"
    "]";
    create_test_users_file(json);
    
    // Act: Remove duplicate gamertag
    int result = logic_remove_user("duplicate123");
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_EQUAL_INT(2, count_users());
    
    // Verify the first duplicate was removed (second one remains)
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    cJSON *remaining_dup = cJSON_GetArrayItem(users, 0);
    cJSON *full_name = cJSON_GetObjectItem(remaining_dup, "full_name");
    TEST_ASSERT_EQUAL_STRING("Second Duplicate", full_name->valuestring);
    cJSON_Delete(users);
}

void test_logic_remove_user_preserves_other_user_data(void) {
    // Arrange: Create detailed user data
    const char *json = "["
        "{"
            "\"full_name\": \"User To Keep\","
            "\"gamertag\": \"keeper\","
            "\"player_hours\": 999,"
            "\"ssn\": \"1234-567890\","
            "\"email\": \"keeper@example.com\","
            "\"subscription_start_date\": \"01.06.2025\","
            "\"subscription_end_date\": \"01.12.2025\","
            "\"is_subscribed\": true"
        "},"
        "{"
            "\"full_name\": \"User To Remove\","
            "\"gamertag\": \"removeme\","
            "\"player_hours\": 100,"
            "\"ssn\": \"9999-999999\","
            "\"email\": \"remove@example.com\","
            "\"subscription_start_date\": \"01.01.2025\","
            "\"subscription_end_date\": \"01.07.2025\","
            "\"is_subscribed\": false"
        "}"
    "]";
    create_test_users_file(json);
    
    // Act
    int result = logic_remove_user("removeme");
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify keeper's data is intact
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    TEST_ASSERT_EQUAL_INT(1, cJSON_GetArraySize(users));
    
    cJSON *keeper = cJSON_GetArrayItem(users, 0);
    TEST_ASSERT_EQUAL_STRING("User To Keep", cJSON_GetObjectItem(keeper, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("keeper", cJSON_GetObjectItem(keeper, "gamertag")->valuestring);
    TEST_ASSERT_EQUAL_INT(999, cJSON_GetObjectItem(keeper, "player_hours")->valueint);
    TEST_ASSERT_EQUAL_STRING("1234-567890", cJSON_GetObjectItem(keeper, "ssn")->valuestring);
    TEST_ASSERT_EQUAL_STRING("keeper@example.com", cJSON_GetObjectItem(keeper, "email")->valuestring);
    TEST_ASSERT_EQUAL_STRING("01.06.2025", cJSON_GetObjectItem(keeper, "subscription_start_date")->valuestring);
    TEST_ASSERT_EQUAL_STRING("01.12.2025", cJSON_GetObjectItem(keeper, "subscription_end_date")->valuestring);
    TEST_ASSERT_TRUE(cJSON_GetObjectItem(keeper, "is_subscribed")->valueint);
    
    cJSON_Delete(users);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_remove_user_success_single_user);
    RUN_TEST(test_logic_remove_user_success_from_multiple_users);
    RUN_TEST(test_logic_remove_user_not_found);
    RUN_TEST(test_logic_remove_user_empty_gamertag);
    RUN_TEST(test_logic_remove_user_case_sensitive);
    RUN_TEST(test_logic_remove_user_special_characters_gamertag);
    RUN_TEST(test_logic_remove_user_from_empty_array);
    RUN_TEST(test_logic_remove_user_missing_file);
    RUN_TEST(test_logic_remove_user_invalid_json);
    RUN_TEST(test_logic_remove_user_remove_first_user);
    RUN_TEST(test_logic_remove_user_remove_last_user);
    RUN_TEST(test_logic_remove_user_duplicate_gamertags_removes_first);
    RUN_TEST(test_logic_remove_user_preserves_other_user_data);
    return UNITY_END();
}