#ifndef PROCESS_GAMES_H
#define PROCESS_GAMES_H

#include "../src/data/game.h"

// Funktion zum Laden der Spiele
int initialize_game_data_loading(const char *filename, Game **games, int *game_count);

// Funktion zum Bearbeiten eines Spiels
int edit_game(Game games[], int game_count, int game_id, const char *new_title);

// Funktion zum Löschen eines Spiels
int delete_game(Game games[], int *game_count, int game_id);

// Funktion zum Hinzufügen eines neuen Spiels
int add_new_game(Game **games, int *game_count, const char *title, const char *description, const char *version, const char *mode);

// Funktion zum Anzeigen aller Spiele
void display_all_games(Game *games, int game_count);

#endif
