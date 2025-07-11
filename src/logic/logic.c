#define _GNU_SOURCE // Für warning implicit declaration of function ‘strdup’
#define _XOPEN_SOURCE // Define für die strptime Funktion, um Datumsformate zu prüfen
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h> // Für validate_player_profile() Funktion um das Startdatum zu prüfen
#ifdef UNITTEST_MOCK_TIME
#include "../test/mocks/mock_time.h"
#define time mock_time
#define localtime mock_localtime
#define mktime mock_mktime
#define difftime mock_difftime
#endif
#include "../inc/logic/logic.h"

#define BUFFER_SIZE 256

// ===================== SCHICHTEN-KOMMENTARE BEGINN =====================
// Logikschicht: Validiert, verarbeitet, prüft Formate, berechnet, entscheidet.
// Datenschicht: Liest/schreibt Dateien, persistiert Daten.
// Präsentationsschicht: Präsentiert Menüs, liest Benutzereingaben, gibt Ausgaben aus.
// ===================== SCHICHTEN-KOMMENTARE ENDE =====================

// Forward declarations for static functions
static void logic_user_menu_workflow(void);
static void logic_handle_add_user_workflow(void);
static void logic_handle_edit_user_workflow(void);
static void read_input(const char *prompt, char *buffer, size_t size);

cJSON *logic_create_report(const char *title, const char *description, const char *date) {
    // Defensive Null-Prüfung
    if (!title || !description || !date) {
        return NULL;
    }

    cJSON *report = cJSON_CreateObject();
    if (!report) {
        return NULL;
    }

    if (!cJSON_AddStringToObject(report, "title", title) ||
        !cJSON_AddStringToObject(report, "description", description) ||
        !cJSON_AddStringToObject(report, "date", date)) {
        cJSON_Delete(report);
        return NULL;
    }

    return report;
}

// --- Validation helpers (now global for testing/coverage) ---
int is_valid_ssn_format(const char* ssn) {
    // Nur Format XXXX-XXXXXX (Bindestrich) ist erlaubt
    if (!ssn || strlen(ssn) != 11) return 0;
    if (ssn[4] != '-') return 0;
    for (int i = 0; i < 11; i++) {
        if (i == 4) continue;
        if (ssn[i] < '0' || ssn[i] > '9') return 0;
    }
    return 1;
}

int is_valid_email_format(const char* email) {
    if (!email || strlen(email) == 0) return 0;
    const char* at_pos = strchr(email, '@');
    if (!at_pos || strchr(at_pos, '.') == NULL) return 0;
    return 1;
}

int is_valid_date_format(const char* date) {
    if (!date || strlen(date) != 10) return 0;
    if (date[2] != '.' || date[5] != '.') return 0;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (date[i] < '0' || date[i] > '9') return 0;
    }
    return 1;
}

