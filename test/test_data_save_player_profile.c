#define USERS_JSON_PATH "test/users_save_profile.json"

#include "unity.h"
#include "cJSON.h"
#include "../../inc/data/data.h"
#include "../../inc/error.h"
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

void test_data_save_player_profile_to_empty_file_success(void) {
    // Act: Save first user to non-existing file
    int result = data_save_player_profile(
        "John Doe",
        "johndoe123",
        150,
        "1234-567890",
        "john.doe@example.com",
        "01.01.2025",
        "01.01.2026",
        1
    );
    
    // Assert: Should succeed
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify file was created with correct content
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_TRUE(cJSON_IsArray(users));
    TEST_ASSERT_EQUAL_INT(1, cJSON_GetArraySize(users));
    
    cJSON *user = cJSON_GetArrayItem(users, 0);
    TEST_ASSERT_EQUAL_STRING("John Doe", cJSON_GetObjectItem(user, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("johndoe123", cJSON_GetObjectItem(user, "gamertag")->valuestring);
    TEST_ASSERT_EQUAL_INT(150, cJSON_GetObjectItem(user, "player_hours")->valueint);
    TEST_ASSERT_EQUAL_STRING("1234-567890", cJSON_GetObjectItem(user, "ssn")->valuestring);
    TEST_ASSERT_EQUAL_STRING("john.doe@example.com", cJSON_GetObjectItem(user, "email")->valuestring);
    TEST_ASSERT_EQUAL_STRING("01.01.2025", cJSON_GetObjectItem(user, "subscription_start_date")->valuestring);
    TEST_ASSERT_EQUAL_STRING("01.01.2026", cJSON_GetObjectItem(user, "subscription_end_date")->valuestring);
    TEST_ASSERT_EQUAL_INT(1, cJSON_GetObjectItem(user, "is_subscribed")->valueint);
    
    cJSON_Delete(users);
}

void test_data_save_player_profile_append_to_existing_users(void) {
    // Arrange: Create file with existing user
    cJSON *existing_users = cJSON_CreateArray();
    cJSON *existing_user = cJSON_CreateObject();
    cJSON_AddStringToObject(existing_user, "full_name", "Existing User");
    cJSON_AddStringToObject(existing_user, "gamertag", "existing123");
    cJSON_AddNumberToObject(existing_user, "player_hours", 100);
    cJSON_AddStringToObject(existing_user, "ssn", "9999-999999");
    cJSON_AddStringToObject(existing_user, "email", "existing@example.com");
    cJSON_AddStringToObject(existing_user, "subscription_start_date", "01.06.2024");
    cJSON_AddStringToObject(existing_user, "subscription_end_date", "01.06.2025");
    cJSON_AddBoolToObject(existing_user, "is_subscribed", 1);
    cJSON_AddItemToArray(existing_users, existing_user);
    
    char *json_str = cJSON_Print(existing_users);
    FILE *f = fopen(USERS_JSON_PATH, "w");
    fputs(json_str, f);
    fclose(f);
    free(json_str);
    cJSON_Delete(existing_users);
    
    // Act: Add new user
    int result = data_save_player_profile(
        "Jane Smith",
        "janesmith456",
        200,
        "2222-333333",
        "jane.smith@example.com",
        "15.02.2025",
        "15.08.2025",
        1
    );
    
    // Assert: Should succeed
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify both users exist
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_EQUAL_INT(2, cJSON_GetArraySize(users));
    
    // Check first user still exists
    cJSON *user1 = cJSON_GetArrayItem(users, 0);
    TEST_ASSERT_EQUAL_STRING("Existing User", cJSON_GetObjectItem(user1, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("existing123", cJSON_GetObjectItem(user1, "gamertag")->valuestring);
    
    // Check new user was added
    cJSON *user2 = cJSON_GetArrayItem(users, 1);
    TEST_ASSERT_EQUAL_STRING("Jane Smith", cJSON_GetObjectItem(user2, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("janesmith456", cJSON_GetObjectItem(user2, "gamertag")->valuestring);
    TEST_ASSERT_EQUAL_INT(200, cJSON_GetObjectItem(user2, "player_hours")->valueint);
    
    cJSON_Delete(users);
}

void test_data_save_player_profile_with_duplicate_gamertag(void) {
    // Arrange: Create user with gamertag "duplicate123"
    int result1 = data_save_player_profile(
        "First User",
        "duplicate123",
        100,
        "1111-111111",
        "first@example.com",
        "01.01.2025",
        "01.07.2025",
        1
    );
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result1);
    
    // Act: Add another user with same gamertag (function allows this)
    int result2 = data_save_player_profile(
        "Second User",
        "duplicate123",  // Same gamertag
        200,
        "2222-222222",
        "second@example.com",
        "01.02.2025",
        "01.08.2025",
        1
    );
    
    // Assert: Should succeed (function doesn't check for duplicates)
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result2);
    
    // Verify both users exist
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    TEST_ASSERT_EQUAL_INT(2, cJSON_GetArraySize(users));
    
    // Both should have the same gamertag
    cJSON *user1 = cJSON_GetArrayItem(users, 0);
    cJSON *user2 = cJSON_GetArrayItem(users, 1);
    TEST_ASSERT_EQUAL_STRING("duplicate123", cJSON_GetObjectItem(user1, "gamertag")->valuestring);
    TEST_ASSERT_EQUAL_STRING("duplicate123", cJSON_GetObjectItem(user2, "gamertag")->valuestring);
    
    // But different names
    TEST_ASSERT_EQUAL_STRING("First User", cJSON_GetObjectItem(user1, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("Second User", cJSON_GetObjectItem(user2, "full_name")->valuestring);
    
    cJSON_Delete(users);
}

void test_data_save_player_profile_with_special_characters(void) {
    // Act: Save user with special characters
    int result = data_save_player_profile(
        "Müller, José & O'Brien",  // Special chars in name
        "sp€ci@l_ch@rs",          // Special chars in gamertag
        999,
        "1234 56/7890",           // Special format SSN
        "user+tag@sub-domain.example.com", // Complex email
        "31.12.2024",
        "31.12.2025",
        0
    );
    
    // Assert: Should succeed
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify special characters are preserved
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    cJSON *user = cJSON_GetArrayItem(users, 0);
    
    TEST_ASSERT_EQUAL_STRING("Müller, José & O'Brien", cJSON_GetObjectItem(user, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("sp€ci@l_ch@rs", cJSON_GetObjectItem(user, "gamertag")->valuestring);
    TEST_ASSERT_EQUAL_STRING("1234 56/7890", cJSON_GetObjectItem(user, "ssn")->valuestring);
    TEST_ASSERT_EQUAL_STRING("user+tag@sub-domain.example.com", cJSON_GetObjectItem(user, "email")->valuestring);
    
    cJSON_Delete(users);
}

void test_data_save_player_profile_with_edge_values(void) {
    // Act: Save user with edge case values
    int result = data_save_player_profile(
        "",           // Empty name
        "",           // Empty gamertag
        0,            // Zero hours
        "",           // Empty SSN
        "",           // Empty email
        "",           // Empty start date
        "",           // Empty end date
        0             // Not subscribed
    );
    
    // Assert: Should succeed (no validation in data layer)
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify empty strings are saved
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    cJSON *user = cJSON_GetArrayItem(users, 0);
    
    TEST_ASSERT_EQUAL_STRING("", cJSON_GetObjectItem(user, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("", cJSON_GetObjectItem(user, "gamertag")->valuestring);
    TEST_ASSERT_EQUAL_INT(0, cJSON_GetObjectItem(user, "player_hours")->valueint);
    TEST_ASSERT_EQUAL_STRING("", cJSON_GetObjectItem(user, "ssn")->valuestring);
    TEST_ASSERT_EQUAL_STRING("", cJSON_GetObjectItem(user, "email")->valuestring);
    TEST_ASSERT_EQUAL_INT(0, cJSON_GetObjectItem(user, "is_subscribed")->valueint);
    
    cJSON_Delete(users);
}

void test_data_save_player_profile_multiple_saves_sequentially(void) {
    // Act: Save multiple users sequentially
    int result1 = data_save_player_profile(
        "User One", "user1", 100, "1111-111111", "user1@example.com",
        "01.01.2025", "01.07.2025", 1
    );
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result1);
    
    int result2 = data_save_player_profile(
        "User Two", "user2", 200, "2222-222222", "user2@example.com",
        "02.01.2025", "02.07.2025", 1
    );
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result2);
    
    int result3 = data_save_player_profile(
        "User Three", "user3", 300, "3333-333333", "user3@example.com",
        "03.01.2025", "03.07.2025", 0
    );
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result3);
    
    // Assert: All users should be saved
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    TEST_ASSERT_EQUAL_INT(3, cJSON_GetArraySize(users));
    
    // Verify order is preserved
    cJSON *user1 = cJSON_GetArrayItem(users, 0);
    cJSON *user2 = cJSON_GetArrayItem(users, 1);
    cJSON *user3 = cJSON_GetArrayItem(users, 2);
    
    TEST_ASSERT_EQUAL_STRING("User One", cJSON_GetObjectItem(user1, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("User Two", cJSON_GetObjectItem(user2, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("User Three", cJSON_GetObjectItem(user3, "full_name")->valuestring);
    
    TEST_ASSERT_EQUAL_INT(100, cJSON_GetObjectItem(user1, "player_hours")->valueint);
    TEST_ASSERT_EQUAL_INT(200, cJSON_GetObjectItem(user2, "player_hours")->valueint);
    TEST_ASSERT_EQUAL_INT(300, cJSON_GetObjectItem(user3, "player_hours")->valueint);
    
    cJSON_Delete(users);
}

void test_data_save_player_profile_with_large_player_hours(void) {
    // Act: Save user with very large player hours
    int result = data_save_player_profile(
        "Hardcore Gamer",
        "nolife999",
        999999,  // Very large hours
        "9999-999999",
        "hardcore@gamer.com",
        "01.01.2020",
        "01.01.2030",
        1
    );
    
    // Assert: Should succeed
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify large number is saved correctly
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    cJSON *user = cJSON_GetArrayItem(users, 0);
    
    TEST_ASSERT_EQUAL_INT(999999, cJSON_GetObjectItem(user, "player_hours")->valueint);
    
    cJSON_Delete(users);
}

void test_data_save_player_profile_to_invalid_json_file(void) {
    // Arrange: Create file with invalid JSON (not an array)
    FILE *f = fopen(USERS_JSON_PATH, "w");
    fputs("{\"not\": \"an array\"}", f);
    fclose(f);
    
    // Act: Try to save user (function creates new array when JSON is invalid)
    int result = data_save_player_profile(
        "New User",
        "newuser123",
        50,
        "5555-555555",
        "new@example.com",
        "01.03.2025",
        "01.09.2025",
        1
    );
    
    // Assert: Should succeed (creates new array)
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify new array was created with the user
    cJSON *users = load_json_from_file(USERS_JSON_PATH);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_TRUE(cJSON_IsArray(users));
    TEST_ASSERT_EQUAL_INT(1, cJSON_GetArraySize(users));
    
    cJSON *user = cJSON_GetArrayItem(users, 0);
    TEST_ASSERT_EQUAL_STRING("New User", cJSON_GetObjectItem(user, "full_name")->valuestring);
    
    cJSON_Delete(users);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_data_save_player_profile_to_empty_file_success);
    RUN_TEST(test_data_save_player_profile_append_to_existing_users);
    RUN_TEST(test_data_save_player_profile_with_duplicate_gamertag);
    RUN_TEST(test_data_save_player_profile_with_special_characters);
    RUN_TEST(test_data_save_player_profile_with_edge_values);
    RUN_TEST(test_data_save_player_profile_multiple_saves_sequentially);
    RUN_TEST(test_data_save_player_profile_with_large_player_hours);
    RUN_TEST(test_data_save_player_profile_to_invalid_json_file);
    return UNITY_END();
}