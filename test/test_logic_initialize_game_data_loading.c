#include "unity.h"
#include "../../cJSON/cJSON.h"
#include "mock_data.h"
#include "mock_presentation.h"  // Include if needed, but not used here
#include "../../inc/logic/logic.h"  // For logic_initialize_game_data_loading
#include "../../src/data/game.h"    // For Game struct

// Mock tracking variables for data_load_games (add to mock_data.h if creating header)
extern int mock_data_load_games_called;
extern int mock_data_load_games_return;
extern Game* mock_data_load_games_games_out;
extern int mock_data_load_games_count_out;

void setUp(void) {
    // Reset mocks before each test
    mock_data_load_games_called = 0;
    mock_data_load_games_return = ERR_SUCCESS;
    if (mock_data_load_games_games_out) {
        free(mock_data_load_games_games_out);
        mock_data_load_games_games_out = NULL;
    }
    mock_data_load_games_count_out = 0;
}

void tearDown(void) {
    // Cleanup
    if (mock_data_load_games_games_out) {
        free(mock_data_load_games_games_out);
        mock_data_load_games_games_out = NULL;
    }
}

// Helper to create test games array
Game* create_test_games(int count, int* out_count) {
    Game* games = malloc(sizeof(Game) * count);
    if (!games) return NULL;
    for (int i = 0; i < count; ++i) {
        games[i].id = i + 1;
        snprintf(games[i].title, sizeof(games[i].title), "Game %d", i + 1);
        snprintf(games[i].description, sizeof(games[i].description), "Desc %d", i + 1);
        snprintf(games[i].version, sizeof(games[i].version), "1.0");
        snprintf(games[i].mode, sizeof(games[i].mode), "Singleplayer");
        games[i].current_streams = i * 10;
    }
    *out_count = count;
    return games;
}

// Test: Successful load with games
void test_logic_initialize_game_data_loading_success(void) {
    const char* filename = "test_games.json";
    Game* games = NULL;
    int game_count = 0;

    // Setup mock
    mock_data_load_games_games_out = create_test_games(2, &mock_data_load_games_count_out);
    mock_data_load_games_return = ERR_SUCCESS;

    int result = logic_initialize_game_data_loading(filename, &games, &game_count);

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_EQUAL_INT(1, mock_data_load_games_called);
    TEST_ASSERT_EQUAL_INT(2, game_count);
    TEST_ASSERT_NOT_NULL(games);
    TEST_ASSERT_EQUAL_STRING("Game 1", games[0].title);
    TEST_ASSERT_EQUAL_STRING("Game 2", games[1].title);

    // Cleanup (tearDown will free)
}

// Test: Load failure (e.g., storage error)
void test_logic_initialize_game_data_loading_failure(void) {
    const char* filename = "invalid.json";
    Game* games = NULL;
    int game_count = 0;

    // Setup mock
    mock_data_load_games_return = ERR_STORAGE_FAILURE;

    int result = logic_initialize_game_data_loading(filename, &games, &game_count);

    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
    TEST_ASSERT_EQUAL_INT(1, mock_data_load_games_called);
    TEST_ASSERT_NULL(games);
    TEST_ASSERT_EQUAL_INT(0, game_count);
}

// Test: Empty games list (success but no games)
void test_logic_initialize_game_data_loading_empty(void) {
    const char* filename = "empty.json";
    Game* games = NULL;
    int game_count = 0;

    // Setup mock
    mock_data_load_games_games_out = NULL;
    mock_data_load_games_count_out = 0;
    mock_data_load_games_return = ERR_SUCCESS;

    int result = logic_initialize_game_data_loading(filename, &games, &game_count);

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_EQUAL_INT(1, mock_data_load_games_called);
    TEST_ASSERT_NULL(games);
    TEST_ASSERT_EQUAL_INT(0, game_count);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_initialize_game_data_loading_success);
    RUN_TEST(test_logic_initialize_game_data_loading_failure);
    RUN_TEST(test_logic_initialize_game_data_loading_empty);
    return UNITY_END();
}