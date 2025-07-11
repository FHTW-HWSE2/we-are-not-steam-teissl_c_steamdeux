#include "unity.h"
#include "cJSON.h"
#include "../../inc/data/data.h"
#include "../../inc/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Override the USERS_JSON_PATH after includes
#ifdef USERS_JSON_PATH
#undef USERS_JSON_PATH
#endif
#define USERS_JSON_PATH "test/users_remove_test.json"

void setUp(void) {
    // Create test file with multiple users
    FILE *f = fopen(USERS_JSON_PATH, "w");
    fputs("["
          "{\"full_name\":\"User One\",\"gamertag\":\"user1\",\"player_hours\":100,"
          "\"ssn\":\"1111-111111\",\"email\":\"user1@example.com\","
          "\"subscription_start_date\":\"01.01.2024\",\"subscription_end_date\":\"01.01.2025\","
          "\"is_subscribed\":true},"
          "{\"full_name\":\"User Two\",\"gamertag\":\"user2\",\"player_hours\":200,"
          "\"ssn\":\"2222-222222\",\"email\":\"user2@example.com\","
          "\"subscription_start_date\":\"01.01.2024\",\"subscription_end_date\":\"01.01.2025\","
          "\"is_subscribed\":false},"
          "{\"full_name\":\"User Three\",\"gamertag\":\"user3\",\"player_hours\":300,"
          "\"ssn\":\"3333-333333\",\"email\":\"user3@example.com\","
          "\"subscription_start_date\":\"01.01.2024\",\"subscription_end_date\":\"01.01.2025\","
          "\"is_subscribed\":true}"
          "]", f);
    fclose(f);
}

void tearDown(void) {
    remove(USERS_JSON_PATH);
}

// Helper function to load and parse JSON file
static cJSON* load_json_from_testfile(const char* path) {
    FILE *file = fopen(path, "r");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = malloc(length + 1);
    if (!data) {
        fclose(file);
        return NULL;
    }

    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    cJSON *json = cJSON_Parse(data);
    free(data);
    return json;
}

void test_data_remove_player_profile_success_returns_ERR_SUCCESS(void) {
    // Act: Remove existing user
    int result = data_remove_player_profile("user2");

    // Assert: Should return success
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    // Verify user was removed
    cJSON *users = load_json_from_testfile(USERS_JSON_PATH);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_TRUE(cJSON_IsArray(users));
    TEST_ASSERT_EQUAL_INT(2, cJSON_GetArraySize(users)); // Should be 2 users left

    // Check remaining users are user1 and user3
    cJSON *user1 = cJSON_GetArrayItem(users, 0);
    cJSON *user2 = cJSON_GetArrayItem(users, 1);
    
    cJSON *gamertag1 = cJSON_GetObjectItem(user1, "gamertag");
    cJSON *gamertag2 = cJSON_GetObjectItem(user2, "gamertag");
    
    TEST_ASSERT_NOT_NULL(gamertag1);
    TEST_ASSERT_NOT_NULL(gamertag2);
    
    // user2 should be gone, so we should have user1 and user3
    TEST_ASSERT_TRUE(strcmp(gamertag1->valuestring, "user1") == 0 || strcmp(gamertag1->valuestring, "user3") == 0);
    TEST_ASSERT_TRUE(strcmp(gamertag2->valuestring, "user1") == 0 || strcmp(gamertag2->valuestring, "user3") == 0);
    TEST_ASSERT_TRUE(strcmp(gamertag1->valuestring, gamertag2->valuestring) != 0); // Should be different

    cJSON_Delete(users);
}

void test_data_remove_player_profile_first_user_success(void) {
    // Act: Remove first user
    int result = data_remove_player_profile("user1");

    // Assert: Should return success
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    // Verify user was removed
    cJSON *users = load_json_from_testfile(USERS_JSON_PATH);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_EQUAL_INT(2, cJSON_GetArraySize(users)); // Should be 2 users left

    // Check first user is now user2
    cJSON *first_user = cJSON_GetArrayItem(users, 0);
    cJSON *gamertag = cJSON_GetObjectItem(first_user, "gamertag");
    TEST_ASSERT_EQUAL_STRING("user2", gamertag->valuestring);

    cJSON_Delete(users);
}

void test_data_remove_player_profile_last_user_success(void) {
    // Act: Remove last user
    int result = data_remove_player_profile("user3");

    // Assert: Should return success
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    // Verify user was removed
    cJSON *users = load_json_from_testfile(USERS_JSON_PATH);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_EQUAL_INT(2, cJSON_GetArraySize(users)); // Should be 2 users left

    // Check that user3 is gone
    for (int i = 0; i < cJSON_GetArraySize(users); i++) {
        cJSON *user = cJSON_GetArrayItem(users, i);
        cJSON *gamertag = cJSON_GetObjectItem(user, "gamertag");
        TEST_ASSERT_TRUE(strcmp(gamertag->valuestring, "user3") != 0);
    }

    cJSON_Delete(users);
}

void test_data_remove_player_profile_user_not_found_returns_ERR_USER_NOT_FOUND(void) {
    // Act: Try to remove non-existent user
    int result = data_remove_player_profile("nonexistent");

    // Assert: Should return user not found error
    TEST_ASSERT_EQUAL_INT(ERR_USER_NOT_FOUND, result);

    // Verify no users were removed
    cJSON *users = load_json_from_testfile(USERS_JSON_PATH);
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_EQUAL_INT(3, cJSON_GetArraySize(users)); // Should still be 3 users

    cJSON_Delete(users);
}

void test_data_remove_player_profile_invalid_file_returns_ERR_STORAGE_FAILURE(void) {
    // Arrange: Create invalid JSON file
    remove(USERS_JSON_PATH);
    FILE *f = fopen(USERS_JSON_PATH, "w");
    fputs("{\"not\": \"an array\"}", f);
    fclose(f);

    // Act: Try to remove user from invalid file
    int result = data_remove_player_profile("user1");

    // Assert: Should return storage failure
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
}

void test_data_remove_player_profile_missing_file_returns_ERR_STORAGE_FAILURE(void) {
    // Arrange: Remove test file
    remove(USERS_JSON_PATH);

    // Act: Try to remove user from missing file
    int result = data_remove_player_profile("user1");

    // Assert: Should return storage failure
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_data_remove_player_profile_success_returns_ERR_SUCCESS);
    RUN_TEST(test_data_remove_player_profile_first_user_success);
    RUN_TEST(test_data_remove_player_profile_last_user_success);
    RUN_TEST(test_data_remove_player_profile_user_not_found_returns_ERR_USER_NOT_FOUND);
    RUN_TEST(test_data_remove_player_profile_invalid_file_returns_ERR_STORAGE_FAILURE);
    RUN_TEST(test_data_remove_player_profile_missing_file_returns_ERR_STORAGE_FAILURE);
    return UNITY_END();
}