#define _XOPEN_SOURCE // Define für die strptime Funktion, um Datumsformate zu prüfen
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/logic/logic.h"
#include "../inc/data/data.h"
#include "process_games.h"
#include "../src/data/load_games.h"
#include <time.h> // Für validate_player_profile() Funktion um das Startdatum zu prüfen
#include "../inc/error.h" // Für Fehlercodes

// ===================== SCHICHTEN-KOMMENTARE BEGINN =====================
//
// Logikschicht: Validiert, verarbeitet, prüft Formate, berechnet, entscheidet.
// Datenschicht: Liest/schreibt Dateien, persistiert Daten.
// Präsentationsschicht: Präsentiert Menüs, liest Benutzereingaben, gibt Ausgaben aus.
//
// HINWEIS: Wo Code gemischte Verantwortlichkeiten hat, ist dies explizit markiert.
// ===================== SCHICHTEN-KOMMENTARE ENDE =====================

cJSON *logic_create_report(const char *title, const char *description, const char *date) {
    // Logikschicht: Erstellt ein Report-Objekt, keine I/O, keine Validierung
    if (!title || !description || !date) {
        return NULL;
    }

    cJSON *report = cJSON_CreateObject();
    if (!report) {
        return NULL;
    }

    cJSON_AddStringToObject(report, "title", title);
    cJSON_AddStringToObject(report, "description", description);
    cJSON_AddStringToObject(report, "date", date);

    return report;
}

int validate_player_profile(const char* full_name, const char* gamertag, const char* ssn, const char* email, const char* sub_start, const char* sub_end, const char* is_subscribed_str){
    // Logikschicht: Validierung der Eingaben
    // Pflichtfelder: leer oder nur Leerzeichen -> Fehler
    if (strlen(full_name) == 0 || logic_is_only_spaces(full_name) ||
        strlen(gamertag) == 0 || logic_is_only_spaces(gamertag) ||
        strlen(ssn) == 0 || logic_is_only_spaces(ssn) ||
        strlen(email) == 0 || logic_is_only_spaces(email) ||
        strlen(sub_start) == 0 || logic_is_only_spaces(sub_start) ||
        strlen(sub_end) == 0 || logic_is_only_spaces(sub_end)) {
        return ERR_EMPTY_FIELD;
    }

    // added after testing
    // Validate SSN: Must be XXXX-XXXXXX (10 digits, 1 "-") - Added after testing 07.04.2025
    if(strlen(ssn) != 11 || ssn[4] != '-'){
        return ERR_INVALID_SSN; // invalid SSN format
    }
    for(int i = 0; i < 11; i++) {
        if(i != 4 && (ssn[i] < '0' || ssn[i] > '9')){
            return ERR_INVALID_SSN; // invalid SSN format (non-digit character)
        }
        // sina
    }

    // Validate email: Must have '@' and at least one '.' after it - Added after testing 07.04.2025
    const char* at_pos = strchr(email, '@');
        if (!at_pos || strchr(at_pos, '.') == NULL) {
            return ERR_INVALID_EMAIL; // invalid email format
            // sina
        }

    // Datumsformate prüfen
    struct tm start_tm = {0}, end_tm = {0};
    if (!strptime(sub_start, "%d.%m.%Y", &start_tm) || !strptime(sub_end, "%d.%m.%Y", &end_tm)) {
        return ERR_INVALID_DATE;
        // sina
    }
    // Set tm_isdst to -1 to let mktime determine DST
    start_tm.tm_isdst = -1;
    end_tm.tm_isdst = -1;

    // Startdatum darf nicht in der Vergangenheit liegen
    time_t now = time(NULL);
    struct tm now_tm = *localtime(&now);
    now_tm.tm_hour = 0; now_tm.tm_min = 0; now_tm.tm_sec = 0;
    now_tm.tm_isdst = -1;
    time_t today = mktime(&now_tm);
    time_t start_time = mktime(&start_tm);
    if (difftime(start_time, today) < 0) {
        return ERR_PAST_DATE; // Startdatum in der Vergangenheit
        // david
    }

    // is_subscribed prüfen
    int is_subscribed;
    if (strcmp(is_subscribed_str, "true") == 0) {
        is_subscribed = 1;
    } else if (strcmp(is_subscribed_str, "false") == 0) {
        is_subscribed = 0;
    } else {
        return ERR_INVALID_SUB_STATUS;
        // Donato
    }


    // Check if gamertag is unique - for future implementation
    // This is a placeholder for the actual implementation
    

    // By default, new player profiles have 0 player hours
    int player_hours = 0;

    // Versuche zu speichern
    int result = save_player_profile(full_name, gamertag, player_hours, ssn, email, sub_start, sub_end, is_subscribed);

    if (result == 0) {
        return ERR_SUCCESS; // Erfolg
    } else {
        return ERR_STORAGE_FAILURE; // Speicherfehler
        // Donato
    }
}

