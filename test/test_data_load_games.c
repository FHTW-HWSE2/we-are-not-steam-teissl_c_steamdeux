#include "unity.h"
#include "cJSON.h"
#include "../../inc/data/data.h"
#include "../../inc/error.h"
#include "../../src/data/game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_GAMES_FILE "test/games_load_test.json"

void setUp(void) {
    // Clean up test file before each test
    remove(TEST_GAMES_FILE);
}

void tearDown(void) {
    // Clean up test file after each test
    remove(TEST_GAMES_FILE);
}

// Helper function to create test JSON file
static void create_test_games_file(const char *content) {
    FILE *f = fopen(TEST_GAMES_FILE, "w");
    fputs(content, f);
    fclose(f);
}

void test_data_load_games_success_with_valid_games(void) {
    // Arrange: Create valid games JSON
    const char *json = "{"
        "\"games\": ["
            "{"
                "\"title\": \"Test Game 1\","
                "\"description\": \"First test game\","
                "\"version\": \"1.0\","
                "\"mode\": \"Singleplayer\","
                "\"current_streams\": 5"
            "},"
            "{"
                "\"title\": \"Test Game 2\","
                "\"description\": \"Second test game\","
                "\"version\": \"2.0\","
                "\"mode\": \"Multiplayer\","
                "\"current_streams\": 10"
            "}"
        "]"
    "}";
    create_test_games_file(json);
    
    Game *games = NULL;
    int count = -1;
    
    // Act
    int result = data_load_games(TEST_GAMES_FILE, &games, &count);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(games);
    TEST_ASSERT_EQUAL_INT(2, count);
    
    // Check first game
    TEST_ASSERT_EQUAL_INT(1, games[0].id);
    TEST_ASSERT_EQUAL_STRING("Test Game 1", games[0].title);
    TEST_ASSERT_EQUAL_STRING("First test game", games[0].description);
    TEST_ASSERT_EQUAL_STRING("1.0", games[0].version);
    TEST_ASSERT_EQUAL_STRING("Singleplayer", games[0].mode);
    TEST_ASSERT_EQUAL_INT(5, games[0].current_streams);
    
    // Check second game
    TEST_ASSERT_EQUAL_INT(2, games[1].id);
    TEST_ASSERT_EQUAL_STRING("Test Game 2", games[1].title);
    TEST_ASSERT_EQUAL_STRING("Second test game", games[1].description);
    TEST_ASSERT_EQUAL_STRING("2.0", games[1].version);
    TEST_ASSERT_EQUAL_STRING("Multiplayer", games[1].mode);
    TEST_ASSERT_EQUAL_INT(10, games[1].current_streams);
    
    // Cleanup
    free(games);
}

