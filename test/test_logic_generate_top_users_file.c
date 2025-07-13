#define USERS_JSON_PATH "test/users_generate_top_test.json"
#define OUTPUT_FILE_PATH "usersRanked.json"

#include "unity.h"
#include "../../inc/logic/logic.h"
#include "../../inc/data/data.h"
#include "../../inc/error.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setUp(void) {
    // Clean up test files before each test
    remove(USERS_JSON_PATH);
    remove(OUTPUT_FILE_PATH);
}

void tearDown(void) {
    // Clean up test files after each test
    remove(USERS_JSON_PATH);
    remove(OUTPUT_FILE_PATH);
}

// Helper function to create test users file
static void create_test_users_file(const char *json_content) {
    FILE *f = fopen(USERS_JSON_PATH, "w");
    fputs(json_content, f);
    fclose(f);
}

// Helper function to read generated file content
static char* read_output_file(void) {
    FILE *f = fopen(OUTPUT_FILE_PATH, "r");
    if (!f) return NULL;
    
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char *content = malloc(length + 1);
    if (!content) {
        fclose(f);
        return NULL;
    }
    
    fread(content, 1, length, f);
    content[length] = '\0';
    fclose(f);
    
    return content;
}

// Helper function to verify user order in output file
static int verify_output_user_order(const char *expected_gamertags[], int count) {
    char *content = read_output_file();
    if (!content) return 0;
    
    cJSON *json = cJSON_Parse(content);
    free(content);
    
    if (!json || !cJSON_IsArray(json)) {
        if (json) cJSON_Delete(json);
        return 0;
    }
    
    if (cJSON_GetArraySize(json) != count) {
        cJSON_Delete(json);
        return 0;
    }
    
    for (int i = 0; i < count; i++) {
        cJSON *user = cJSON_GetArrayItem(json, i);
        cJSON *gamertag = cJSON_GetObjectItem(user, "gamertag");
        if (!gamertag || strcmp(gamertag->valuestring, expected_gamertags[i]) != 0) {
            cJSON_Delete(json);
            return 0;
        }
    }
    
    cJSON_Delete(json);
    return 1;
}

void test_logic_generate_top_users_file_success_with_exactly_10_users(void) {
    // Arrange: Create exactly 10 users with different hours
    const char *json = "["
        "{\"full_name\": \"User 1\", \"gamertag\": \"user1\", \"player_hours\": 100, \"ssn\": \"1111-111111\", \"email\": \"user1@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"User 2\", \"gamertag\": \"user2\", \"player_hours\": 200, \"ssn\": \"2222-222222\", \"email\": \"user2@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"User 3\", \"gamertag\": \"user3\", \"player_hours\": 300, \"ssn\": \"3333-333333\", \"email\": \"user3@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"User 4\", \"gamertag\": \"user4\", \"player_hours\": 400, \"ssn\": \"4444-444444\", \"email\": \"user4@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"User 5\", \"gamertag\": \"user5\", \"player_hours\": 500, \"ssn\": \"5555-555555\", \"email\": \"user5@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"User 6\", \"gamertag\": \"user6\", \"player_hours\": 600, \"ssn\": \"6666-666666\", \"email\": \"user6@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"User 7\", \"gamertag\": \"user7\", \"player_hours\": 700, \"ssn\": \"7777-777777\", \"email\": \"user7@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"User 8\", \"gamertag\": \"user8\", \"player_hours\": 800, \"ssn\": \"8888-888888\", \"email\": \"user8@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"User 9\", \"gamertag\": \"user9\", \"player_hours\": 900, \"ssn\": \"9999-999999\", \"email\": \"user9@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"User 10\", \"gamertag\": \"user10\", \"player_hours\": 1000, \"ssn\": \"0000-000000\", \"email\": \"user10@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true}"
    "]";
    create_test_users_file(json);
    
    // Act
    int result = logic_generate_top_users_file();
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify file exists
    FILE *f = fopen(OUTPUT_FILE_PATH, "r");
    TEST_ASSERT_NOT_NULL(f);
    fclose(f);
    
    // Verify correct order (descending by hours)
    const char *expected_order[] = {"user10", "user9", "user8", "user7", "user6", "user5", "user4", "user3", "user2", "user1"};
    TEST_ASSERT_TRUE(verify_output_user_order(expected_order, 10));
}

