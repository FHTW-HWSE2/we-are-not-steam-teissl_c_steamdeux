#include "unity.h"
#include "../../inc/logic/logic.h"
#include "../../src/data/game.h"    // For Game struct

// No mocks needed as this function doesn't call other layers

void setUp(void) {
    // Nothing needed
}

void tearDown(void) {
    // Nothing needed
}

// Helper to create test games array
Game* create_test_games(int count) {
    Game* games = malloc(sizeof(Game) * count);
    if (!games) return NULL;
    for (int i = 0; i < count; ++i) {
        games[i].id = i + 1;
        snprintf(games[i].title, sizeof(games[i].title), "Original Title %d", i + 1);
        // Fill other fields if needed, but not relevant here
        snprintf(games[i].description, sizeof(games[i].description), "Desc %d", i + 1);
        snprintf(games[i].version, sizeof(games[i].version), "1.0");
        snprintf(games[i].mode, sizeof(games[i].mode), "Mode");
        games[i].current_streams = 0;
    }
    return games;
}

// Test: Successful edit of existing game
void test_logic_edit_game_success(void) {
    int game_count = 3;
    Game* games = create_test_games(game_count);
    TEST_ASSERT_NOT_NULL(games);

    const char* new_title = "New Title";
    int result = logic_edit_game(games, game_count, 2, new_title);

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_EQUAL_STRING("New Title", games[1].title);  // ID 2 is index 1

    free(games);
}

// Test: Game not found
void test_logic_edit_game_not_found(void) {
    int game_count = 3;
    Game* games = create_test_games(game_count);
    TEST_ASSERT_NOT_NULL(games);

    const char* new_title = "New Title";
    int result = logic_edit_game(games, game_count, 4, new_title);  // ID 4 doesn't exist

    TEST_ASSERT_EQUAL_INT(ERR_USER_NOT_FOUND, result);
    // Verify no titles changed
    TEST_ASSERT_EQUAL_STRING("Original Title 1", games[0].title);
    TEST_ASSERT_EQUAL_STRING("Original Title 2", games[1].title);
    TEST_ASSERT_EQUAL_STRING("Original Title 3", games[2].title);

    free(games);
}

// Test: Empty array (game_count=0)
void test_logic_edit_game_empty_array(void) {
    Game* games = NULL;
    int game_count = 0;

    const char* new_title = "New Title";
    int result = logic_edit_game(games, game_count, 1, new_title);

    TEST_ASSERT_EQUAL_INT(ERR_USER_NOT_FOUND, result);
    // No crash, games unchanged (NULL)
}

// Test: New title longer than buffer (truncation)
void test_logic_edit_game_long_title(void) {
    int game_count = 1;
    Game* games = create_test_games(game_count);
    TEST_ASSERT_NOT_NULL(games);

    char long_title[sizeof(games[0].title) + 10];
    memset(long_title, 'A', sizeof(long_title) - 1);
    long_title[sizeof(long_title) - 1] = '\0';

    int result = logic_edit_game(games, game_count, 1, long_title);

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    // Check truncation: title should be max length -1 chars + '\0'
    char expected[sizeof(games[0].title)];
    memset(expected, 'A', sizeof(expected) - 1);
    expected[sizeof(expected) - 1] = '\0';
    TEST_ASSERT_EQUAL_STRING(expected, games[0].title);

    free(games);
}

// Test: New title empty string
void test_logic_edit_game_empty_title(void) {
    int game_count = 1;
    Game* games = create_test_games(game_count);
    TEST_ASSERT_NOT_NULL(games);

    const char* new_title = "";
    int result = logic_edit_game(games, game_count, 1, new_title);

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_EQUAL_STRING("", games[0].title);

    free(games);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_edit_game_success);
    RUN_TEST(test_logic_edit_game_not_found);
    RUN_TEST(test_logic_edit_game_empty_array);
    RUN_TEST(test_logic_edit_game_long_title);
    RUN_TEST(test_logic_edit_game_empty_title);
    return UNITY_END();
}