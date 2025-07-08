#define _XOPEN_SOURCE // Define für die strptime Funktion, um Datumsformate zu prüfen
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h> // Für validate_player_profile() Funktion um das Startdatum zu prüfen
#include "../inc/logic/logic.h"
#include "../inc/data/data.h"
#include "../inc/error.h" // Für Fehlercodes
#include "cJSON.h"
#include "../inc/presentation/presentation.h"

#define BUFFER_SIZE 256

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

// Prototypen für interne Funktionen
static void logic_user_menu_workflow(void);
static void logic_handle_add_user_workflow(void);
static void logic_handle_edit_user_workflow(void);
static void read_input(const char *prompt, char *buffer, size_t size);

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

// Refactored 05.07.2025: Funktion wieder hinzugefügt für saubere Schichtentrennung
int logic_update_all_subscription_flags() {
    return data_update_all_subscription_flags();
}

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

// Generate top users file - moved from presentation layer
int logic_generate_top_users_file(void) {
    int top_n = 10;
    cJSON *top_users = logic_get_top_users(top_n);
    if (!top_users || !cJSON_IsArray(top_users)) {
        if (top_users) cJSON_Delete(top_users);
        return ERR_STORAGE_FAILURE;
    }
    char *json_str = cJSON_Print(top_users);
    FILE *out = fopen("../usersRanked.json", "w");
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
        presentation_show_message("All fields must be non-empty.");
        return;
    }
    if (!logic_is_valid_date_format(date)) {
        presentation_show_message("Invalid date format. Please use DD.MM.YYYY");
        return;
    }

    cJSON *report = logic_create_report(title, description, date);
    if (!report) {
        presentation_show_message("Error creating report.");
        return;
    }
    int save_result = data_save_report(report);
    cJSON_Delete(report);

    if (save_result != ERR_SUCCESS) {
        presentation_show_message("Error saving report.");
    } else {
        presentation_show_message("Report saved successfully.");
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
        presentation_show_error("Critical error during startup tasks.");
    }

    while (1) {
        presentation_display_main_menu();
        int choice = presentation_get_main_menu_choice();

        if (choice == 0) {
            presentation_show_message("Exiting...");
            break;
        }

        switch (choice) {
            case 1:
                logic_user_menu_workflow();
                break;
            case 2:
                presentation_show_message("Display formatted user data selected (Workflow to be implemented).");
                break;
            case 3: // Add a user
                logic_handle_add_user_workflow();
                break;
            case 4: // Edit a user
                logic_handle_edit_user_workflow();
                break;
            case 5:
                presentation_show_message("Delete user selected (Workflow to be implemented).");
                break;
            case 6:
                presentation_show_message("Add report selected (Workflow to be implemented).");
                break;
            case 7:
                presentation_show_message("Rank Top 10 Users selected (Workflow to be implemented).");
                break;
            case 8:
                presentation_show_message("Generate player report selected (Workflow to be implemented).");
                break;
            default:
                presentation_show_error("Invalid option. Please try again.");
        }
    }
}

// Diese Funktion steuert das User Management Menü
static void logic_user_menu_workflow(void) {
    while (1) {
        presentation_display_user_menu();
        int choice = presentation_get_user_menu_choice();
        if (choice == 0) {
            presentation_show_message("Returning to main menu...");
            break;
        }
        switch (choice) {
            case 3: // Add a user
                logic_handle_add_user_workflow();
                break;
            case 4: // Edit a user
                logic_handle_edit_user_workflow();
                break;
            // ... weitere cases für die anderen Menüpunkte ...
            default:
                presentation_show_error("Invalid option. Please try again.");
        }
    }
}