void test_logic_generate_top_users_file_more_than_10_users(void) {
    // Arrange: Create 15 users, should only get top 10
    cJSON *users_array = cJSON_CreateArray();
    
    for (int i = 1; i <= 15; i++) {
        cJSON *user = cJSON_CreateObject();
        char name[50], gamertag[50], ssn[20], email[50];
        sprintf(name, "User %d", i);
        sprintf(gamertag, "user%d", i);
        sprintf(ssn, "%04d-%06d", i, i * 111111);
        sprintf(email, "user%d@example.com", i);
        
        cJSON_AddStringToObject(user, "full_name", name);
        cJSON_AddStringToObject(user, "gamertag", gamertag);
        cJSON_AddNumberToObject(user, "player_hours", i * 100);
        cJSON_AddStringToObject(user, "ssn", ssn);
        cJSON_AddStringToObject(user, "email", email);
        cJSON_AddStringToObject(user, "subscription_start_date", "01.01.2025");
        cJSON_AddStringToObject(user, "subscription_end_date", "01.07.2025");
        cJSON_AddBoolToObject(user, "is_subscribed", 1);
        
        cJSON_AddItemToArray(users_array, user);
    }
    
    char *json_str = cJSON_Print(users_array);
    create_test_users_file(json_str);
    free(json_str);
    cJSON_Delete(users_array);
    
    // Act
    int result = logic_generate_top_users_file();
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Should only have top 10 users (user15 to user6)
    const char *expected_order[] = {"user15", "user14", "user13", "user12", "user11", "user10", "user9", "user8", "user7", "user6"};
    TEST_ASSERT_TRUE(verify_output_user_order(expected_order, 10));
}

void test_logic_generate_top_users_file_less_than_10_users(void) {
    // Arrange: Create only 5 users
    const char *json = "["
        "{\"full_name\": \"User A\", \"gamertag\": \"userA\", \"player_hours\": 50, \"ssn\": \"1111-111111\", \"email\": \"userA@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"User B\", \"gamertag\": \"userB\", \"player_hours\": 150, \"ssn\": \"2222-222222\", \"email\": \"userB@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"User C\", \"gamertag\": \"userC\", \"player_hours\": 250, \"ssn\": \"3333-333333\", \"email\": \"userC@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"User D\", \"gamertag\": \"userD\", \"player_hours\": 350, \"ssn\": \"4444-444444\", \"email\": \"userD@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"User E\", \"gamertag\": \"userE\", \"player_hours\": 450, \"ssn\": \"5555-555555\", \"email\": \"userE@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true}"
    "]";
    create_test_users_file(json);
    
    // Act
    int result = logic_generate_top_users_file();
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Should have all 5 users
    const char *expected_order[] = {"userE", "userD", "userC", "userB", "userA"};
    TEST_ASSERT_TRUE(verify_output_user_order(expected_order, 5));
}

void test_logic_generate_top_users_file_empty_users(void) {
    // Arrange: Create empty users array
    create_test_users_file("[]");
    
    // Act
    int result = logic_generate_top_users_file();
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify file contains empty array
    char *content = read_output_file();
    TEST_ASSERT_NOT_NULL(content);
    
    cJSON *json = cJSON_Parse(content);
    free(content);
    
    TEST_ASSERT_NOT_NULL(json);
    TEST_ASSERT_TRUE(cJSON_IsArray(json));
    TEST_ASSERT_EQUAL_INT(0, cJSON_GetArraySize(json));
    
    cJSON_Delete(json);
}

void test_logic_generate_top_users_file_single_user(void) {
    // Arrange: Create single user
    const char *json = "["
        "{\"full_name\": \"Solo Player\", \"gamertag\": \"solo\", \"player_hours\": 999, \"ssn\": \"9999-999999\", \"email\": \"solo@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true}"
    "]";
    create_test_users_file(json);
    
    // Act
    int result = logic_generate_top_users_file();
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    const char *expected_order[] = {"solo"};
    TEST_ASSERT_TRUE(verify_output_user_order(expected_order, 1));
}