void print_users_logic(){
    // Logikschicht: Ruft Datenschicht auf
    // HINWEIS: printf in dieser Funktion ist Präsentationslogik und sollte ausgelagert werden!
    print_user_to_cli();
}

int display_users_logic(){
    // Logikschicht: Ruft Datenschicht auf, gibt aber auch direkt aus (printf)
    // HINWEIS: printf ist Präsentationslogik und sollte ausgelagert werden!
    char* users_data = NULL;
    int result = read_player_profiles(&users_data);
    if(result == ERR_SUCCESS){
        printf("%s\n", users_data); // Print the formatted user data
        free(users_data);           // Free the allocated string
    }
    return result;
}

int remove_user_logic(const char* gamertag) {
    // Logikschicht: Ruft Datenschicht auf
    return remove_player_profile(gamertag);
}

int edit_user_logic(const char* gamertag, const char* new_full_name, const char* new_ssn, const char* new_email, const char* sub_start, const char* sub_end, const char* is_subscribed_str) {
    // Logikschicht: Validierung und Aufruf Datenschicht
    // Wenn alle Eingabefelder leer sind → nichts zu bearbeiten
    // Eingefügt, weil beim Editieren von Usern Daten die Felder mit "0" als "Error Fields cannot be empty" markiert wurden
    if (strlen(new_full_name) == 0 &&
        strlen(new_ssn) == 0 &&
        strlen(new_email) == 0 &&
        strlen(sub_start) == 0 &&
        strlen(sub_end) == 0) {
        return ERR_EMPTY_FIELD;
    }

    // SSN prüfen (nur wenn vorhanden)
    if (strlen(new_ssn) > 0) {
        if (strlen(new_ssn) != 11 || new_ssn[4] != '-') {
            return ERR_INVALID_SSN;
        }
        for (int i = 0; i < 11; i++) {
            if (i != 4 && (new_ssn[i] < '0' || new_ssn[i] > '9')) {
                return ERR_INVALID_SSN;
            }
        }
    }

    // E-Mail prüfen
    if (strlen(new_email) != 0) {
        const char* at_pos = strchr(new_email, '@');
        if (!at_pos || strchr(at_pos, '.') == NULL) {
            return ERR_INVALID_EMAIL;
        }
    }


    // Datum prüfen (nur wenn beide gesetzt sind)
    if (strlen(sub_start) > 0 && strlen(sub_end) > 0) {
        struct tm start_tm = {0}, end_tm = {0};
        if (!strptime(sub_start, "%d.%m.%Y", &start_tm) || !strptime(sub_end, "%d.%m.%Y", &end_tm)) {
            return ERR_INVALID_DATE;
        }
        // Set tm_isdst to -1 to let mktime determine DST
        start_tm.tm_isdst = -1;
        end_tm.tm_isdst = -1;

        time_t now = time(NULL);
        struct tm now_tm = *localtime(&now);
        now_tm.tm_hour = 0; now_tm.tm_min = 0; now_tm.tm_sec = 0;
        now_tm.tm_isdst = -1;
        time_t today = mktime(&now_tm);
        time_t start_time = mktime(&start_tm);

        if (difftime(start_time, today) < 0) {
            return ERR_PAST_DATE;
        }
    }

    // Abo-Status prüfen (nur wenn übergeben)
    int is_subscribed = -1;  // -1 = ignorieren
    if (is_subscribed_str != NULL) {
        if (strcmp(is_subscribed_str, "true") == 0) {
            is_subscribed = 1;
        } else if (strcmp(is_subscribed_str, "false") == 0) {
            is_subscribed = 0;
        } else {
            return ERR_INVALID_SUB_STATUS;
        }
    }

    int result = edit_player_profile(gamertag, new_full_name, new_ssn, new_email,
                                     sub_start, sub_end, is_subscribed);

    if (result == ERR_SUCCESS) {
        return ERR_SUCCESS;
    } else if (result == ERR_USER_NOT_FOUND) {
        return ERR_USER_NOT_FOUND; // Gamertag nicht gefunden
    } else {
        return ERR_STORAGE_FAILURE; // Unbekannter Fehler
    }
}