// Diese Funktion steuert das Hinzufügen eines Users im Pull-Modell
static void logic_handle_add_user_workflow(void) {
    char full_name[BUFFER_SIZE], gamertag[BUFFER_SIZE], ssn[BUFFER_SIZE], email[BUFFER_SIZE], sub_start[BUFFER_SIZE], duration_str[BUFFER_SIZE];
    int use_today = 0, duration = 1;

    presentation_show_message("Please add a new user.");
    presentation_get_full_name(full_name, sizeof(full_name));
    presentation_get_gamertag(gamertag, sizeof(gamertag));
    presentation_get_ssn(ssn, sizeof(ssn));
    presentation_get_email(email, sizeof(email));

    // Startdatum-Auswahl
    presentation_show_message("Choose subscription start date:\n1. Use today's date\n0. Enter a future date");
    read_input("Enter your choice (1/0): ", sub_start, sizeof(sub_start));
    use_today = atoi(sub_start);
    if (use_today != 1) {
        presentation_get_subscription_start(sub_start, sizeof(sub_start));
    } else {
        strcpy(sub_start, ""); // Logik entscheidet über heutiges Datum
    }

    // Dauer-Auswahl
    presentation_show_message("Choose subscription model:\n1. 1 month\n2. 6 months\n3. 12 months");
    presentation_get_subscription_duration(duration_str, sizeof(duration_str));
    int duration_option = atoi(duration_str);
    if (duration_option == 2) duration = 6;
    else if (duration_option == 3) duration = 12;
    else duration = 1;

    const char *is_subscribed_str = "true";
    int result = logic_create_user_with_duration(full_name, gamertag, ssn, email, sub_start, is_subscribed_str, duration, use_today);

    switch (result) {
        case ERR_SUCCESS:
            presentation_show_message("User added successfully!");
            break;
        case ERR_INVALID_EMAIL:
            presentation_show_error("The email format is invalid.");
            break;
        case ERR_PAST_DATE:
            presentation_show_error("The start date cannot be in the past.");
            break;
        case ERR_INVALID_SSN:
            presentation_show_error("SSN must be in format XXXX-XXXXXX (e.g., 1234-567890).");
            break;
        case ERR_INVALID_SUB_STATUS:
            presentation_show_error("Subscription status must be 'true' or 'false'.");
            break;
        case ERR_INVALID_DATE:
            presentation_show_error("Invalid date format. Please use DD.MM.YYYY.");
            break;
        case ERR_EMPTY_FIELD:
            presentation_show_error("All fields must be non-empty.");
            break;
        case ERR_STORAGE_FAILURE:
            presentation_show_error("Could not save user data.");
            break;
        default:
            presentation_show_error("An unknown error occurred.");
            break;
    }
}

// Diese Funktion steuert das Bearbeiten eines Users im Pull-Modell
static void logic_handle_edit_user_workflow(void) {
    char gamertag[MAX_USER_INPUT], new_full_name[MAX_USER_INPUT], new_ssn[MAX_USER_INPUT], new_email[MAX_USER_INPUT];
    char sub_start[MAX_USER_INPUT], sub_end[MAX_USER_INPUT], is_subscribed_str[MAX_USER_INPUT];

    presentation_get_gamertag(gamertag, sizeof(gamertag));
    presentation_show_message("Enter new full name (or leave empty to keep current):");
    read_input("", new_full_name, sizeof(new_full_name));
    presentation_show_message("Enter new SSN (or leave empty to keep current):");
    read_input("", new_ssn, sizeof(new_ssn));
    presentation_show_message("Enter new email (or leave empty to keep current):");
    read_input("", new_email, sizeof(new_email));
    presentation_show_message("Enter new subscription start date (DD.MM.YYYY) (or leave empty to keep current):");
    read_input("", sub_start, sizeof(sub_start));
    presentation_show_message("Enter new subscription end date (DD.MM.YYYY) (or leave empty to keep current):");
    read_input("", sub_end, sizeof(sub_end));
    presentation_show_message("Is subscribed? (1 = yes, 0 = no, or leave empty to keep current):");
    read_input("", is_subscribed_str, sizeof(is_subscribed_str));

    int result = logic_edit_user(gamertag, new_full_name, new_ssn, new_email, sub_start, sub_end, is_subscribed_str);
    switch (result) {
        case ERR_SUCCESS:
            presentation_show_message("User edited successfully.");
            break;
        case ERR_USER_NOT_FOUND:
            presentation_show_error("User not found.");
            break;
        case ERR_INVALID_SSN:
            presentation_show_error("Invalid SSN format.");
            break;
        case ERR_INVALID_EMAIL:
            presentation_show_error("Invalid email format.");
            break;
        case ERR_INVALID_DATE:
            presentation_show_error("Invalid date format.");
            break;
        case ERR_PAST_DATE:
            presentation_show_error("Date cannot be in the past.");
            break;
        case ERR_EMPTY_FIELD:
            presentation_show_error("Fields cannot be empty.");
            break;
        case ERR_STORAGE_FAILURE:
            presentation_show_error("Could not save data.");
            break;
        default:
            presentation_show_error("Unknown error occurred.");
    }
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