void test_data_load_games_empty_array_success(void) {
    // Arrange: Create empty games array
    const char *json = "{\"games\": []}";
    create_test_games_file(json);
    
    Game *games = (Game *)0xDEADBEEF; // Non-NULL to check it gets set to NULL
    int count = -1;
    
    // Act
    int result = data_load_games(TEST_GAMES_FILE, &games, &count);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NULL(games);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_data_load_games_missing_file_returns_error(void) {
    // Arrange: No file created
    Game *games = NULL;
    int count = -1;
    
    // Act
    int result = data_load_games("nonexistent.json", &games, &count);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
    TEST_ASSERT_NULL(games);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_data_load_games_invalid_json_returns_error(void) {
    // Arrange: Create malformed JSON
    const char *json = "{\"games\": [corrupted json";
    create_test_games_file(json);
    
    Game *games = NULL;
    int count = -1;
    
    // Act
    int result = data_load_games(TEST_GAMES_FILE, &games, &count);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
    TEST_ASSERT_NULL(games);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_data_load_games_not_object_returns_error(void) {
    // Arrange: Root is array instead of object
    const char *json = "[{\"title\": \"Game\"}]";
    create_test_games_file(json);
    
    Game *games = NULL;
    int count = -1;
    
    // Act
    int result = data_load_games(TEST_GAMES_FILE, &games, &count);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
    TEST_ASSERT_NULL(games);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_data_load_games_missing_games_key_returns_error(void) {
    // Arrange: No "games" key
    const char *json = "{\"other_key\": []}";
    create_test_games_file(json);
    
    Game *games = NULL;
    int count = -1;
    
    // Act
    int result = data_load_games(TEST_GAMES_FILE, &games, &count);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
    TEST_ASSERT_NULL(games);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_data_load_games_games_not_array_returns_error(void) {
    // Arrange: "games" is not an array
    const char *json = "{\"games\": \"not an array\"}";
    create_test_games_file(json);
    
    Game *games = NULL;
    int count = -1;
    
    // Act
    int result = data_load_games(TEST_GAMES_FILE, &games, &count);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
    TEST_ASSERT_NULL(games);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_data_load_games_missing_fields_uses_defaults(void) {
    // Arrange: Game with missing optional fields
    const char *json = "{"
        "\"games\": ["
            "{"
                "\"title\": \"Minimal Game\""
                // Missing: description, version, mode, current_streams
            "}"
        "]"
    "}";
    create_test_games_file(json);
    
    Game *games = NULL;
    int count = -1;
    
    // Act
    int result = data_load_games(TEST_GAMES_FILE, &games, &count);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(games);
    TEST_ASSERT_EQUAL_INT(1, count);
    
    // Check defaults
    TEST_ASSERT_EQUAL_STRING("Minimal Game", games[0].title);
    TEST_ASSERT_EQUAL_STRING("", games[0].description);
    TEST_ASSERT_EQUAL_STRING("", games[0].version);
    TEST_ASSERT_EQUAL_STRING("", games[0].mode);
    TEST_ASSERT_EQUAL_INT(0, games[0].current_streams);
    
    free(games);
}

void test_data_load_games_null_parameters_returns_error(void) {
    // Test NULL filename
    Game *games = NULL;
    int count = 0;
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, data_load_games(NULL, &games, &count));
    
    // Test NULL games_out
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, data_load_games(TEST_GAMES_FILE, NULL, &count));
    
    // Test NULL count_out
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, data_load_games(TEST_GAMES_FILE, &games, NULL));
}

void test_data_load_games_truncates_long_strings(void) {
    // Arrange: Create game with strings exceeding field sizes
    const char *json = "{"
        "\"games\": ["
            "{"
                "\"title\": \"This is a very long title that exceeds the 100 character limit for the title field and should be truncated properly without buffer overflow\","
                "\"description\": \"This is an extremely long description that goes way beyond the 256 character limit. It contains lots of text to ensure that the truncation happens correctly. The function should handle this gracefully by truncating the string and ensuring null termination. This extra text should not appear in the loaded game structure.\","
                "\"version\": \"1.0.0.0.0.0.0.0.0.0-extra-long\","
                "\"mode\": \"VeryLongGameModeNameThatExceedsLimit\","
                "\"current_streams\": 999"
            "}"
        "]"
    "}";
    create_test_games_file(json);
    
    Game *games = NULL;
    int count = -1;
    
    // Act
    int result = data_load_games(TEST_GAMES_FILE, &games, &count);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(games);
    
    // Check truncation
    TEST_ASSERT_EQUAL_INT(99, strlen(games[0].title)); // 100 - 1 for null terminator
    TEST_ASSERT_EQUAL_INT(255, strlen(games[0].description)); // 256 - 1
    TEST_ASSERT_EQUAL_INT(9, strlen(games[0].version)); // 10 - 1
    TEST_ASSERT_EQUAL_INT(19, strlen(games[0].mode)); // 20 - 1
    
    // Verify null termination
    TEST_ASSERT_EQUAL_INT('\0', games[0].title[99]);
    TEST_ASSERT_EQUAL_INT('\0', games[0].description[255]);
    TEST_ASSERT_EQUAL_INT('\0', games[0].version[9]);
    TEST_ASSERT_EQUAL_INT('\0', games[0].mode[19]);
    
    free(games);
}

void test_data_load_games_multiple_games_correct_ids(void) {
    // Arrange: Create multiple games
    const char *json = "{"
        "\"games\": ["
            "{\"title\": \"Game A\", \"current_streams\": 1},"
            "{\"title\": \"Game B\", \"current_streams\": 2},"
            "{\"title\": \"Game C\", \"current_streams\": 3},"
            "{\"title\": \"Game D\", \"current_streams\": 4},"
            "{\"title\": \"Game E\", \"current_streams\": 5}"
        "]"
    "}";
    create_test_games_file(json);
    
    Game *games = NULL;
    int count = -1;
    
    // Act
    int result = data_load_games(TEST_GAMES_FILE, &games, &count);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_EQUAL_INT(5, count);
    
    // Check IDs are assigned correctly (1-based)
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_EQUAL_INT(i + 1, games[i].id);
    }
    
    // Check data integrity
    TEST_ASSERT_EQUAL_STRING("Game A", games[0].title);
    TEST_ASSERT_EQUAL_INT(1, games[0].current_streams);
    TEST_ASSERT_EQUAL_STRING("Game E", games[4].title);
    TEST_ASSERT_EQUAL_INT(5, games[4].current_streams);
    
    free(games);
}

void test_data_load_games_special_characters(void) {
    // Arrange: Games with special characters
    const char *json = "{"
        "\"games\": ["
            "{"
                "\"title\": \"Über-Game: Ñoño's Adventure\","
                "\"description\": \"Contains UTF-8: €, £, ¥, émojis: 🎮🎯\","
                "\"version\": \"β-1.0\","
                "\"mode\": \"Co-op & PvP\","
                "\"current_streams\": 42"
            "}"
        "]"
    "}";
    create_test_games_file(json);
    
    Game *games = NULL;
    int count = -1;
    
    // Act
    int result = data_load_games(TEST_GAMES_FILE, &games, &count);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(games);
    TEST_ASSERT_EQUAL_INT(1, count);
    
    // Check special characters are preserved
    TEST_ASSERT_EQUAL_STRING("Über-Game: Ñoño's Adventure", games[0].title);
    TEST_ASSERT_EQUAL_STRING("Contains UTF-8: €, £, ¥, émojis: 🎮🎯", games[0].description);
    TEST_ASSERT_EQUAL_STRING("β-1.0", games[0].version);
    TEST_ASSERT_EQUAL_STRING("Co-op & PvP", games[0].mode);
    
    free(games);
}

void test_data_load_games_wrong_field_types_handles_gracefully(void) {
    // Arrange: Wrong types for fields
    const char *json = "{"
        "\"games\": ["
            "{"
                "\"title\": 123,"                    // Number instead of string
                "\"description\": true,"              // Boolean instead of string
                "\"version\": null,"                  // Null instead of string
                "\"mode\": [\"array\"],"              // Array instead of string
                "\"current_streams\": \"not a number\"" // String instead of number
            "}"
        "]"
    "}";
    create_test_games_file(json);
    
    Game *games = NULL;
    int count = -1;
    
    // Act
    int result = data_load_games(TEST_GAMES_FILE, &games, &count);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(games);
    TEST_ASSERT_EQUAL_INT(1, count);
    
    // Wrong types should result in empty strings/0
    TEST_ASSERT_EQUAL_STRING("", games[0].title);
    TEST_ASSERT_EQUAL_STRING("", games[0].description);
    TEST_ASSERT_EQUAL_STRING("", games[0].version);
    TEST_ASSERT_EQUAL_STRING("", games[0].mode);
    TEST_ASSERT_EQUAL_INT(0, games[0].current_streams);
    
    free(games);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_data_load_games_success_with_valid_games);
    RUN_TEST(test_data_load_games_empty_array_success);
    RUN_TEST(test_data_load_games_missing_file_returns_error);
    RUN_TEST(test_data_load_games_invalid_json_returns_error);
    RUN_TEST(test_data_load_games_not_object_returns_error);
    RUN_TEST(test_data_load_games_missing_games_key_returns_error);
    RUN_TEST(test_data_load_games_games_not_array_returns_error);
    RUN_TEST(test_data_load_games_missing_fields_uses_defaults);
    RUN_TEST(test_data_load_games_null_parameters_returns_error);
    RUN_TEST(test_data_load_games_truncates_long_strings);
    RUN_TEST(test_data_load_games_multiple_games_correct_ids);
    RUN_TEST(test_data_load_games_special_characters);
    RUN_TEST(test_data_load_games_wrong_field_types_handles_gracefully);
    return UNITY_END();
}