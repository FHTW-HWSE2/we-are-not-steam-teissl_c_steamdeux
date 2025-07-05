#define _XOPEN_SOURCE // Define für die strptime Funktion, um Datumsformate zu prüfen
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h> // Für validate_player_profile() Funktion um das Startdatum zu prüfen
#include "../inc/logic/logic.h"
#include "../inc/data/data.h"
#include "../inc/error.h" // Für Fehlercodes
#include "cJSON.h"

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

// --- Static validation helpers (internal use only) ---
static int logic_validate_required_field(const char* str) {
    return str && strlen(str) > 0 && !logic_is_only_spaces(str);
}

static int is_valid_ssn_format(const char* ssn) {
    if (!ssn || strlen(ssn) != 11) return 0;
    if (ssn[4] != '-' && ssn[4] != ' ') return 0;
    for (int i = 0; i < 11; i++) {
        if (i == 4) continue;
        if (ssn[i] < '0' || ssn[i] > '9') return 0;
    }
    return 1;
}

static int is_valid_email_format(const char* email) {
    if (!email || strlen(email) == 0) return 0;
    const char* at_pos = strchr(email, '@');
    if (!at_pos || strchr(at_pos, '.') == NULL) return 0;
    return 1;
}

static int is_valid_date_format(const char* date) {
    if (!date || strlen(date) != 10) return 0;
    if (date[2] != '.' || date[5] != '.') return 0;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (date[i] < '0' || date[i] > '9') return 0;
    }
    return 1;
}

static int is_valid_alpha_format(const char *str) {
    if (!str || strlen(str) == 0) return 0;
    for (size_t i = 0; i < strlen(str); ++i) {
        if ((str[i] < 'A' || (str[i] > 'Z' && str[i] < 'a') || str[i] > 'z') && str[i] != ' ' && str[i] != '-') {
            return 0;
        }
    }
    return 1;
}

static int logic_validate_subscription_status(const char* status, int* out) {
    if (strcmp(status, "true") == 0) { *out = 1; return 1; }
    if (strcmp(status, "false") == 0) { *out = 0; return 1; }
    return 0;
}

static int is_date_in_future(const char* date_str) {
    if (!date_str) return 0;
    struct tm date_tm = {0};
    if (!strptime(date_str, "%d.%m.%Y", &date_tm)) return 0;
    date_tm.tm_isdst = -1;
    
    time_t now = time(NULL);
    struct tm now_tm = *localtime(&now);
    now_tm.tm_hour = 0; now_tm.tm_min = 0; now_tm.tm_sec = 0;
    now_tm.tm_isdst = -1;
    
    time_t today = mktime(&now_tm);
    time_t date_time = mktime(&date_tm);
    
    return difftime(date_time, today) >= 0;
}

// --- Split: validation only ---
int logic_validate_player_profile(const char* full_name, const char* gamertag, const char* ssn, const char* email, const char* sub_start, const char* sub_end, const char* is_subscribed_str) {
    if (!logic_validate_required_field(full_name) ||
        !logic_validate_required_field(gamertag) ||
        !logic_validate_required_field(ssn) ||
        !logic_validate_required_field(email) ||
        !logic_validate_required_field(sub_start) ||
        !logic_validate_required_field(sub_end)) {
        return ERR_EMPTY_FIELD;
    }
    if (!is_valid_ssn_format(ssn)) return ERR_INVALID_SSN;
    if (!is_valid_email_format(email)) return ERR_INVALID_EMAIL;
    if (!is_valid_date_format(sub_start) || !is_valid_date_format(sub_end)) return ERR_INVALID_DATE;
    if (!is_date_in_future(sub_start)) return ERR_PAST_DATE;
    int is_subscribed;
    if (!logic_validate_subscription_status(is_subscribed_str, &is_subscribed)) return ERR_INVALID_SUB_STATUS;
    return ERR_SUCCESS;
}

// --- Split: creation (calls validation, then data layer) ---
int logic_create_user(const char* full_name, const char* gamertag, const char* ssn, const char* email, const char* sub_start, const char* sub_end, const char* is_subscribed_str) {
    int valid = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);
    if (valid != ERR_SUCCESS) return valid;
    int is_subscribed;
    logic_validate_subscription_status(is_subscribed_str, &is_subscribed);
    int player_hours = 0;
    int result = data_save_player_profile(full_name, gamertag, player_hours, ssn, email, sub_start, sub_end, is_subscribed);
    if (result == 0) return ERR_SUCCESS;
    else return ERR_STORAGE_FAILURE;
}

