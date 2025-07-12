#include "unity.h"
#include "../../cJSON/cJSON.h"
#include "../mocks/mock_data.h"
#include "mock_presentation.h"  // Assumes mock_presentation.c is updated with the mocks
#include "../../inc/logic/logic.h"  // For logic_display_games_formatted and helpers
#include "../../src/data/game.h"   // For Game struct definition

void setUp(void) {
    // Reset mocks before each test
    mock_data_get_all_users_called = 0;  // Not used here, but reset for consistency
    mock_data_get_all_users_return = ERR_SUCCESS;
    mock_data_get_all_users_users_out = NULL;

    mock_presentation_show_error_called = 0;
    if (mock_presentation_show_error_message) {
        free(mock_presentation_show_error_message);
        mock_presentation_show_error_message = NULL;
    }

    mock_presentation_display_users_formatted_called = 0;
    if (mock_presentation_display_users_formatted_lines) {
        for (int i = 0; i < mock_presentation_display_users_formatted_count; ++i) {
            free(mock_presentation_display_users_formatted_lines[i]);
        }
        free(mock_presentation_display_users_formatted_lines);
        mock_presentation_display_users_formatted_lines = NULL;
    }
    mock_presentation_display_users_formatted_count = 0;
}

void tearDown(void) {
    // Cleanup any leftover mocks
    if (mock_presentation_show_error_message) {
        free(mock_presentation_show_error_message);
        mock_presentation_show_error_message = NULL;
    }
    if (mock_presentation_display_users_formatted_lines) {
        for (int i = 0; i < mock_presentation_display_users_formatted_count; ++i) {
            free(mock_presentation_display_users_formatted_lines[i]);
        }
        free(mock_presentation_display_users_formatted_lines);
        mock_presentation_display_users_formatted_lines = NULL;
    }
}

// Helper to create a test game array
Game* create_test_games(int count) {
    Game* games = malloc(count * sizeof(Game));
    if (!games) return NULL;

    for (int i = 0; i < count; ++i) {
        games[i].id = i + 1;
        snprintf(games[i].title, sizeof(games[i].title), "Test Game %d", i + 1);
        snprintf(games[i].description, sizeof(games[i].description), "Description for game %d", i + 1);
        snprintf(games[i].version, sizeof(games[i].version), "1.0.%d", i);
        snprintf(games[i].mode, sizeof(games[i].mode), "Mode %d", i + 1);
        games[i].current_streams = 10 * (i + 1);
    }

    return games;
}

// Test: No games (game_count=0) -> shows error, no display
void test_logic_display_games_formatted_no_games_shows_error(void) {
    Game* games = NULL;
    int game_count = 0;

    logic_display_games_formatted(games, game_count);

    TEST_ASSERT_EQUAL_INT(1, mock_presentation_show_error_called);
    TEST_ASSERT_EQUAL_STRING("Could not format game list.", mock_presentation_show_error_message);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_display_users_formatted_called);
}

// Test: Successful display with one game -> calls display with formatted lines
void test_logic_display_games_formatted_successful_display(void) {
    int game_count = 1;
    Game* games = create_test_games(game_count);

    logic_display_games_formatted(games, game_count);

    TEST_ASSERT_EQUAL_INT(0, mock_presentation_show_error_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_display_users_formatted_called);
    TEST_ASSERT_EQUAL_INT(7, mock_presentation_display_users_formatted_count);  // Expected lines: 6 per game + 1 separator

    // Verify some sample lines (adjust indices based on formatting in logic_get_game_lines_for_display)
    TEST_ASSERT_EQUAL_STRING("Game 1:", mock_presentation_display_users_formatted_lines[0]);
    TEST_ASSERT_EQUAL_STRING("Title: Test Game 1", mock_presentation_display_users_formatted_lines[1]);
    TEST_ASSERT_EQUAL_STRING("Description: Description for game 1", mock_presentation_display_users_formatted_lines[2]);
    TEST_ASSERT_EQUAL_STRING("Version: 1.0.0", mock_presentation_display_users_formatted_lines[3]);
    TEST_ASSERT_EQUAL_STRING("Mode: Mode 1", mock_presentation_display_users_formatted_lines[4]);
    TEST_ASSERT_EQUAL_STRING("Current Streams: 10", mock_presentation_display_users_formatted_lines[5]);
    TEST_ASSERT_EQUAL_STRING("--------------------------", mock_presentation_display_users_formatted_lines[6]);

    free(games);
}

// Test: Multiple games -> correct number of lines and calls display
void test_logic_display_games_formatted_multiple_games(void) {
    int game_count = 2;
    Game* games = create_test_games(game_count);

    logic_display_games_formatted(games, game_count);

    TEST_ASSERT_EQUAL_INT(0, mock_presentation_show_error_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_display_users_formatted_called);
    TEST_ASSERT_EQUAL_INT(14, mock_presentation_display_users_formatted_count);  // 7 lines per game * 2

    // Spot-check a few lines
    TEST_ASSERT_EQUAL_STRING("Game 1:", mock_presentation_display_users_formatted_lines[0]);
    TEST_ASSERT_EQUAL_STRING("Game 2:", mock_presentation_display_users_formatted_lines[7]);
    TEST_ASSERT_EQUAL_STRING("Current Streams: 20", mock_presentation_display_users_formatted_lines[12]);

    free(games);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_display_games_formatted_no_games_shows_error);
    RUN_TEST(test_logic_display_games_formatted_successful_display);
    RUN_TEST(test_logic_display_games_formatted_multiple_games);
    return UNITY_END();
}