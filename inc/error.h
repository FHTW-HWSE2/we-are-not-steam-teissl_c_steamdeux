#ifndef ERROR_H
#define ERROR_H

// Erfolgreiche Ausführung
#define ERR_SUCCESS             0   // Keine Fehler

// Allgemeine Eingabefehler
#define ERR_EMPTY_FIELD        -1   // Ein oder mehrere Felder sind leer
#define ERR_INVALID_SSN        -2   // Sozialversicherungsnummer ist ungültig
#define ERR_INVALID_EMAIL      -3   // E-Mail-Adresse ist ungültig
#define ERR_INVALID_SUB_STATUS -4   // Abo-Status ist nicht "true" oder "false"
#define ERR_INVALID_DATE       -5   // Datumsformat ist ungültig

// Logik- oder Speicherfehler
#define ERR_STORAGE_FAILURE    -6   // Fehler beim Speichern in Datei/JSON
#define ERR_PAST_DATE          -7   // Startdatum liegt in der Vergangenheit
#define ERR_USER_NOT_FOUND     -8   // Gamertag nicht gefunden (zB bei Bearbeitung oder Löschung)
#define ERR_GAME_NOT_FOUND     -9   // Spiel nicht gefunden (zB bei Bearbeitung oder Löschung)


#endif // ERROR_H