//=======================================================================
// CODE VON DEV BRANCH EINGEFÜGT:
// Funktion, um Spieldaten zu laden (mit klarem Fehler-Log)
// in logic.c
// 20.06.2025: Funktion initialize_game_data_loading() wurde angepasst, den Fehler zu beheben, dass bei Programmstart Spiele falsch geladen werden.
int initialize_game_data_loading(const char *filename, Game **games, int *game_count) {
    // Logikschicht: Initialisiert Spieldaten, ruft Datenschicht auf
    // HINWEIS: printf ist Präsentationslogik und sollte ausgelagert werden!
    printf("Loading games from: %s\n", filename);

    // Die Funktion wird nun mit der Adresse von game_count aufgerufen.
    *games = load_games(filename, game_count); 

    if (*games == NULL) {
        printf("Error: Could not load games from %s\n", filename);
        // game_count wird bei einem Fehler bereits von load_games auf 0 gesetzt.
        return ERR_STORAGE_FAILURE;
    }

    printf("Loaded %d games successfully.\n", *game_count);
    return ERR_SUCCESS;
}

// Funktion zum Bearbeiten eines Spiels
int edit_game(Game games[], int game_count, int game_id, const char *new_title) {
    // Logikschicht: Bearbeitet Spieldaten im Speicher
    for (int i = 0; i < game_count; i++) {
        if (games[i].id == game_id) {
            strncpy(games[i].title, new_title, sizeof(games[i].title) - 1);
            games[i].title[sizeof(games[i].title) - 1] = '\0';
            printf("Game with ID %d has been updated.\n", game_id);
            return ERR_SUCCESS;
        }
    }
    printf("Error: Game with ID %d not found.\n", game_id);
    return ERR_USER_NOT_FOUND;
}

// Funktion zum Löschen eines Spiels
int delete_game(Game games[], int *game_count, int game_id) {
    // Logikschicht: Löscht Spieldaten im Speicher
    for (int i = 0; i < *game_count; i++) {
        if (games[i].id == game_id) {
            for (int j = i; j < *game_count - 1; j++) {
                games[j] = games[j + 1];
            }
            (*game_count)--;
            printf("Game with ID %d has been deleted.\n", game_id);
            return ERR_SUCCESS;
        }
    }
    printf("Error: Game with ID %d not found.\n", game_id);
    return ERR_GAME_NOT_FOUND;
}

// Funktion zum Hinzufügen eines neuen Spiels
int add_new_game(Game **games, int *game_count, const char *title, const char *description, const char *version, const char *mode) {
    // Logikschicht: Fügt neues Spiel hinzu
    *games = realloc(*games, (*game_count + 1) * sizeof(Game));
    if (*games == NULL) {
        printf("Error: Memory allocation failed.\n");
        return ERR_STORAGE_FAILURE;
    }

    (*games)[*game_count].id = (*game_count) + 1;
    strncpy((*games)[*game_count].title, title, sizeof((*games)[*game_count].title) - 1);
    (*games)[*game_count].title[sizeof((*games)[*game_count].title) - 1] = '\0';

    strncpy((*games)[*game_count].description, description, sizeof((*games)[*game_count].description) - 1);
    (*games)[*game_count].description[sizeof((*games)[*game_count].description) - 1] = '\0';

    strncpy((*games)[*game_count].version, version, sizeof((*games)[*game_count].version) - 1);
    (*games)[*game_count].version[sizeof((*games)[*game_count].version) - 1] = '\0';

    strncpy((*games)[*game_count].mode, mode, sizeof((*games)[*game_count].mode) - 1);
    (*games)[*game_count].mode[sizeof((*games)[*game_count].mode) - 1] = '\0';

    (*games)[*game_count].current_streams = 0;
    (*game_count)++;
    
    printf("New game added: %s\n", (*games)[*game_count - 1].title);
    return ERR_SUCCESS;
}

// Funktion, um alle Spiele anzuzeigen
void display_all_games(Game *games, int game_count) {
    // Logikschicht: Gibt Spieldaten aus
    // HINWEIS: printf ist Präsentationslogik und sollte ausgelagert werden!
    printf("\n=== Displaying All Games ===\n");
    for (int i = 0; i < game_count; i++) {
        printf("ID: %d | Title: %s | Version: %s | Mode: %s\n", 
            games[i].id, 
            games[i].title, 
            games[i].version, 
            games[i].mode);
    }
    printf("============================\n");
}

// Refactored 04.07.2025: Hilfsfunktion für Präsentationsschicht
int logic_is_only_spaces(const char *str) {
    for (size_t i = 0; i < strlen(str); ++i) {
        if (str[i] != ' ') return 0;
    }
    return 1;
}

// Hinweis (04.07.2025): Die Funktion logic_update_all_subscription_flags() wurde entfernt, da der Logic-Layer für diese Operation nicht benötigt wird.