void test_logic_generate_top_users_file_same_hours(void) {
    // Arrange: Create users with same hours
    const char *json = "["
        "{\"full_name\": \"Same 1\", \"gamertag\": \"same1\", \"player_hours\": 500, \"ssn\": \"1111-111111\", \"email\": \"same1@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"Different\", \"gamertag\": \"different\", \"player_hours\": 600, \"ssn\": \"2222-222222\", \"email\": \"different@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"Same 2\", \"gamertag\": \"same2\", \"player_hours\": 500, \"ssn\": \"3333-333333\", \"email\": \"same2@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"Same 3\", \"gamertag\": \"same3\", \"player_hours\": 500, \"ssn\": \"4444-444444\", \"email\": \"same3@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true}"
    "]";
    create_test_users_file(json);
    
    // Act
    int result = logic_generate_top_users_file();
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify file exists and contains 4 users
    char *content = read_output_file();
    TEST_ASSERT_NOT_NULL(content);
    
    cJSON *json_result = cJSON_Parse(content);
    free(content);
    
    TEST_ASSERT_NOT_NULL(json_result);
    TEST_ASSERT_EQUAL_INT(4, cJSON_GetArraySize(json_result));
    
    // First should be "different" with 600 hours
    cJSON *first = cJSON_GetArrayItem(json_result, 0);
    TEST_ASSERT_EQUAL_STRING("different", cJSON_GetObjectItem(first, "gamertag")->valuestring);
    TEST_ASSERT_EQUAL_INT(600, cJSON_GetObjectItem(first, "player_hours")->valueint);
    
    cJSON_Delete(json_result);
}

void test_logic_generate_top_users_file_overwrites_existing(void) {
    // Arrange: Create existing file with content
    FILE *f = fopen(OUTPUT_FILE_PATH, "w");
    fprintf(f, "{\"old\": \"content that should be overwritten\"}");
    fclose(f);
    
    // Create test users
    const char *json = "["
        "{\"full_name\": \"New User\", \"gamertag\": \"newuser\", \"player_hours\": 100, \"ssn\": \"1111-111111\", \"email\": \"new@example.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true}"
    "]";
    create_test_users_file(json);
    
    // Act
    int result = logic_generate_top_users_file();
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify old content is gone
    char *content = read_output_file();
    TEST_ASSERT_NOT_NULL(content);
    TEST_ASSERT_NULL(strstr(content, "old"));
    
    // Verify new content is there
    cJSON *json_result = cJSON_Parse(content);
    free(content);
    
    TEST_ASSERT_NOT_NULL(json_result);
    TEST_ASSERT_TRUE(cJSON_IsArray(json_result));
    TEST_ASSERT_EQUAL_INT(1, cJSON_GetArraySize(json_result));
    
    cJSON_Delete(json_result);
}

