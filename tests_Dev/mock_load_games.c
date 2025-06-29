
#include <stdio.h>
#include "../data/load_games.h"
#include "../game.h"

int load_games(const char *filename, Game games[], int *game_count) {
    snprintf(games[0].title, sizeof(games[0].title), "Mock Game");
    snprintf(games[0].description, sizeof(games[0].description), "A mocked game description");
    snprintf(games[0].version, sizeof(games[0].version), "1.0");
    snprintf(games[0].mode, sizeof(games[0].mode), "Solo");
    games[0].current_streams = 42;
    *game_count = 1;
    return 0;
}
