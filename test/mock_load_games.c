#include "load_games.h"
#include <stdlib.h>
#include <string.h>

Game dummy_game_list[1];

Game* load_games(const char *filename, int *count_out) {
    (void)filename;

    *count_out = 1;

    // Dummy-Spiel setzen
    strncpy(dummy_game_list[0].title, "Mock Game", sizeof(dummy_game_list[0].title) - 1);
    strncpy(dummy_game_list[0].description, "Test description", sizeof(dummy_game_list[0].description) - 1);
    strncpy(dummy_game_list[0].version, "1.0", sizeof(dummy_game_list[0].version) - 1);
    strncpy(dummy_game_list[0].mode, "Singleplayer", sizeof(dummy_game_list[0].mode) - 1);
    dummy_game_list[0].id = 1;
    dummy_game_list[0].current_streams = 0;

    return dummy_game_list;
}