void test_logic_generate_top_users_file_preserves_all_fields(void) {
    // Arrange: Create user with all fields
    const char *json = "["
        "{"
            "\"full_name\": \"Complete User\","
            "\"gamertag\": \"complete123\","
            "\"player_hours\": 12345,"
            "\"ssn\": \"1234-567890\","
            "\"email\": \"complete@example.com\","
            "\"subscription_start_date\": \"15.06.2025\","
            "\"subscription_end_date\": \"15.12.2025\","
            "\"is_subscribed\": true"
        "}"
    "]";
    create_test_users_file(json);
    
    // Act
    int result = logic_generate_top_users_file();
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify all fields are preserved
    char *content = read_output_file();
    TEST_ASSERT_NOT_NULL(content);
    
    cJSON *json_result = cJSON_Parse(content);
    free(content);
    
    TEST_ASSERT_NOT_NULL(json_result);
    cJSON *user = cJSON_GetArrayItem(json_result, 0);
    
    TEST_ASSERT_EQUAL_STRING("Complete User", cJSON_GetObjectItem(user, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("complete123", cJSON_GetObjectItem(user, "gamertag")->valuestring);
    TEST_ASSERT_EQUAL_INT(12345, cJSON_GetObjectItem(user, "player_hours")->valueint);
    TEST_ASSERT_EQUAL_STRING("1234-567890", cJSON_GetObjectItem(user, "ssn")->valuestring);
    TEST_ASSERT_EQUAL_STRING("complete@example.com", cJSON_GetObjectItem(user, "email")->valuestring);
    TEST_ASSERT_EQUAL_STRING("15.06.2025", cJSON_GetObjectItem(user, "subscription_start_date")->valuestring);
    TEST_ASSERT_EQUAL_STRING("15.12.2025", cJSON_GetObjectItem(user, "subscription_end_date")->valuestring);
    TEST_ASSERT_TRUE(cJSON_IsTrue(cJSON_GetObjectItem(user, "is_subscribed")));
    
    cJSON_Delete(json_result);
}

void test_logic_generate_top_users_file_special_characters(void) {
    // Arrange: Create users with special characters
    const char *json = "["
        "{"
            "\"full_name\": \"José María Müller-O'Brien\","
            "\"gamertag\": \"spëcîál_üsér@123\","
            "\"player_hours\": 200,"
            "\"ssn\": \"1234-567890\","
            "\"email\": \"josé.maría@ñoño-domain.com\","
            "\"subscription_start_date\": \"01.01.2025\","
            "\"subscription_end_date\": \"01.07.2025\","
            "\"is_subscribed\": true"
        "},"
        "{"
            "\"full_name\": \"李明 (Li Ming)\","
            "\"gamertag\": \"李明123\","
            "\"player_hours\": 100,"
            "\"ssn\": \"8888-888888\","
            "\"email\": \"liming@例え.jp\","
            "\"subscription_start_date\": \"01.02.2025\","
            "\"subscription_end_date\": \"01.08.2025\","
            "\"is_subscribed\": false"
        "}"
    "]";
    create_test_users_file(json);
    
    // Act
    int result = logic_generate_top_users_file();
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Verify special characters are preserved
    char *content = read_output_file();
    TEST_ASSERT_NOT_NULL(content);
    
    cJSON *json_result = cJSON_Parse(content);
    free(content);
    
    TEST_ASSERT_NOT_NULL(json_result);
    TEST_ASSERT_EQUAL_INT(2, cJSON_GetArraySize(json_result));
    
    // First user should be José (200 hours)
    cJSON *user1 = cJSON_GetArrayItem(json_result, 0);
    TEST_ASSERT_EQUAL_STRING("José María Müller-O'Brien", cJSON_GetObjectItem(user1, "full_name")->valuestring);
    TEST_ASSERT_EQUAL_STRING("spëcîál_üsér@123", cJSON_GetObjectItem(user1, "gamertag")->valuestring);
    
    cJSON_Delete(json_result);
}

void test_logic_generate_top_users_file_missing_users_file(void) {
    // Arrange: Ensure no users file exists
    remove(USERS_JSON_PATH);
    
    // Act
    int result = logic_generate_top_users_file();
    
    // Assert: Should succeed with empty array
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    char *content = read_output_file();
    TEST_ASSERT_NOT_NULL(content);
    
    cJSON *json = cJSON_Parse(content);
    free(content);
    
    TEST_ASSERT_NOT_NULL(json);
    TEST_ASSERT_TRUE(cJSON_IsArray(json));
    TEST_ASSERT_EQUAL_INT(0, cJSON_GetArraySize(json));
    
    cJSON_Delete(json);
}

void test_logic_generate_top_users_file_valid_json_format(void) {
    // Arrange: Create a few users
    const char *json = "["
        "{\"full_name\": \"User 1\", \"gamertag\": \"user1\", \"player_hours\": 100, \"ssn\": \"1111-111111\", \"email\": \"user1@test.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": true},"
        "{\"full_name\": \"User 2\", \"gamertag\": \"user2\", \"player_hours\": 200, \"ssn\": \"2222-222222\", \"email\": \"user2@test.com\", \"subscription_start_date\": \"01.01.2025\", \"subscription_end_date\": \"01.07.2025\", \"is_subscribed\": false}"
    "]";
    create_test_users_file(json);
    
    // Act
    int result = logic_generate_top_users_file();
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    
    // Read and parse to verify valid JSON
    char *content = read_output_file();
    TEST_ASSERT_NOT_NULL(content);
    
    // Should be valid JSON
    cJSON *json_result = cJSON_Parse(content);
    TEST_ASSERT_NOT_NULL(json_result);
    TEST_ASSERT_TRUE(cJSON_IsArray(json_result));
    
    // Verify it's pretty-printed (contains newlines)
    TEST_ASSERT_NOT_NULL(strstr(content, "\n"));
    
    free(content);
    cJSON_Delete(json_result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_generate_top_users_file_success_with_exactly_10_users);
    RUN_TEST(test_logic_generate_top_users_file_more_than_10_users);
    RUN_TEST(test_logic_generate_top_users_file_less_than_10_users);
    RUN_TEST(test_logic_generate_top_users_file_empty_users);
    RUN_TEST(test_logic_generate_top_users_file_single_user);
    RUN_TEST(test_logic_generate_top_users_file_same_hours);
    RUN_TEST(test_logic_generate_top_users_file_overwrites_existing);
    RUN_TEST(test_logic_generate_top_users_file_preserves_all_fields);
    RUN_TEST(test_logic_generate_top_users_file_special_characters);
    RUN_TEST(test_logic_generate_top_users_file_missing_users_file);
    RUN_TEST(test_logic_generate_top_users_file_valid_json_format);
    return UNITY_END();
}