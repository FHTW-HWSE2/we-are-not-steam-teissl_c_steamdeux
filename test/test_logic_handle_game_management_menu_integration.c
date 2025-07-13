#include "unity.h"
#include "../../inc/logic/logic.h"
#include "../../src/data/game.h"
#include <stdlib.h>
#include <string.h>

// Kein Mocking von logic_display_games_formatted etc.

void setUp(void) {
    // Setup if needed
}

void tearDown(void) {
    // Cleanup if needed
}

#include <stdio.h>
#include <string.h>

void test_logic_handle_game_management_menu_all_cases(void) {
    // Erstelle Test-Games-Datei mit einem Spiel für edit/delete
    FILE *f = fopen("test/test_games_integration.json", "w");
    fputs("{\"games\": [{\"title\": \"T1\", \"description\": \"Desc\", \"version\": \"1.0\", \"mode\": \"Arcade\", \"current_streams\": 0}]}", f);
    fclose(f);

    // Redirect stdin auf vorbereitete Eingabedatei (alle Fälle)
    FILE *input = freopen("test/test_input_all_cases_game_mgmt.txt", "r", stdin);
    TEST_ASSERT_NOT_NULL_MESSAGE(input, "Konnte test_input_all_cases.txt nicht öffnen.");

    logic_handle_game_management_menu();
}



int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_handle_game_management_menu_all_cases);
    return UNITY_END();
}
