#define USERS_JSON_PATH "test/users_get_all_test.json"

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

void test_logic_get_all_users_success_with_multiple_users(void) {
    // Arrange: Create file with multiple users
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
        "},"
        "{"
            "\"full_name\": \"Jane Smith\","
            "\"gamertag\": \"janesmith456\","
            "\"player_hours\": 200,"
            "\"ssn\": \"2222-333333\","
            "\"email\": \"jane@example.com\","
            "\"subscription_start_date\": \"01.02.2025\","
            "\"subscription_end_date\": \"01.08.2025\","
            "\"is_subscribed\": false"
        "},"
        "{"
            "\"full_name\": \"Bob Johnson\","
            "\"gamertag\": \"bobjohnson789\","
            "\"player_hours\": 300,"
            "\"ssn\": \"3333-444444\","
            "\"email\": \"bob@example.com\","
            "\"subscription_start_date\": \"01.03.2025\","
            "\"subscription_end_date\": \"01.09.2025\","
            "\"is_subscribed\": true"
        "}"
    "]";
    create_test_users_file(json);
    
    cJSON *users = NULL;
    
    // Act
    int result = logic_get_all_users(&users);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_TRUE(cJSON_IsArray(users));
    TEST_ASSERT_EQUAL_INT(3, cJSON_GetArraySize(users));
    
    // Verify first user
    cJSON *user1 = cJSON_GetArrayItem(users, 0);
    TEST_ASSERT_EQUAL_STRING("John Doe", cJSON_GetObjectItem(user1, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("johndoe123", cJSON_GetObjectItem(user1, "gamertag")->valuestring);
    TEST_ASSERT_EQUAL_INT(100, cJSON_GetObjectItem(user1, "player_hours")->valueint);
    
    // Verify second user
    cJSON *user2 = cJSON_GetArrayItem(users, 1);
    TEST_ASSERT_EQUAL_STRING("Jane Smith", cJSON_GetObjectItem(user2, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("janesmith456", cJSON_GetObjectItem(user2, "gamertag")->valuestring);
    TEST_ASSERT_EQUAL_INT(200, cJSON_GetObjectItem(user2, "player_hours")->valueint);
    
    // Verify third user
    cJSON *user3 = cJSON_GetArrayItem(users, 2);
    TEST_ASSERT_EQUAL_STRING("Bob Johnson", cJSON_GetObjectItem(user3, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("bobjohnson789", cJSON_GetObjectItem(user3, "gamertag")->valuestring);
    TEST_ASSERT_EQUAL_INT(300, cJSON_GetObjectItem(user3, "player_hours")->valueint);
    
    // Cleanup - caller must free
    cJSON_Delete(users);
}

void test_logic_get_all_users_empty_array(void) {
    // Arrange: Create empty array
    create_test_users_file("[]");
    
    cJSON *users = NULL;
    
    // Act
    int result = logic_get_all_users(&users);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_TRUE(cJSON_IsArray(users));
    TEST_ASSERT_EQUAL_INT(0, cJSON_GetArraySize(users));
    
    // Cleanup
    cJSON_Delete(users);
}

void test_logic_get_all_users_single_user(void) {
    // Arrange: Create file with single user
    const char *json = "["
        "{"
            "\"full_name\": \"Solo User\","
            "\"gamertag\": \"solo123\","
            "\"player_hours\": 999,"
            "\"ssn\": \"9999-999999\","
            "\"email\": \"solo@example.com\","
            "\"subscription_start_date\": \"01.06.2025\","
            "\"subscription_end_date\": \"01.12.2025\","
            "\"is_subscribed\": true"
        "}"
    "]";
    create_test_users_file(json);
    
    cJSON *users = NULL;
    
    // Act
    int result = logic_get_all_users(&users);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_TRUE(cJSON_IsArray(users));
    TEST_ASSERT_EQUAL_INT(1, cJSON_GetArraySize(users));
    
    cJSON *user = cJSON_GetArrayItem(users, 0);
    TEST_ASSERT_EQUAL_STRING("Solo User", cJSON_GetObjectItem(user, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_INT(999, cJSON_GetObjectItem(user, "player_hours")->valueint);
    
    // Cleanup
    cJSON_Delete(users);
}

void test_logic_get_all_users_missing_file_returns_empty_array(void) {
    // Arrange: Ensure file doesn't exist
    remove(USERS_JSON_PATH);
    
    cJSON *users = NULL;
    
    // Act
    int result = logic_get_all_users(&users);
    
    // Assert: Should return success with empty array
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_TRUE(cJSON_IsArray(users));
    TEST_ASSERT_EQUAL_INT(0, cJSON_GetArraySize(users));
    
    // Cleanup
    cJSON_Delete(users);
}

void test_logic_get_all_users_invalid_json_returns_empty_array(void) {
    // Arrange: Create invalid JSON
    create_test_users_file("{corrupted json data[}");
    
    cJSON *users = NULL;
    
    // Act
    int result = logic_get_all_users(&users);
    
    // Assert: Should return success with empty array
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_TRUE(cJSON_IsArray(users));
    TEST_ASSERT_EQUAL_INT(0, cJSON_GetArraySize(users));
    
    // Cleanup
    cJSON_Delete(users);
}

void test_logic_get_all_users_not_array_returns_empty_array(void) {
    // Arrange: Create valid JSON but not an array
    create_test_users_file("{\"not\": \"an array\", \"but\": \"valid json\"}");
    
    cJSON *users = NULL;
    
    // Act
    int result = logic_get_all_users(&users);
    
    // Assert: Should return success with empty array
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_TRUE(cJSON_IsArray(users));
    TEST_ASSERT_EQUAL_INT(0, cJSON_GetArraySize(users));
    
    // Cleanup
    cJSON_Delete(users);
}

void test_logic_get_all_users_with_special_characters(void) {
    // Arrange: Create users with special characters
    const char *json = "["
        "{"
            "\"full_name\": \"José María Müller-O'Brien\","
            "\"gamertag\": \"spëcîál_üsér@123\","
            "\"player_hours\": 42,"
            "\"ssn\": \"1234-567890\","
            "\"email\": \"josé.maría@ñoño-domain.com\","
            "\"subscription_start_date\": \"01.01.2025\","
            "\"subscription_end_date\": \"01.07.2025\","
            "\"is_subscribed\": true"
        "},"
        "{"
            "\"full_name\": \"李明 (Li Ming)\","
            "\"gamertag\": \"李明123\","
            "\"player_hours\": 88,"
            "\"ssn\": \"8888-888888\","
            "\"email\": \"liming@例え.jp\","
            "\"subscription_start_date\": \"01.02.2025\","
            "\"subscription_end_date\": \"01.08.2025\","
            "\"is_subscribed\": false"
        "}"
    "]";
    create_test_users_file(json);
    
    cJSON *users = NULL;
    
    // Act
    int result = logic_get_all_users(&users);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_EQUAL_INT(2, cJSON_GetArraySize(users));
    
    // Verify special characters are preserved
    cJSON *user1 = cJSON_GetArrayItem(users, 0);
    TEST_ASSERT_EQUAL_STRING("José María Müller-O'Brien", cJSON_GetObjectItem(user1, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("spëcîál_üsér@123", cJSON_GetObjectItem(user1, "gamertag")->valuestring);
    
    cJSON *user2 = cJSON_GetArrayItem(users, 1);
    TEST_ASSERT_EQUAL_STRING("李明 (Li Ming)", cJSON_GetObjectItem(user2, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("李明123", cJSON_GetObjectItem(user2, "gamertag")->valuestring);
    
    // Cleanup
    cJSON_Delete(users);
}

void test_logic_get_all_users_with_missing_fields(void) {
    // Arrange: Create users with some missing fields
    const char *json = "["
        "{"
            "\"gamertag\": \"minimal1\","
            "\"player_hours\": 10"
            // Missing other fields
        "},"
        "{"
            "\"full_name\": \"Partial User\","
            "\"gamertag\": \"partial2\""
            // Missing numeric and date fields
        "},"
        "{"
            "\"full_name\": \"Complete User\","
            "\"gamertag\": \"complete3\","
            "\"player_hours\": 500,"
            "\"ssn\": \"5555-555555\","
            "\"email\": \"complete@example.com\","
            "\"subscription_start_date\": \"01.05.2025\","
            "\"subscription_end_date\": \"01.11.2025\","
            "\"is_subscribed\": true"
        "}"
    "]";
    create_test_users_file(json);
    
    cJSON *users = NULL;
    
    // Act
    int result = logic_get_all_users(&users);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_EQUAL_INT(3, cJSON_GetArraySize(users));
    
    // Verify minimal user
    cJSON *user1 = cJSON_GetArrayItem(users, 0);
    TEST_ASSERT_EQUAL_STRING("minimal1", cJSON_GetObjectItem(user1, "gamertag")->valuestring);
    TEST_ASSERT_NULL(cJSON_GetObjectItem(user1, "full_name")); // Missing field
    
    // Verify partial user
    cJSON *user2 = cJSON_GetArrayItem(users, 1);
    TEST_ASSERT_EQUAL_STRING("Partial User", cJSON_GetObjectItem(user2, "full_name")->valuestring);
    TEST_ASSERT_NULL(cJSON_GetObjectItem(user2, "player_hours")); // Missing field
    
    // Cleanup
    cJSON_Delete(users);
}

void test_logic_get_all_users_large_user_list(void) {
    // Arrange: Create many users
    cJSON *large_array = cJSON_CreateArray();
    
    for (int i = 0; i < 100; i++) {
        cJSON *user = cJSON_CreateObject();
        
        char name[50], gamertag[50], ssn[20], email[50];
        sprintf(name, "User %d", i);
        sprintf(gamertag, "user%d", i);
        sprintf(ssn, "%04d-%06d", i, i * 10);
        sprintf(email, "user%d@example.com", i);
        
        cJSON_AddStringToObject(user, "full_name", name);
        cJSON_AddStringToObject(user, "gamertag", gamertag);
        cJSON_AddNumberToObject(user, "player_hours", i * 10);
        cJSON_AddStringToObject(user, "ssn", ssn);
        cJSON_AddStringToObject(user, "email", email);
        cJSON_AddStringToObject(user, "subscription_start_date", "01.01.2025");
        cJSON_AddStringToObject(user, "subscription_end_date", "01.01.2026");
        cJSON_AddBoolToObject(user, "is_subscribed", i % 2 == 0);
        
        cJSON_AddItemToArray(large_array, user);
    }
    
    char *json_str = cJSON_Print(large_array);
    create_test_users_file(json_str);
    free(json_str);
    cJSON_Delete(large_array);
    
    cJSON *users = NULL;
    
    // Act
    int result = logic_get_all_users(&users);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_EQUAL_INT(100, cJSON_GetArraySize(users));
    
    // Spot check some users
    cJSON *first = cJSON_GetArrayItem(users, 0);
    TEST_ASSERT_EQUAL_STRING("User 0", cJSON_GetObjectItem(first, "full_name")->valuestring);
    
    cJSON *middle = cJSON_GetArrayItem(users, 50);
    TEST_ASSERT_EQUAL_STRING("User 50", cJSON_GetObjectItem(middle, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_INT(500, cJSON_GetObjectItem(middle, "player_hours")->valueint);
    
    cJSON *last = cJSON_GetArrayItem(users, 99);
    TEST_ASSERT_EQUAL_STRING("User 99", cJSON_GetObjectItem(last, "full_name")->valuestring);
    
    // Cleanup
    cJSON_Delete(users);
}

void test_logic_get_all_users_preserves_data_integrity(void) {
    // Arrange: Create users with all field types
    const char *json = "["
        "{"
            "\"full_name\": \"Test User\","
            "\"gamertag\": \"testuser123\","
            "\"player_hours\": 12345,"
            "\"ssn\": \"1234-567890\","
            "\"email\": \"test@example.com\","
            "\"subscription_start_date\": \"15.06.2025\","
            "\"subscription_end_date\": \"15.12.2025\","
            "\"is_subscribed\": true"
        "}"
    "]";
    create_test_users_file(json);
    
    cJSON *users = NULL;
    
    // Act
    int result = logic_get_all_users(&users);
    
    // Assert: Verify all data types are preserved correctly
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(users);
    
    cJSON *user = cJSON_GetArrayItem(users, 0);
    
    // String fields
    TEST_ASSERT_EQUAL_STRING("Test User", cJSON_GetObjectItem(user, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("testuser123", cJSON_GetObjectItem(user, "gamertag")->valuestring);
    TEST_ASSERT_EQUAL_STRING("1234-567890", cJSON_GetObjectItem(user, "ssn")->valuestring);
    TEST_ASSERT_EQUAL_STRING("test@example.com", cJSON_GetObjectItem(user, "email")->valuestring);
    TEST_ASSERT_EQUAL_STRING("15.06.2025", cJSON_GetObjectItem(user, "subscription_start_date")->valuestring);
    TEST_ASSERT_EQUAL_STRING("15.12.2025", cJSON_GetObjectItem(user, "subscription_end_date")->valuestring);
    
    // Numeric field
    TEST_ASSERT_EQUAL_INT(12345, cJSON_GetObjectItem(user, "player_hours")->valueint);
    
    // Boolean field
    TEST_ASSERT_TRUE(cJSON_IsTrue(cJSON_GetObjectItem(user, "is_subscribed")));
    
    // Cleanup
    cJSON_Delete(users);
}

void test_logic_get_all_users_multiple_calls(void) {
    // Arrange: Create users
    const char *json = "["
        "{\"gamertag\": \"user1\", \"full_name\": \"User One\"},"
        "{\"gamertag\": \"user2\", \"full_name\": \"User Two\"}"
    "]";
    create_test_users_file(json);
    
    cJSON *users1 = NULL;
    cJSON *users2 = NULL;
    
    // Act: Call function multiple times
    int result1 = logic_get_all_users(&users1);
    int result2 = logic_get_all_users(&users2);
    
    // Assert: Both calls should succeed independently
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result1);
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result2);
    TEST_ASSERT_NOT_NULL(users1);
    TEST_ASSERT_NOT_NULL(users2);
    TEST_ASSERT_EQUAL_INT(2, cJSON_GetArraySize(users1));
    TEST_ASSERT_EQUAL_INT(2, cJSON_GetArraySize(users2));
    
    // They should be different objects (not the same pointer)
    TEST_ASSERT_NOT_EQUAL(users1, users2);
    
    // Cleanup both
    cJSON_Delete(users1);
    cJSON_Delete(users2);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_get_all_users_success_with_multiple_users);
    RUN_TEST(test_logic_get_all_users_empty_array);
    RUN_TEST(test_logic_get_all_users_single_user);
    RUN_TEST(test_logic_get_all_users_missing_file_returns_empty_array);
    RUN_TEST(test_logic_get_all_users_invalid_json_returns_empty_array);
    RUN_TEST(test_logic_get_all_users_not_array_returns_empty_array);
    RUN_TEST(test_logic_get_all_users_with_special_characters);
    RUN_TEST(test_logic_get_all_users_with_missing_fields);
    RUN_TEST(test_logic_get_all_users_large_user_list);
    RUN_TEST(test_logic_get_all_users_preserves_data_integrity);
    RUN_TEST(test_logic_get_all_users_multiple_calls);
    return UNITY_END();
}