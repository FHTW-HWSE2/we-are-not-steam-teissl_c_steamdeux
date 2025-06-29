
#ifndef GAME_H
#define GAME_H

typedef struct {
    int id;                     // Spiel-ID (eindeutig)
    char title[100];             // Titel des Spiels
    char description[256];       // Beschreibung des Spiels
    char version[10];            // Version des Spiels
    char mode[20];               // Spielmodus (z.B. Singleplayer, Multiplayer)
    int current_streams;         // Anzahl der aktuellen Streams
} Game;

#endif