// --- Refactor edit_user_logic to use helpers ---
int logic_edit_user(const char* gamertag, const char* new_full_name, const char* new_ssn, const char* new_email, const char* sub_start, const char* sub_end, const char* is_subscribed_str) {
    // Validate non-empty fields individually with proper validation
    if (logic_validate_required_field(new_full_name) && !is_valid_alpha_format(new_full_name)) {
        return ERR_EMPTY_FIELD;  // Using existing error code for invalid format
    }
    if (logic_validate_required_field(new_ssn) && !is_valid_ssn_format(new_ssn)) {
        return ERR_INVALID_SSN;
    }
    if (logic_validate_required_field(new_email) && !is_valid_email_format(new_email)) {
        return ERR_INVALID_EMAIL;
    }
    if (logic_validate_required_field(sub_start) && !is_valid_date_format(sub_start)) {
        return ERR_INVALID_DATE;
    }
    if (logic_validate_required_field(sub_end) && !is_valid_date_format(sub_end)) {
        return ERR_INVALID_DATE;
    }
    
    // Date logic: if start date is provided, validate it's not in past
    if (logic_validate_required_field(sub_start) && !is_date_in_future(sub_start)) {
        return ERR_PAST_DATE;
    }
    
    // Validate subscription status if provided
    int is_subscribed = -1;
    if (is_subscribed_str && strlen(is_subscribed_str) > 0) {
        if (!logic_validate_subscription_status(is_subscribed_str, &is_subscribed)) {
            return ERR_INVALID_SUB_STATUS;
        }
    }
    
    // Call data layer with validated inputs
    int result = data_edit_player_profile(gamertag, new_full_name, new_ssn, new_email, sub_start, sub_end, is_subscribed);
    if (result == ERR_SUCCESS) return ERR_SUCCESS;
    else if (result == ERR_USER_NOT_FOUND) return ERR_USER_NOT_FOUND;
    else return ERR_STORAGE_FAILURE;
}

// --- LOGIC GAME FUNKTIONEN (stumm, keine printf, nur ERR_... Rückgabe) ---

int logic_initialize_game_data_loading(const char *filename, Game **games, int *game_count) {
    return data_load_games(filename, games, game_count);
}

int logic_edit_game(Game games[], int game_count, int game_id, const char *new_title) {
    for (int i = 0; i < game_count; i++) {
        if (games[i].id == game_id) {
            strncpy(games[i].title, new_title, sizeof(games[i].title) - 1);
            games[i].title[sizeof(games[i].title) - 1] = '\0';
            return ERR_SUCCESS;
        }
    }
    return ERR_USER_NOT_FOUND;
}

int logic_delete_game(Game games[], int *game_count, int game_id) {
    for (int i = 0; i < *game_count; i++) {
        if (games[i].id == game_id) {
            for (int j = i; j < *game_count - 1; j++) {
                games[j] = games[j + 1];
            }
            (*game_count)--;
            return ERR_SUCCESS;
        }
    }
    return ERR_GAME_NOT_FOUND;
}

int logic_add_new_game(Game **games, int *game_count, const char *title, const char *description, const char *version, const char *mode) {
    Game *tmp = realloc(*games, (*game_count + 1) * sizeof(Game));
    if (!tmp) {
        return ERR_STORAGE_FAILURE;
    }
    *games = tmp;
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
    return ERR_SUCCESS;
}

// Refactored 04.07.2025: Hilfsfunktion für Präsentationsschicht
int logic_is_only_spaces(const char *str) {
    for (size_t i = 0; i < strlen(str); ++i) {
        if (str[i] != ' ') return 0;
    }
    return 1;
}

// Öffentliche Validierungsfunktionen für Präsentationsschicht - delegieren an interne Helfer
int logic_is_valid_date_format(const char *date) {
    return is_valid_date_format(date);
}

int logic_is_valid_alpha(const char *str) {
    return is_valid_alpha_format(str);
}

int logic_is_valid_email(const char *str) {
    return is_valid_email_format(str);
}

int logic_is_valid_ssn(const char *str) {
    return is_valid_ssn_format(str);
}

// Hinweis (04.07.2025): Die Funktion logic_update_all_subscription_flags() wurde entfernt, da der Logic-Layer für diese Operation nicht benötigt wird.

// Prototyp für die Sortierfunktion (wird in logic_get_top_users verwendet)
static int compare_users_by_hours(const void *a, const void *b);

// Sortierfunktion für logic_get_top_users
static int compare_users_by_hours(const void *a, const void *b) {
    const cJSON *userA = *(const cJSON **)a;
    const cJSON *userB = *(const cJSON **)b;
    int hoursA = cJSON_GetObjectItem(userA, "player_hours")->valueint;
    int hoursB = cJSON_GetObjectItem(userB, "player_hours")->valueint;
    return hoursB - hoursA;
}

// Liefert ein cJSON-Array der Top-N User nach Spielzeit (player_hours absteigend sortiert)
cJSON *logic_get_top_users(int n) {
    cJSON *user_array = NULL;
    if (data_get_all_users(&user_array) != ERR_SUCCESS || !user_array || !cJSON_IsArray(user_array)) {
        if (user_array) cJSON_Delete(user_array);
        return cJSON_CreateArray();
    }
    int user_count = cJSON_GetArraySize(user_array);
    if (user_count == 0) {
        cJSON_Delete(user_array);
        return cJSON_CreateArray();
    }
    cJSON **user_ptrs = malloc(user_count * sizeof(cJSON*));
    if (!user_ptrs) {
        cJSON_Delete(user_array);
        return cJSON_CreateArray();
    }
    for (int i = 0; i < user_count; ++i) {
        user_ptrs[i] = cJSON_GetArrayItem(user_array, i);
    }
    // Sortieren nach player_hours absteigend
    qsort(user_ptrs, user_count, sizeof(cJSON*), compare_users_by_hours);
    int top = user_count < n ? user_count : n;
    cJSON *result = cJSON_CreateArray();
    for (int i = 0; i < top; ++i) {
        cJSON_AddItemToArray(result, cJSON_Duplicate(user_ptrs[i], 1));
    }
    free(user_ptrs);
    cJSON_Delete(user_array);
    return result;
}

int logic_remove_user(const char* gamertag) {
    return data_remove_player_profile(gamertag);
}

int logic_get_all_users(cJSON **users_out) {
    return data_get_all_users(users_out);
}