int is_valid_alpha_format(const char *str) {
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

int is_date_in_future(const char* date_str) {
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

// Enhanced logic_create_user that handles date calculation and duration
int logic_create_user_with_duration(const char* full_name, const char* gamertag, const char* ssn, const char* email, const char* sub_start, const char* is_subscribed_str, int duration_months, int use_today_as_start) {
    char actual_start[64] = {0};
    char calculated_end[64] = {0};
    
    // Handle start date logic
    if (use_today_as_start) {
        time_t now = time(NULL);
        struct tm *now_tm = localtime(&now);
        strftime(actual_start, sizeof(actual_start), "%d.%m.%Y", now_tm);
    } else {
        strncpy(actual_start, sub_start, sizeof(actual_start) - 1);
    }
    
    // Calculate end date based on duration
    struct tm start_tm = {0};
    if (!strptime(actual_start, "%d.%m.%Y", &start_tm)) {
        return ERR_INVALID_DATE;
    }
    
    // Add duration months
    start_tm.tm_mon += duration_months;
    // Handle year overflow
    while (start_tm.tm_mon >= 12) {
        start_tm.tm_mon -= 12;
        start_tm.tm_year++;
    }
    
    // Format end date
    strftime(calculated_end, sizeof(calculated_end), "%d.%m.%Y", &start_tm);
    
    // Now validate and create user with calculated dates
    int valid = logic_validate_player_profile(full_name, gamertag, ssn, email, actual_start, calculated_end, is_subscribed_str);
    if (valid != ERR_SUCCESS) return valid;
    
    int is_subscribed;
    logic_validate_subscription_status(is_subscribed_str, &is_subscribed);
    int player_hours = 0;
    int result = data_save_player_profile(full_name, gamertag, player_hours, ssn, email, actual_start, calculated_end, is_subscribed);
    if (result == 0) return ERR_SUCCESS;
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
    if (!str || str[0] == '\0') return 0;
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

// Generate top users file - moved from presentation layer
int logic_generate_top_users_file(void) {
    int top_n = 10;
    cJSON *top_users = logic_get_top_users(top_n);
    if (!top_users || !cJSON_IsArray(top_users)) {
        if (top_users) cJSON_Delete(top_users);
        return ERR_STORAGE_FAILURE;
    }
    char *json_str = cJSON_Print(top_users);
    FILE *out = fopen("usersRanked.json", "w");
    if (out && json_str) {
        fputs(json_str, out);
        fclose(out);
        free(json_str);
        cJSON_Delete(top_users);
        return ERR_SUCCESS;
    } else {
        if (out) fclose(out);
        free(json_str);
        cJSON_Delete(top_users);
        return ERR_STORAGE_FAILURE;
    }
}

void logic_create_and_save_report(void) {
    const char* title = presentation_get_report_title();
    const char* description = presentation_get_report_description();
    const char* date = presentation_get_report_date();

    if (!title || !description || !date || logic_is_only_spaces(title) || logic_is_only_spaces(description) || logic_is_only_spaces(date)) {
        presentation_error_empty_fields();
        return;
    }
    if (!logic_is_valid_date_format(date)) {
        presentation_error_date();
        return;
    }

    cJSON *report = logic_create_report(title, description, date);
    if (!report) {
        presentation_error_unknown();
        return;
    }
    int save_result = data_save_report(report);

    if (save_result != ERR_SUCCESS) {
        presentation_error_storage();
    } else {
        presentation_success_report_saved();
    }
}

// Diese Funktion bündelt die Startup-Tasks
static int logic_perform_startup_tasks(int* removed_count, int* changed_flags) {
    *removed_count = 0;
    *changed_flags = 0;
    if (data_remove_expired_users(removed_count) != ERR_SUCCESS) {
        return ERR_STORAGE_FAILURE;
    }
    *changed_flags = data_update_all_subscription_flags();
    return ERR_SUCCESS;
}

// Die neue Hauptschleife der Anwendung, gesteuert von der Logik
void logic_start_application(void) {
    presentation_print_welcome_banner();

    // Startup-Tasks ausführen und Ergebnis anzeigen
    int removed, changed;
    if (logic_perform_startup_tasks(&removed, &changed) == ERR_SUCCESS) {
        presentation_show_startup_info(removed, changed);
    } else {
        presentation_error_critical_startup();
    }

    while (1) {
        presentation_display_main_menu();
        int choice = presentation_get_main_menu_choice();

        if (choice == 0) {
            presentation_info_exiting();
            break;
        }
        switch (choice) {
            case 1:
                logic_user_menu_workflow(); // User Management Menü
                break;
            case 2:
                presentation_start_game_management_menu(); // Game Management Menü starten
                break;
            case 3:
                start_simulation(); // Simulation starten
                break;
            case 0:
                presentation_info_exiting();
                return;
            default:
                presentation_error_invalid_option();
        }
    }
}

// Diese Funktion steuert das User Management Menü
static void logic_user_menu_workflow(void) {
    while (1) {
        presentation_display_user_menu();
        int choice = presentation_get_user_menu_choice();
        if (choice == 0) {
            presentation_info_returning_to_main_menu();
            break;
        }
        switch (choice) {
            case 1: // Option 1: Alle User als formatierte Liste anzeigen
                logic_display_users_formatted();
                break;
            case 2: // Add a user (war vorher 3)
                logic_handle_add_user_workflow();
                break;
            case 3: // Edit a user (war vorher 4)
                logic_handle_edit_user_workflow();
                break;
            case 4: // Remove a user (war vorher 5)
                presentation_remove_user();
             break;
            case 5:
                presentation_info_add_report_selected();
                logic_create_and_save_report();
                break;
            case 6:
                presentation_info_rank_top_users_selected();
                presentation_show_top_users_terminal();
                break;
            case 7:
                presentation_info_generate_player_report_selected();
                presentation_generate_top_users_file();
                break;
            default:
                presentation_error_invalid_option();
        }
    }
}

// Diese Funktion steuert das Hinzufügen eines Users im Pull-Modell
static void logic_handle_add_user_workflow(void) {
    char full_name[BUFFER_SIZE], gamertag[BUFFER_SIZE], ssn[BUFFER_SIZE], email[BUFFER_SIZE], sub_start[BUFFER_SIZE], duration_str[BUFFER_SIZE];
    int use_today = 0, duration = 1;

    presentation_welcome_add_user();
    // Full name validation loop
    while (1) {
        presentation_get_full_name(full_name, sizeof(full_name));
        if (logic_is_only_spaces(full_name) || strlen(full_name) == 0) {
            presentation_error_full_name_empty();
        } else if (!logic_is_valid_alpha(full_name)) {
            presentation_error_full_name_format();
        } else {
            break;
        }
    }
    // Gamertag validation loop
    while (1) {
        presentation_get_gamertag(gamertag, sizeof(gamertag));
        if (logic_is_only_spaces(gamertag) || strlen(gamertag) == 0) {
            presentation_error_gamertag_empty();
        } else {
            break;
        }
    }
    // SSN validation loop
    while (1) {
        presentation_get_ssn(ssn, sizeof(ssn));
        if (!logic_is_only_spaces(ssn) && logic_is_valid_ssn(ssn)) {
            break;
        } else {
            presentation_error_ssn();
        }
    }
    // Email validation loop
    while (1) {
        presentation_get_email(email, sizeof(email));
        if (!logic_is_only_spaces(email) && logic_is_valid_email(email)) {
            break;
        } else {
            presentation_error_email();
        }
    }
    // Startdatum-Auswahl
    presentation_choose_subscription_start();
    read_input("Enter your choice (1/0): ", sub_start, sizeof(sub_start));
    use_today = atoi(sub_start);
    if (use_today != 1) {
        presentation_get_subscription_start(sub_start, sizeof(sub_start));
    } else {
        strcpy(sub_start, ""); // Logik entscheidet über heutiges Datum
    }
    // Dauer-Auswahl
    presentation_choose_subscription_model();
    presentation_get_subscription_duration(duration_str, sizeof(duration_str));
    int duration_option = atoi(duration_str);
    if (duration_option == 2) duration = 6;
    else if (duration_option == 3) duration = 12;
    else duration = 1;
    const char *is_subscribed_str = "true";
    int result = logic_create_user_with_duration(full_name, gamertag, ssn, email, sub_start, is_subscribed_str, duration, use_today);
    switch (result) {
        case ERR_SUCCESS:
            presentation_success_user_added();
            break;
        case ERR_INVALID_EMAIL:
            presentation_error_email();
            break;
        case ERR_PAST_DATE:
            presentation_error_past_date();
            break;
        case ERR_INVALID_SSN:
            presentation_error_ssn();
            break;
        case ERR_INVALID_SUB_STATUS:
            presentation_error_sub_status();
            break;
        case ERR_INVALID_DATE:
            presentation_error_date();
            break;
        case ERR_EMPTY_FIELD:
            presentation_error_empty_fields();
            break;
        case ERR_STORAGE_FAILURE:
            presentation_error_storage();
            break;
        default:
            presentation_error_unknown();
            break;
    }
}


// Diese Funktion steuert das Bearbeiten eines Users im Pull-Modell
static void logic_handle_edit_user_workflow(void) {
    char gamertag[MAX_USER_INPUT], new_full_name[MAX_USER_INPUT], new_ssn[MAX_USER_INPUT], new_email[MAX_USER_INPUT];
    char sub_start[MAX_USER_INPUT], sub_end[MAX_USER_INPUT], is_subscribed_str[MAX_USER_INPUT];

    // Gamertag (Pflichtfeld) validieren
    while (1) {
        presentation_get_gamertag(gamertag, sizeof(gamertag));
        if (!logic_is_only_spaces(gamertag)) {
            break;
        } else {
            presentation_error_gamertag_empty_edit();
        }
    }

    // Umstrukturierung 10.07.2025: Warum im unteren Kommentar optional?
    // Full name (optional, aber falls eingegeben, validieren)
    presentation_info_enter_full_name_edit();
    while (1) {
        read_input("", new_full_name, sizeof(new_full_name));
        if (strlen(new_full_name) == 0 || (!logic_is_only_spaces(new_full_name) && logic_is_valid_alpha(new_full_name))) {
            break;
        } else {
            presentation_error_full_name_format_edit();
        }
    }

    // Umstrukturierung 10.07.2025: Warum im unteren Kommentar optional?
    // SSN (optional, aber falls eingegeben, validieren)
    presentation_info_enter_ssn_edit();
    while (1) {
        read_input("", new_ssn, sizeof(new_ssn));
        if (strlen(new_ssn) == 0 || (!logic_is_only_spaces(new_ssn) && logic_is_valid_ssn(new_ssn))) {
            break;
        } else {
            presentation_error_ssn_format_edit();
        }
    }

    // Umstrukturierung 10.07.2025: Warum im unteren Kommentar optional?
    // Email (optional, aber falls eingegeben, validieren)
    presentation_info_enter_email_edit();
    while (1) {
        read_input("", new_email, sizeof(new_email));
        if (strlen(new_email) == 0 || (!logic_is_only_spaces(new_email) && logic_is_valid_email(new_email))) {
            break;
        } else {
            presentation_error_email_format_edit();
        }
    }

    // Umstrukturierung 10.07.2025: Warum im unteren Kommentar optional?
    // Startdatum (optional, aber falls eingegeben, validieren)
    presentation_info_enter_start_date_edit();
    while (1) {
        read_input("", sub_start, sizeof(sub_start));
        if (strlen(sub_start) == 0 || logic_is_valid_date_format(sub_start)) {
            break;
        } else {
            presentation_error_date_format_edit();
        }
    }

    // Umstrukturierung 10.07.2025: Enddatum muss ja berechnet werden. Wenn user nicht subscribed ist, darf er nicht angelegt werden.
    // Enddatum (optional, aber falls eingegeben, validieren)
    presentation_info_enter_end_date_edit();
    while (1) {
        read_input("", sub_end, sizeof(sub_end));
        if (strlen(sub_end) == 0 || logic_is_valid_date_format(sub_end)) {
            break;
        } else {
            presentation_error_date_format_edit();
        }
    }

    // Subscription status (optional, aber falls eingegeben, validieren)
    presentation_info_enter_sub_status_edit();
    while (1) {
        read_input("", is_subscribed_str, sizeof(is_subscribed_str));
        if (strlen(is_subscribed_str) == 0 || strcmp(is_subscribed_str, "1") == 0 || strcmp(is_subscribed_str, "0") == 0) {
            break;
        } else {
            presentation_error_sub_status_edit();
        }
    }

    int result = logic_edit_user(gamertag, new_full_name, new_ssn, new_email, sub_start, sub_end, is_subscribed_str);
    switch (result) {
        case ERR_SUCCESS:
            presentation_success_user_edited();
            break;
        case ERR_USER_NOT_FOUND:
            presentation_error_user_not_found();
            break;
        case ERR_INVALID_SSN:
            presentation_error_ssn();
            break;
        case ERR_INVALID_EMAIL:
            presentation_error_email();
            break;
        case ERR_INVALID_DATE:
            presentation_error_date();
            break;
        case ERR_PAST_DATE:
            presentation_error_past_date();
            break;
        case ERR_EMPTY_FIELD:
            presentation_error_empty_fields();
            break;
        case ERR_STORAGE_FAILURE:
            presentation_error_storage();
            break;
        default:
            presentation_error_unknown();
            break;
    }
}

// Korrigierte Version: logic_edit_user ruft nur die Datenschicht-Funktion auf
int logic_edit_user(const char* gamertag, const char* new_full_name, const char* new_ssn, const char* new_email, const char* sub_start, const char* sub_end, const char* is_subscribed_str) {
    // Schritt 1: Validierung der Eingaben
    if (logic_validate_required_field(new_full_name) && !is_valid_alpha_format(new_full_name)) {
        return ERR_EMPTY_FIELD;
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
    int is_subscribed = -1; // -1 = nicht ändern
    if (is_subscribed_str && strlen(is_subscribed_str) > 0) {
        if (strcmp(is_subscribed_str, "1") == 0) {
            is_subscribed = 1;
        } else if (strcmp(is_subscribed_str, "0") == 0) {
            is_subscribed = 0;
        } else {
            return ERR_INVALID_SUB_STATUS;
        }
    }
    // EIN Aufruf an die Datenschicht, die alles erledigt
    return data_edit_player_profile(gamertag, new_full_name, new_ssn, new_email, sub_start, sub_end, is_subscribed);
}

// Funktion zum Einlesen von Benutzereingaben mit Prompt
static void read_input(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
    } else {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

// Zeigt alle User als formatierte Liste an (Workflow für Menü Option 1)
// 10.07. geändert
void logic_display_users_formatted(void) {
    char **lines = NULL;
    int count = 0;

    if (logic_get_user_lines_for_display(&lines, &count) != ERR_SUCCESS) {
        presentation_show_error("Could not load users.");
        return;
    }

    // Übergabe an Präsentation
    presentation_display_users_formatted(lines, count);

    // Speicher aufräumen
    for (int i = 0; i < count; ++i) {
        free(lines[i]);
    }
    free(lines);
}

// Added 10.07.2025: Diese Funktion gibt eine formatierte Liste aller User zurück
// presentation macht nur printf
// logic erstellt aufbereitete/formatierte Strings.
// data bleibt Dateischnittstelle (JSON laden/speichern)
int logic_get_user_lines_for_display(char*** lines_out, int* count_out) {
    cJSON* users = NULL;
    if (data_get_all_users(&users) != ERR_SUCCESS || !users) {
        if (users) cJSON_Delete(users);
        return ERR_STORAGE_FAILURE;
    }

    int count = cJSON_GetArraySize(users);
    char** result = malloc(sizeof(char*) * count * 10); // max 10 Zeilen pro User (worst-case)

    int line_index = 0;
    for (int i = 0; i < count; i++) {
        cJSON* user = cJSON_GetArrayItem(users, i);
        if (!user) continue;

        char line[256];

        snprintf(line, sizeof(line), "User %d:", i + 1);
        result[line_index++] = strdup(line);

        snprintf(line, sizeof(line), "Name: %s", cJSON_GetObjectItem(user, "full_name")->valuestring);
        result[line_index++] = strdup(line);

        snprintf(line, sizeof(line), "Gamertag: %s", cJSON_GetObjectItem(user, "gamertag")->valuestring);
        result[line_index++] = strdup(line);

        snprintf(line, sizeof(line), "Email: %s", cJSON_GetObjectItem(user, "email")->valuestring);
        result[line_index++] = strdup(line);

        snprintf(line, sizeof(line), "SSN: %s", cJSON_GetObjectItem(user, "ssn")->valuestring);
        result[line_index++] = strdup(line);

        snprintf(line, sizeof(line), "Subscribed: %s", cJSON_GetObjectItem(user, "is_subscribed")->valueint ? "true" : "false");
        result[line_index++] = strdup(line);

        snprintf(line, sizeof(line), "Subscription: %s – %s",
            cJSON_GetObjectItem(user, "subscription_start_date")->valuestring,
            cJSON_GetObjectItem(user, "subscription_end_date")->valuestring);
        result[line_index++] = strdup(line);

        snprintf(line, sizeof(line), "Playtime: %d hours",
            cJSON_GetObjectItem(user, "player_hours")->valueint);
        result[line_index++] = strdup(line);

        result[line_index++] = strdup("--------------------------------------");
    }

    cJSON_Delete(users);
    *lines_out = result;
    *count_out = line_index;
    return ERR_SUCCESS;
}

// Added 10.07.2025
// ersetzt void presentation_start_menu() aus presentation.c
void logic_handle_game_management_menu(void) {
    Game *games = NULL;
    int game_count = 0;

    if (logic_initialize_game_data_loading(GAMES_JSON_PATH, &games, &game_count) != ERR_SUCCESS) {
        presentation_display_error("Failed to load games.");
        return;
    }

    int choice;
    do {
        presentation_display_game_management_menu(); // zeigt nur das Menü
        choice = presentation_get_game_menu_choice(); // liest Eingabe
        switch (choice) {
            case 1:
                logic_display_games_formatted(games, game_count); // Präsentationsfunktion // 10.07. ersetzt die alte Funktion presentation_display_games
                break;
            case 2:
                logic_handle_add_game(&games, &game_count);
                break;
            case 3:
                logic_handle_edit_game(games, game_count);
                break;
            case 4:
                logic_handle_delete_game(&games, &game_count);
                break;
            case 0:
                break;
            default:
                presentation_display_error("Invalid option.");
        }
    } while (choice != 0);

    free(games);
}

// Added 10.07.2025
// ersetzt void presentation_start_menu() aus presentation.c
void logic_handle_add_game(Game **games, int *game_count) {
    char title[100], description[256], version[20], mode[50];

    presentation_get_game_title(title, sizeof(title));
    presentation_get_game_description(description, sizeof(description));
    presentation_get_game_version(version, sizeof(version));
    presentation_get_game_mode(mode, sizeof(mode));

    if (!logic_is_only_spaces(title) && !logic_is_only_spaces(mode)) {
        int result = logic_add_new_game(games, game_count, title, description, version, mode);
        if (result == ERR_SUCCESS) {
            if (data_save_games(GAMES_JSON_PATH, *games, *game_count) == ERR_SUCCESS) {
                presentation_show_message("New game added.");
            } else {
                presentation_display_error("Failed to save game.");
            }
        } else {
            presentation_display_error("Failed to add game.");
        }
    } else {
        presentation_display_error("Title and Mode must not be empty.");
    }
}

void logic_handle_edit_game(Game *games, int game_count) {
    char input_id[16], new_title[100];
    int game_id;

    presentation_get_game_id_to_edit(input_id, sizeof(input_id));
    game_id = atoi(input_id);

    presentation_get_new_game_title(new_title, sizeof(new_title));

    if (logic_is_only_spaces(new_title)) {
        presentation_display_error("New title must not be empty.");
        return;
    }

    if (logic_edit_game(games, game_count, game_id, new_title) == ERR_SUCCESS) {
        if (data_save_games(GAMES_JSON_PATH, games, game_count) == ERR_SUCCESS) {
            presentation_show_message("Game edited.");
        } else {
            presentation_display_error("Failed to save changes.");
        }
    } else {
        presentation_display_error("Game not found.");
    }
}

void logic_handle_delete_game(Game **games, int *game_count) {
    char input_id[16];
    int game_id;

    presentation_get_game_id_to_delete(input_id, sizeof(input_id));
    game_id = atoi(input_id);

    if (logic_delete_game(*games, game_count, game_id) == ERR_SUCCESS) {
        if (data_save_games(GAMES_JSON_PATH, *games, *game_count) == ERR_SUCCESS) {
            presentation_show_message("Game deleted.");
        } else {
            presentation_display_error("Failed to save changes.");
        }
    } else {
        presentation_display_error("Game not found.");
    }
}

// ersetzt presentation_display_games aus presentation.c
void logic_display_games_formatted(const Game *games, int game_count) {
    char **lines = NULL;
    int count = 0;

    if (logic_get_game_lines_for_display(games, game_count, &lines, &count) != ERR_SUCCESS) {
        presentation_show_error("Could not format game list.");
        return;
    }

    // Ausgabe delegieren
    presentation_display_users_formatted(lines, count);  // Wiederverwendet: zeigt einfach Zeilen an

    // Aufräumen
    for (int i = 0; i < count; ++i) {
        free(lines[i]);
    }
    free(lines);
}

// ersetzt presentation_display_games aus presentation.c
int logic_get_game_lines_for_display(const Game *games, int game_count, char ***lines_out, int *line_count_out) {
    if (!games || game_count <= 0 || !lines_out || !line_count_out) {
        return ERR_STORAGE_FAILURE;
    }

    char **lines = malloc(sizeof(char*) * game_count * 10); // max 10 Zeilen pro Spiel
    if (!lines) return ERR_STORAGE_FAILURE;

    int idx = 0;
    char buffer[256];

    for (int i = 0; i < game_count; i++) {
        snprintf(buffer, sizeof(buffer), "Game %d:", i + 1);
        lines[idx++] = strdup(buffer);

        snprintf(buffer, sizeof(buffer), "Title: %s", games[i].title);
        lines[idx++] = strdup(buffer);

        snprintf(buffer, sizeof(buffer), "Description: %.240s", games[i].description);
        lines[idx++] = strdup(buffer);

        snprintf(buffer, sizeof(buffer), "Version: %s", games[i].version);
        lines[idx++] = strdup(buffer);

        snprintf(buffer, sizeof(buffer), "Mode: %s", games[i].mode);
        lines[idx++] = strdup(buffer);

        snprintf(buffer, sizeof(buffer), "Current Streams: %d", games[i].current_streams);
        lines[idx++] = strdup(buffer);

        lines[idx++] = strdup("--------------------------");
    }

    *lines_out = lines;
    *line_count_out = idx;
    return ERR_SUCCESS;
}

//ZINEDIN
int logic_validate_required_field(const char* str) {
    return str && strlen(str) > 0 && !logic_is_only_spaces(str);
}
