#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../game.h"
#include "../business/process_games.h"
#include "../data/load_games.h"
#include "../data/save_games.h"
#include "../presentation/display_games.h"
#include "../presentation/display_error.h"

// Test für load_games (Mocked JSON-Daten)
void test_load_games() {
    Game *games = load_games("../games.json");
    assert(games != NULL);
    free(games);
    printf("Test load_games: Passed\n");
}

// Test für save_games (JSON speichern)
void test_save_games() {
    Game games[2] = {
        {1, "Test Game 1", "Description 1", "1.0", "Singleplayer", 0},
        {2, "Test Game 2", "Description 2", "1.1", "Multiplayer", 0}
    };
    int result = save_games("../test_save.json", games, 2);
    assert(result == 0);
    printf("Test save_games: Passed\n");
}

// Test für initialize_game_data_loading
void test_initialize_game_data_loading() {
    Game *games = NULL;
    int game_count = 0;
    int result = initialize_game_data_loading("../games.json", &games, &game_count);
    
    assert(result == 0);
    assert(game_count > 0);
    free(games);
    printf("Test initialize_game_data_loading: Passed\n");
}

// Test für edit_game
void test_edit_game() {
    Game games[2] = {
        {1, "Test Game 1", "Description 1", "1.0", "Singleplayer", 0},
        {2, "Test Game 2", "Description 2", "1.1", "Multiplayer", 0}
    };
    int result = edit_game(games, 2, 1, "Updated Game");
    assert(result == 0);
    assert(strcmp(games[0].title, "Updated Game") == 0);

    result = edit_game(games, 2, 99, "Nonexistent Game");
    assert(result == -1);

    printf("Test edit_game: Passed\n");
}

// Test für delete_game
void test_delete_game() {
    Game games[3] = {
        {1, "Game 1", "Desc 1", "1.0", "Single", 0},
        {2, "Game 2", "Desc 2", "1.1", "Multi", 0},
        {3, "Game 3", "Desc 3", "1.2", "Single", 0}
    };
    int game_count = 3;

    int result = delete_game(games, &game_count, 2);
    assert(result == 0);
    assert(game_count == 2);
    assert(games[1].id == 3);

    result = delete_game(games, &game_count, 99);
    assert(result == -1);

    printf("Test delete_game: Passed\n");
}

// Test für add_new_game
void test_add_new_game() {
    Game *games = NULL;
    int game_count = 0;

    int result = add_new_game(&games, &game_count, "New Game", "New Description", "1.0", "Singleplayer");
    assert(result == 0);
    assert(game_count == 1);
    assert(strcmp(games[0].title, "New Game") == 0);

    free(games);
    printf("Test add_new_game: Passed\n");
}

// Test für display_all_games
void test_display_all_games() {
    Game games[2] = {
        {1, "Game 1", "Description 1", "1.0", "Single", 0},
        {2, "Game 2", "Description 2", "1.1", "Multi", 0}
    };
    printf("\n=== Display All Games Test ===\n");
    display_all_games(games, 2);
    printf("Test display_all_games: Passed\n");
}

// Test für display_error
void test_display_error() {
    display_error("Test Error Message");
    printf("Test display_error: Passed\n");
}

// Hauptfunktion für alle Tests
int main() {
    test_load_games();
    test_save_games();
    test_initialize_game_data_loading();
    test_edit_game();
    test_delete_game();
    test_add_new_game();
    test_display_all_games();
    test_display_error();
    printf("\nAlle Tests abgeschlossen.\n");
    return 0;
}
