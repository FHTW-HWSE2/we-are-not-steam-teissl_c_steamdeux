#include "unity.h"
#include "../../inc/logic/logic.h"
#include "../../src/data/game.h"
#include "../mocks/mock_data.h"
#include "../mocks/mock_presentation.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// --- Mocks für Präsentationsfunktionen und Input ---
// Input-Mocks jetzt global aus mock_presentation.h

// --- Hilfsfunktionen ---
Game* create_test_games(int count) {
    Game* games = malloc(sizeof(Game) * count);
    for (int i = 0; i < count; ++i) {
        games[i].id = i + 1;
        snprintf(games[i].title, sizeof(games[i].title), "Titel %d", i + 1);
        snprintf(games[i].description, sizeof(games[i].description), "Desc %d", i + 1);
        snprintf(games[i].version, sizeof(games[i].version), "1.0");
        snprintf(games[i].mode, sizeof(games[i].mode), "Mode");
        games[i].current_streams = 0;
    }
    return games;
}

void setUp(void) {
    mock_show_message_called = 0;
    mock_display_error_called = 0;
    last_error_msg[0] = '\0';
}
void tearDown(void) {}

// --- Tests für logic_handle_edit_game ---
void test_handle_edit_game_success(void) {
    int game_count = 2;
    Game* games = create_test_games(game_count);
    strcpy(mock_input_id, "2");
    strcpy(mock_new_title, "Bearbeitet");

    logic_handle_edit_game(games, game_count);

    TEST_ASSERT_EQUAL_STRING("Bearbeitet", games[1].title);
    TEST_ASSERT_EQUAL(1, mock_show_message_called);
    TEST_ASSERT_EQUAL(0, mock_display_error_called);
    free(games);
}

void test_handle_edit_game_not_found(void) {
    int game_count = 2;
    Game* games = create_test_games(game_count);
    strcpy(mock_input_id, "99");
    strcpy(mock_new_title, "Bearbeitet");

    logic_handle_edit_game(games, game_count);

    TEST_ASSERT_EQUAL(0, mock_show_message_called);
    TEST_ASSERT_EQUAL(1, mock_display_error_called);
    TEST_ASSERT_EQUAL_STRING("Game not found.", last_error_msg);
    free(games);
}

void test_handle_edit_game_empty_title(void) {
    int game_count = 2;
    Game* games = create_test_games(game_count);
    strcpy(mock_input_id, "1");
    strcpy(mock_new_title, "   ");

    logic_handle_edit_game(games, game_count);

    TEST_ASSERT_EQUAL(0, mock_show_message_called);
    TEST_ASSERT_EQUAL(1, mock_display_error_called);
    TEST_ASSERT_EQUAL_STRING("New title must not be empty.", last_error_msg);
    free(games);
}

void test_handle_edit_game_save_failure(void) {
    int game_count = 2;
    Game* games = create_test_games(game_count);
    strcpy(mock_input_id, "1");
    strcpy(mock_new_title, "Bearbeitet");
    // Simuliere Fehler beim Speichern
    extern int mock_data_save_games_return;
    mock_data_save_games_return = ERR_STORAGE_FAILURE;

    logic_handle_edit_game(games, game_count);

    TEST_ASSERT_EQUAL_STRING("Titel 1", games[0].title); // Titel bleibt unverändert
    TEST_ASSERT_EQUAL(0, mock_show_message_called);
    TEST_ASSERT_EQUAL(1, mock_display_error_called);
    TEST_ASSERT_EQUAL_STRING("Failed to save games.", last_error_msg);
    mock_data_save_games_return = ERR_SUCCESS; // Reset für andere Tests
    free(games);
}

// --- Tests für logic_handle_delete_game ---
void test_handle_delete_game_success(void) {
    int game_count = 2;
    Game* games = create_test_games(game_count);
    Game* orig = games;
    strcpy(mock_input_id, "1");
    logic_handle_delete_game(&games, &game_count);
    TEST_ASSERT_EQUAL(1, game_count);
    TEST_ASSERT_EQUAL_STRING("Titel 2", games[0].title);
    TEST_ASSERT_EQUAL(1, mock_show_message_called);
    TEST_ASSERT_EQUAL(0, mock_display_error_called);
    free(games);
    if (games != orig) free(orig); // falls realloc
}

void test_handle_delete_game_not_found(void) {
    int game_count = 2;
    Game* games = create_test_games(game_count);
    strcpy(mock_input_id, "99");
    logic_handle_delete_game(&games, &game_count);
    TEST_ASSERT_EQUAL(2, game_count);
    TEST_ASSERT_EQUAL(0, mock_show_message_called);
    TEST_ASSERT_EQUAL(1, mock_display_error_called);
    TEST_ASSERT_EQUAL_STRING("Game not found.", last_error_msg);
    free(games);
}

void test_handle_delete_game_save_failure(void) {
    int game_count = 2;
    Game* games = create_test_games(game_count);
    strcpy(mock_input_id, "2");
    extern int mock_data_save_games_return;
    mock_data_save_games_return = ERR_STORAGE_FAILURE;

    logic_handle_delete_game(&games, &game_count);

    TEST_ASSERT_EQUAL(2, game_count); // Kein Spiel gelöscht
    TEST_ASSERT_EQUAL_STRING("Titel 1", games[0].title);
    TEST_ASSERT_EQUAL_STRING("Titel 2", games[1].title);
    TEST_ASSERT_EQUAL(0, mock_show_message_called);
    TEST_ASSERT_EQUAL(1, mock_display_error_called);
    TEST_ASSERT_EQUAL_STRING("Failed to save games.", last_error_msg);
    mock_data_save_games_return = ERR_SUCCESS; // Reset für andere Tests
    free(games);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_handle_edit_game_success);
    RUN_TEST(test_handle_edit_game_not_found);
    RUN_TEST(test_handle_edit_game_empty_title);
    RUN_TEST(test_handle_edit_game_save_failure);
    RUN_TEST(test_handle_delete_game_success);
    RUN_TEST(test_handle_delete_game_not_found);
    RUN_TEST(test_handle_delete_game_save_failure);
    return UNITY_END();
}
