#ifndef LOAD_GAMES_H
#define LOAD_GAMES_H

#include "game.h"

// 20.06.2025: Funktion load_games() wurde angepasst, den Fehler zu beheben, dass bei Programmstart Spiele falsch geladen werden.
Game *load_games(const char *filename, int *count_out);

#endif
