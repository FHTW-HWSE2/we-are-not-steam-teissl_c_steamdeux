#include <stdio.h>
#include "display_games.h"

void display_games(const Game games[], int game_count) {
    printf("=== Game List ===\n");
    for (int i = 0; i < game_count; i++) {
        printf("Title: %s\n", games[i].title);
        printf("Description: %s\n", games[i].description);
        printf("Version: %s\n", games[i].version);
        printf("Mode: %s\n", games[i].mode);
        printf("Current Streams: %d\n", games[i].current_streams);
        printf("--------------------------\n");
    }
}
