#include "unity.h"
#include "../../inc/logic/logic.h"  // For logic_add_new_game
#include "../../src/data/game.h"   // For Game struct
#include "../error.h"              // For ERR_SUCCESS, ERR_STORAGE_FAILURE


void setUp(void) {
    // No global setup needed
}

void tearDown(void) {
    // No global teardown needed
}

// Test: Add first game to empty list (games == NULL, *game_count == 0)
void test_logic_add_new_game_add_to_empty_success(void) {
    Game *games = NULL;
    int game_count = 0;

    const char *title = "Test Title";
    const char *description = "Test Description";
    const char *version = "1.0";
    const char *mode = "Single Player";

    int result = logic_add_new_game(&games, &game_count, title, description, version, mode);

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_EQUAL_INT(1, game_count);
    TEST_ASSERT_NOT_NULL(games);
    TEST_ASSERT_EQUAL_INT(1, games[0].id);
    TEST_ASSERT_EQUAL_STRING("Test Title", games[0].title);
    TEST_ASSERT_EQUAL_STRING("Test Description", games[0].description);
    TEST_ASSERT_EQUAL_STRING("1.0", games[0].version);
    TEST_ASSERT_EQUAL_STRING("Single Player", games[0].mode);
    TEST_ASSERT_EQUAL_INT(0, games[0].current_streams);

    free(games);
}

// Test: Add second game to existing list
void test_logic_add_new_game_add_second_success(void) {
    Game *games = NULL;
    int game_count = 0;

    // Add first
    logic_add_new_game(&games, &game_count, "First Title", "First Desc", "1.0", "Mode1");

    // Add second
    int result = logic_add_new_game(&games, &game_count, "Second Title", "Second Desc", "2.0", "Mode2");

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_EQUAL_INT(2, game_count);
    TEST_ASSERT_NOT_NULL(games);

    // Check first unchanged
    TEST_ASSERT_EQUAL_INT(1, games[0].id);
    TEST_ASSERT_EQUAL_STRING("First Title", games[0].title);

    // Check second
    TEST_ASSERT_EQUAL_INT(2, games[1].id);
    TEST_ASSERT_EQUAL_STRING("Second Title", games[1].title);
    TEST_ASSERT_EQUAL_STRING("Second Desc", games[1].description);
    TEST_ASSERT_EQUAL_STRING("2.0", games[1].version);
    TEST_ASSERT_EQUAL_STRING("Mode2", games[1].mode);
    TEST_ASSERT_EQUAL_INT(0, games[1].current_streams);

    free(games);
}

// Test: String truncation for long inputs (assuming typical sizes, e.g., title[100])
void test_logic_add_new_game_long_strings_truncated(void) {
    Game *games = NULL;
    int game_count = 0;

    // Generate long strings (assume title max ~99 chars + null)
    char long_title[200];
    memset(long_title, 'A', 199);
    long_title[199] = '\0';

    char long_desc[300];
    memset(long_desc, 'B', 299);
    long_desc[299] = '\0';

    int result = logic_add_new_game(&games, &game_count, long_title, long_desc, "1.0", "Mode");

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_EQUAL_INT(1, game_count);

    // Check truncation: strlen should be sizeof(title)-1
    // Note: Actual sizes from game.h; assuming title[100], desc[256], etc.
    // For test, check not full length
    TEST_ASSERT_LESS_THAN(200, strlen(games[0].title));
    TEST_ASSERT_LESS_THAN(300, strlen(games[0].description));
    TEST_ASSERT_EQUAL_STRING_LEN(long_title, games[0].title, sizeof(games[0].title) - 1);
    TEST_ASSERT_EQUAL_STRING_LEN(long_desc, games[0].description, sizeof(games[0].description) - 1);
    TEST_ASSERT_EQUAL_CHAR('\0', games[0].title[sizeof(games[0].title) - 1]);
    TEST_ASSERT_EQUAL_CHAR('\0', games[0].description[sizeof(games[0].description) - 1]);

    free(games);
}

// Note: Testing realloc failure requires mocking realloc, which is not done here.
// You can add if using a mock library for malloc/realloc.

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_add_new_game_add_to_empty_success);
    RUN_TEST(test_logic_add_new_game_add_second_success);
    RUN_TEST(test_logic_add_new_game_long_strings_truncated);
    return UNITY_END();
}