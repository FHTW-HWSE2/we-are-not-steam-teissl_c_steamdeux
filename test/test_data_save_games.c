#include "unity.h"
#include "cJSON.h"
#include "../../inc/data/data.h"
#include "../../inc/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Override the GAMES_JSON_PATH after includes
#ifdef GAMES_JSON_PATH
#undef GAMES_JSON_PATH
#endif
#define GAMES_JSON_PATH "test/games_save_test.json"

void setUp(void) {
    // Clean up test file before each test
    remove(GAMES_JSON_PATH);
}

void tearDown(void) {
    // Clean up test file after each test
    remove(GAMES_JSON_PATH);
}

void test_data_save_games_success_returns_ERR_SUCCESS(void) {
    // Arrange: Create test games array
    Game games[2];
    
    games[0].id = 1;
    strcpy(games[0].title, "Test Game 1");
    strcpy(games[0].description, "Test description 1");
    strcpy(games[0].version, "1.0");
    strcpy(games[0].mode, "Singleplayer");
    games[0].current_streams = 5;
    
    games[1].id = 2;
    strcpy(games[1].title, "Test Game 2");
    strcpy(games[1].description, "Test description 2");
    strcpy(games[1].version, "2.0");
    strcpy(games[1].mode, "Multiplayer");
    games[1].current_streams = 10;

    // Act: Save games
    int result = data_save_games(GAMES_JSON_PATH, games, 2);

    // Assert: Check success
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    // Verify file was created and contains expected data
    FILE *f = fopen(GAMES_JSON_PATH, "r");
    TEST_ASSERT_NOT_NULL(f);
    
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *data = malloc(len + 1);
    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);

    cJSON *json = cJSON_Parse(data);
    TEST_ASSERT_NOT_NULL(json);
    
    cJSON *games_array = cJSON_GetObjectItem(json, "games");
    TEST_ASSERT_NOT_NULL(games_array);
    TEST_ASSERT_TRUE(cJSON_IsArray(games_array));
    TEST_ASSERT_EQUAL_INT(2, cJSON_GetArraySize(games_array));

    // Check first game
    cJSON *game1 = cJSON_GetArrayItem(games_array, 0);
    TEST_ASSERT_EQUAL_STRING("Test Game 1", cJSON_GetObjectItem(game1, "title")->valuestring);
    TEST_ASSERT_EQUAL_STRING("Test description 1", cJSON_GetObjectItem(game1, "description")->valuestring);
    TEST_ASSERT_EQUAL_STRING("1.0", cJSON_GetObjectItem(game1, "version")->valuestring);
    TEST_ASSERT_EQUAL_STRING("Singleplayer", cJSON_GetObjectItem(game1, "mode")->valuestring);
    TEST_ASSERT_EQUAL_INT(5, cJSON_GetObjectItem(game1, "current_streams")->valueint);

    free(data);
    cJSON_Delete(json);
}

void test_data_save_games_empty_array_success(void) {
    // Act: Save empty games array
    int result = data_save_games(GAMES_JSON_PATH, NULL, 0);

    // Assert: Should still succeed
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    // Verify file contains empty games array
    FILE *f = fopen(GAMES_JSON_PATH, "r");
    TEST_ASSERT_NOT_NULL(f);
    
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *data = malloc(len + 1);
    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);

    cJSON *json = cJSON_Parse(data);
    TEST_ASSERT_NOT_NULL(json);
    
    cJSON *games_array = cJSON_GetObjectItem(json, "games");
    TEST_ASSERT_NOT_NULL(games_array);
    TEST_ASSERT_TRUE(cJSON_IsArray(games_array));
    TEST_ASSERT_EQUAL_INT(0, cJSON_GetArraySize(games_array));

    free(data);
    cJSON_Delete(json);
}

void test_data_save_games_single_game_success(void) {
    // Arrange: Create single game
    Game games[1];
    games[0].id = 1;
    strcpy(games[0].title, "Single Game");
    strcpy(games[0].description, "Only one game");
    strcpy(games[0].version, "1.5");
    strcpy(games[0].mode, "Online");
    games[0].current_streams = 3;

    // Act: Save single game
    int result = data_save_games(GAMES_JSON_PATH, games, 1);

    // Assert: Should succeed
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    // Verify file contains one game
    FILE *f = fopen(GAMES_JSON_PATH, "r");
    TEST_ASSERT_NOT_NULL(f);
    
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *data = malloc(len + 1);
    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);

    cJSON *json = cJSON_Parse(data);
    TEST_ASSERT_NOT_NULL(json);
    
    cJSON *games_array = cJSON_GetObjectItem(json, "games");
    TEST_ASSERT_NOT_NULL(games_array);
    TEST_ASSERT_TRUE(cJSON_IsArray(games_array));
    TEST_ASSERT_EQUAL_INT(1, cJSON_GetArraySize(games_array));

    // Check game content
    cJSON *game = cJSON_GetArrayItem(games_array, 0);
    TEST_ASSERT_EQUAL_STRING("Single Game", cJSON_GetObjectItem(game, "title")->valuestring);
    TEST_ASSERT_EQUAL_STRING("Only one game", cJSON_GetObjectItem(game, "description")->valuestring);
    TEST_ASSERT_EQUAL_STRING("1.5", cJSON_GetObjectItem(game, "version")->valuestring);
    TEST_ASSERT_EQUAL_STRING("Online", cJSON_GetObjectItem(game, "mode")->valuestring);
    TEST_ASSERT_EQUAL_INT(3, cJSON_GetObjectItem(game, "current_streams")->valueint);

    free(data);
    cJSON_Delete(json);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_data_save_games_success_returns_ERR_SUCCESS);
    RUN_TEST(test_data_save_games_empty_array_success);
    RUN_TEST(test_data_save_games_single_game_success);
    return UNITY_END();
}