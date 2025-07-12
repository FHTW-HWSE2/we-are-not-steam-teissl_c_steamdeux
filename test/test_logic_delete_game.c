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
        snprintf(games[i].title, sizeof(games[i].title), "Title %d", i + 1);
        // Fill other fields if needed, but not relevant here
        snprintf(games[i].description, sizeof(games[i].description), "Desc %d", i + 1);
        snprintf(games[i].version, sizeof(games[i].version), "1.0");
        snprintf(games[i].mode, sizeof(games[i].mode), "Mode");
        games[i].current_streams = 0;
    }
    return games;
}

// Test: Successful deletion of a game in the middle
void test_logic_delete_game_success_middle(void) {
    int game_count = 3;
    Game* games = create_test_games(game_count);
    TEST_ASSERT_NOT_NULL(games);

    int result = logic_delete_game(games, &game_count, 2);  // Delete ID 2 (index 1)

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_EQUAL_INT(2, game_count);
    TEST_ASSERT_EQUAL_STRING("Title 1", games[0].title);
    TEST_ASSERT_EQUAL_STRING("Title 3", games[1].title);  // Shifted

    free(games);
}

// Test: Successful deletion of the first game
void test_logic_delete_game_success_first(void) {
    int game_count = 3;
    Game* games = create_test_games(game_count);
    TEST_ASSERT_NOT_NULL(games);

    int result = logic_delete_game(games, &game_count, 1);  // Delete ID 1 (index 0)

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_EQUAL_INT(2, game_count);
    TEST_ASSERT_EQUAL_STRING("Title 2", games[0].title);
    TEST_ASSERT_EQUAL_STRING("Title 3", games[1].title);

    free(games);
}

// Test: Successful deletion of the last game
void test_logic_delete_game_success_last(void) {
    int game_count = 3;
    Game* games = create_test_games(game_count);
    TEST_ASSERT_NOT_NULL(games);

    int result = logic_delete_game(games, &game_count, 3);  // Delete ID 3 (index 2)

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_EQUAL_INT(2, game_count);
    TEST_ASSERT_EQUAL_STRING("Title 1", games[0].title);
    TEST_ASSERT_EQUAL_STRING("Title 2", games[1].title);

    free(games);
}

// Test: Game not found
void test_logic_delete_game_not_found(void) {
    int game_count = 3;
    Game* games = create_test_games(game_count);
    TEST_ASSERT_NOT_NULL(games);

    int original_count = game_count;
    int result = logic_delete_game(games, &game_count, 4);  // ID 4 doesn't exist

    TEST_ASSERT_EQUAL_INT(ERR_GAME_NOT_FOUND, result);
    TEST_ASSERT_EQUAL_INT(original_count, game_count);  // Count unchanged
    // Verify array unchanged
    TEST_ASSERT_EQUAL_STRING("Title 1", games[0].title);
    TEST_ASSERT_EQUAL_STRING("Title 2", games[1].title);
    TEST_ASSERT_EQUAL_STRING("Title 3", games[2].title);

    free(games);
}

// Test: Empty array
void test_logic_delete_game_empty_array(void) {
    Game* games = NULL;
    int game_count = 0;

    int result = logic_delete_game(games, &game_count, 1);

    TEST_ASSERT_EQUAL_INT(ERR_GAME_NOT_FOUND, result);
    TEST_ASSERT_EQUAL_INT(0, game_count);
    // No crash, games still NULL
}

// Test: Invalid ID (negative)
void test_logic_delete_game_invalid_id(void) {
    int game_count = 3;
    Game* games = create_test_games(game_count);
    TEST_ASSERT_NOT_NULL(games);

    int original_count = game_count;
    int result = logic_delete_game(games, &game_count, -1);

    TEST_ASSERT_EQUAL_INT(ERR_GAME_NOT_FOUND, result);
    TEST_ASSERT_EQUAL_INT(original_count, game_count);

    free(games);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_delete_game_success_middle);
    RUN_TEST(test_logic_delete_game_success_first);
    RUN_TEST(test_logic_delete_game_success_last);
    RUN_TEST(test_logic_delete_game_not_found);
    RUN_TEST(test_logic_delete_game_empty_array);
    RUN_TEST(test_logic_delete_game_invalid_id);
    return UNITY_END();
}