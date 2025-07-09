#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE
#include <unistd.h>
#include "presentation.h"
#include "../inc/logic/logic.h"
#include "../inc/presentation/run.h"
#include "../inc/data/data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../inc/error.h" // For error codes
#include "../simulation/simulation.h"
#include "messages.h"
#include "menu.h"
#include "cJSON.h"

#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_CYAN    "\x1b[36;1m"
#define ANSI_COLOR_GREEN   "\x1b[32;1m"
#define ANSI_COLOR_YELLOW  "\x1b[33;1m"
#define ANSI_COLOR_RED     "\x1b[31;1m"

#define BUFFER_SIZE 256

//nicht okay
void presentation_update_all_subscription_flags() {
    int changed = data_update_all_subscription_flags();
    printf("Updated %d user subscription flag(s).\n", changed);
}

// Übermenü für User Management und Game Management Menü
void presentation_print_typewriter(const char *str, useconds_t delay, const char *color) {
    // Präsentationsschicht: Nur Ausgabe
    if (color) printf("%s", color);
    for (size_t i = 0; i < strlen(str); ++i) {
        putchar(str[i]);
        fflush(stdout);
        usleep(delay);
    }
    putchar('\n');
    if (color) printf(ANSI_COLOR_RESET);
}

void presentation_show_startup_info(int removed_count, int changed_flags) {
    printf("Info: %d expired user(s) removed.\n", removed_count);
    printf("Info: %d user subscription(s) checked and updated.\n", changed_flags);
}

// Refactored 04.07.2025: Validierung (nur Leerzeichen) ausgelagert in logic layer,
static void read_input(const char *prompt, char *buffer, size_t size) {
    int valid = 0;
    while (!valid) {
        printf("%s", prompt);
        if (fgets(buffer, size, stdin)) {
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }
            if (strlen(buffer) == 0) {
                printf("Invalid input. Please enter a valid value.\n");
                continue;
            }
            if (logic_is_only_spaces(buffer)) {
                printf("Input must not be only spaces. Please enter a valid value.\n");
                continue;
            }
            valid = 1;
        } else {
            printf("Invalid input. Please enter a valid value.\n");
        }
    }
}

void presentation_show_message(const char *msg) {
    printf("%s\n", msg);
}

const char* presentation_get_report_title(void) {
    static char title[BUFFER_SIZE];
    printf("Enter report title: ");
    fgets(title, BUFFER_SIZE, stdin);
    title[strcspn(title, "\n")] = 0;
    return title;
}

const char* presentation_get_report_description(void) {
    static char description[BUFFER_SIZE];
    printf("Enter report description: ");
    fgets(description, BUFFER_SIZE, stdin);
    description[strcspn(description, "\n")] = 0;
    return description;
}

const char* presentation_get_report_date(void) {
    static char date[BUFFER_SIZE];
    printf("Enter report date (DD.MM.YYYY): ");
    fgets(date, BUFFER_SIZE, stdin);
    date[strcspn(date, "\n")] = 0;
    return date;
}

// Show top 10 in terminal
void presentation_show_top_users_terminal(void) {
    int top_n = 10;
    cJSON *top_users = logic_get_top_users(top_n);
    if (!top_users || !cJSON_IsArray(top_users)) {
        printf("No users found or error loading users.\n");
        if (top_users) cJSON_Delete(top_users);
        return;
    }
    int count = cJSON_GetArraySize(top_users);
    if (count == 0) {
        printf("No users found.\n");
        cJSON_Delete(top_users);
        return;
    }
    printf("Top %d users by playtime:\n", count);
    for (int i = 0; i < count; ++i) {
        cJSON *user = cJSON_GetArrayItem(top_users, i);
        cJSON *gamertag = cJSON_GetObjectItem(user, "gamertag");
        cJSON *player_hours = cJSON_GetObjectItem(user, "player_hours");
        printf("%d. %s - %d hours\n", i + 1,
            gamertag && cJSON_IsString(gamertag) ? gamertag->valuestring : "(unknown)",
            player_hours && cJSON_IsNumber(player_hours) ? player_hours->valueint : 0);
    }
    cJSON_Delete(top_users);
}

// NOCH FALSCH
void presentation_generate_top_users_file(void) {
    // Pure UI: call logic layer to handle file generation, only print result
    int result = logic_generate_top_users_file();
    if (result == ERR_SUCCESS) {
        printf("usersRanked.json generated.\n");
    } else {// ===================== SCHICHTENKOMMENTARE BEGINN =====================

        printf("Error: Could not generate usersRanked.json\n");
    }
}
void presentation_start_admin_menu(){
    // Präsentationsschicht: Menüführung, Benutzereingaben, Aufruf anderer Präsentationsfunktionen
    printf("\n=== User Managment Menu ===\n");
    char choice[MAX_USER_INPUT];

    while (1){
        printf("---------------\n");
        printf("1. Print user.json data in the CLI.\n");
        printf("2. Display formatted user data to CLI.\n");
        printf("3. Add a user\n");
        printf("4. Edit a user\n");
        printf("5. Delete a user\n");
        printf("6. Add a report\n");
        printf("7. Rank Top 10 Users by Playtime\n"); // <-- NEW
        printf("8. Generate player report (usersRanked.json)\n"); // <-- NEU
        printf("0. Return to Main Menu\n");
        printf("Choose an option: ");
        fgets(choice, MAX_USER_INPUT, stdin);

        int option = atoi(choice);
        switch (option) {
            case 1:
                // Option 1: Print raw user JSON via logic layer
                logic_handle_list_users_json_workflow();
                break;
            case 2:
                presentation_display_users();
                break;
            case 3:
                presentation_add_user();
                break;
            case 4:
                presentation_edit_user();
                break;
            case 5:
                presentation_remove_user();
                break;
            case 6:
                presentation_run();
                break;
            case 7:
                presentation_show_top_users_terminal();
                break;
            case 8:
                presentation_generate_top_users_file();
                break;
            case 0:
                printf("Exiting...\n");
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}


void presentation_read_alpha_input(const char *prompt, char *buffer, size_t size) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, size, stdin)) {
            buffer[strcspn(buffer, "\n")] = '\0';
            if (strlen(buffer) == 0) {
                printf("Invalid input. Please enter only letters and spaces.\n");
                continue;
            }
            if (logic_is_only_spaces(buffer)) {
                printf("Input must not be only spaces. Please enter only letters and spaces.\n");
                continue;
            }
            if (logic_is_valid_alpha(buffer)) return;
        }
        printf("Invalid input. Please enter only letters and spaces.\n");
    }
}

void presentation_read_email_input(const char *prompt, char *buffer, size_t size) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, size, stdin)) {
            buffer[strcspn(buffer, "\n")] = '\0';
            if (logic_is_valid_email(buffer)) return;
        }
        printf("Invalid email format. Please enter a valid email (e.g., user@domain.com).\n");
    }
}

void presentation_read_ssn_input(const char *prompt, char *buffer, size_t size) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, size, stdin)) {
            buffer[strcspn(buffer, "\n")] = '\0';
            if (logic_is_valid_ssn(buffer)) return;
        }
        printf("Invalid SSN. Please use format XXXX-XXXXXX or XXXX XXXXXX (e.g., 1234-567890 or 1234 567890).\n");
    }
} // Eingabefunktion endet  >validate profile

/*
void presentation_add_user() {
    // 1. DUMM DATEN SAMMELN (ohne jede Prüfung)
    char full_name[BUFFER_SIZE], gamertag[BUFFER_SIZE], ssn[BUFFER_SIZE], email[BUFFER_SIZE], sub_start[BUFFER_SIZE];
    char start_choice_str[BUFFER_SIZE], duration_choice_str[BUFFER_SIZE];
    
    printf("Please add a new user.\n");
    
    // Sammle alle Eingaben ohne Validation
    read_input("Enter full name: ", full_name, sizeof(full_name));
    read_input("Enter gamertag: ", gamertag, sizeof(gamertag));
    read_input("Enter SSN (format XXXX-XXXXXX or XXXX XXXXXX): ", ssn, sizeof(ssn));
    read_input("Enter email address: ", email, sizeof(email));
    
    printf("Choose subscription start date:\n");
    printf("1. Use today's date\n");
    printf("0. Enter a future date\n");
    read_input("Enter your choice (1/0): ", start_choice_str, sizeof(start_choice_str));
    int use_today = atoi(start_choice_str);
    
    // Falls kein "heute", frage nach Datum
    if (use_today != 1) {
        read_input("Enter subscription start date (DD.MM.YYYY): ", sub_start, sizeof(sub_start));
    } else {
        strcpy(sub_start, ""); // Logik entscheidet über heutiges Datum
    }
    
    printf("Choose subscription model:\n");
    printf("1. 1 month\n");
    printf("2. 6 months\n");
    printf("3. 12 months\n");
    read_input("Enter your choice (1/2/3): ", duration_choice_str, sizeof(duration_choice_str));
    int duration_option = atoi(duration_choice_str);
    
    // Map choice to duration
    int duration = 1; // default
    if (duration_option == 2) duration = 6;
    else if (duration_option == 3) duration = 12;
    
    // 2. EINEN EINZIGEN AUFRUF an das "Gehirn"
    const char *is_subscribed_str = "true";
    int result = logic_create_user_with_duration(full_name, gamertag, ssn, email, sub_start, is_subscribed_str, duration, use_today);
    
    // 3. DUMM AUF DIE ANTWORT REAGIEREN
    switch (result) {
        case ERR_SUCCESS:
            printf("\nUser added successfully!\n");
            break;
        case ERR_INVALID_EMAIL:
            printf("\nError: The email format is invalid.\n");
            break;
        case ERR_PAST_DATE:
            printf("\nError: The start date cannot be in the past.\n");
            break;
        case ERR_INVALID_SSN:
            printf("\nError: SSN must be in format XXXX-XXXXXX (e.g., 1234-567890).\n");
            break;
        case ERR_INVALID_SUB_STATUS:
            printf("\nError: Subscription status must be 'true' or 'false'.\n");
            break;
        case ERR_INVALID_DATE:
            printf("\nError: Invalid date format. Please use DD.MM.YYYY.\n");
            break;
        case ERR_EMPTY_FIELD:
            printf("\nError: All fields must be non-empty.\n");
            break;
        case ERR_STORAGE_FAILURE:
            printf("\nError: Could not save user data.\n");
            break;
        default:
            printf("\nAn unknown error occurred.\n");
            break;
    }
}

void presentation_edit_user() {
    char gamertag[MAX_USER_INPUT], new_full_name[MAX_USER_INPUT], new_ssn[MAX_USER_INPUT], new_email[MAX_USER_INPUT];
    char sub_start[MAX_USER_INPUT], sub_end[MAX_USER_INPUT], is_subscribed_str[MAX_USER_INPUT];
    
    printf("Enter gamertag of user to edit: ");
    fgets(gamertag, sizeof(gamertag), stdin);
    gamertag[strcspn(gamertag, "\n")] = 0;
    
    printf("Enter new full name (or leave empty to keep current): ");
    fgets(new_full_name, sizeof(new_full_name), stdin);
    new_full_name[strcspn(new_full_name, "\n")] = 0;
    
    printf("Enter new SSN (or leave empty to keep current): ");
    fgets(new_ssn, sizeof(new_ssn), stdin);
    new_ssn[strcspn(new_ssn, "\n")] = 0;
    
    printf("Enter new email (or leave empty to keep current): ");
    fgets(new_email, sizeof(new_email), stdin);
    new_email[strcspn(new_email, "\n")] = 0;
    
    printf("Enter new subscription start date (DD.MM.YYYY) (or leave empty to keep current): ");
    fgets(sub_start, sizeof(sub_start), stdin);
    sub_start[strcspn(sub_start, "\n")] = 0;
    
    printf("Enter new subscription end date (DD.MM.YYYY) (or leave empty to keep current): ");
    fgets(sub_end, sizeof(sub_end), stdin);
    sub_end[strcspn(sub_end, "\n")] = 0;
    
    printf("Is subscribed? (1 = yes, 0 = no, or leave empty to keep current): ");
    fgets(is_subscribed_str, sizeof(is_subscribed_str), stdin);
    is_subscribed_str[strcspn(is_subscribed_str, "\n")] = 0;
    
    int result = logic_edit_user(gamertag, new_full_name, new_ssn, new_email, sub_start, sub_end, is_subscribed_str);
    switch (result) {
        case ERR_SUCCESS:
            printf("User edited successfully.\n");
            break;
        case ERR_USER_NOT_FOUND:
            printf("Error: User not found.\n");
            break;
        case ERR_INVALID_SSN:
            printf("Error: Invalid SSN format.\n");
            break;
        case ERR_INVALID_EMAIL:
            printf("Error: Invalid email format.\n");
            break;
        case ERR_INVALID_DATE:
            printf("Error: Invalid date format.\n");
            break;
        case ERR_PAST_DATE:
            printf("Error: Date cannot be in the past.\n");
            break;
        case ERR_EMPTY_FIELD:
            printf("Error: Fields cannot be empty.\n");
            break;
        case ERR_STORAGE_FAILURE:
            printf("Error: Could not save data.\n");
            break;
        default:
            printf("Unknown error occurred.\n");
    }
}
*/

//========================================================================
// Neue Funktion zum Starten des Game Management Menüs
void presentation_start_game_management_menu() {
    // Präsentationsschicht: Menüaufruf
    printf("\nOpening Game Management Menu...\n");
    presentation_start_menu();  // ruft das Menü aus menu.c auf
}
// ===================== SCHICHTEN-KOMMENTARE ENDE =====================

void presentation_display_users() {
    cJSON *users = NULL;
    int result = logic_get_all_users(&users);
    if (result != 0 || !users || !cJSON_IsArray(users)) {
        printf("Failed to load users.\n");
        if (users) cJSON_Delete(users);
        return;
    }
    int count = cJSON_GetArraySize(users);
    if (count == 0) {
        printf("No users found.\n");
        cJSON_Delete(users);
        return;
    }
    printf("=== User List ===\n");
    for (int i = 0; i < count; ++i) {
        cJSON *user = cJSON_GetArrayItem(users, i);
        cJSON *full_name = cJSON_GetObjectItem(user, "full_name");
        cJSON *gamertag = cJSON_GetObjectItem(user, "gamertag");
        cJSON *player_hours = cJSON_GetObjectItem(user, "player_hours");
        cJSON *ssn = cJSON_GetObjectItem(user, "ssn");
        cJSON *email = cJSON_GetObjectItem(user, "email");
        cJSON *sub_start = cJSON_GetObjectItem(user, "subscription_start_date");
        cJSON *sub_end = cJSON_GetObjectItem(user, "subscription_end_date");
        cJSON *is_sub = cJSON_GetObjectItem(user, "is_subscribed");
        printf("User %d:\n  Name: %s\n  Gamertag: %s\n  Hours: %d\n  SSN: %s\n  Email: %s\n  Sub Start: %s\n  Sub End: %s\n  Subscribed: %s\n\n",
            i+1,
            full_name && cJSON_IsString(full_name) ? full_name->valuestring : "",
            gamertag && cJSON_IsString(gamertag) ? gamertag->valuestring : "",
            player_hours && cJSON_IsNumber(player_hours) ? player_hours->valueint : 0,
            ssn && cJSON_IsString(ssn) ? ssn->valuestring : "",
            email && cJSON_IsString(email) ? email->valuestring : "",
            sub_start && cJSON_IsString(sub_start) ? sub_start->valuestring : "",
            sub_end && cJSON_IsString(sub_end) ? sub_end->valuestring : "",
            is_sub && cJSON_IsBool(is_sub) ? (is_sub->valueint ? "Yes" : "No") : "No");
    }
    cJSON_Delete(users);
}

void presentation_remove_user() {
    char gamertag[MAX_USER_INPUT] = {};
    printf("Enter gamertag of user to remove: ");
    fgets(gamertag, MAX_USER_INPUT, stdin);
    gamertag[strcspn(gamertag, "\n")] = '\0';
    
    int result = logic_remove_user(gamertag);
    switch (result) {
        case ERR_SUCCESS:
            printf("\nUser removed successfully.\n");
            break;
        case ERR_USER_NOT_FOUND:
            printf("\nError: User not found.\n");
            break;
        case ERR_STORAGE_FAILURE:
            printf("\nError: Failed to save changes to storage.\n");
            break;
        default:
            printf("\nError: An unexpected error occurred.\n");
            break;
    }
}

// ===================== CONSOLIDATED PRESENTATION FUNCTIONS =====================

// From display_games.c
void presentation_display_games(const Game games[], int game_count) {
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

// From menu.c
void presentation_start_menu() {
    Game *games = NULL;
    int game_count = 0;
    int choice;

    if (logic_initialize_game_data_loading(GAMES_JSON_PATH, &games, &game_count) != 0) {
        presentation_display_error("Failed to load games.");
        return;
    }

    do {
        printf("\n=== Game Management Menu ===\n");
        printf("1. Display all games\n");
        printf("2. Add new game\n");
        printf("3. Edit a game\n");
        printf("4. Delete a game\n");
        printf("0. Exit\n");
        printf("Choose an option: ");
        char input[16];
        fgets(input, sizeof(input), stdin);
        choice = atoi(input);

        switch (choice) {
            case 1:
                presentation_display_games(games, game_count);
                break;

            case 2: {
                char title[100], description[256], version[10], mode[20];
                printf("Enter title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;
                printf("Enter description: ");
                fgets(description, sizeof(description), stdin);
                description[strcspn(description, "\n")] = 0;
                printf("Enter version: ");
                fgets(version, sizeof(version), stdin);
                version[strcspn(version, "\n")] = 0;
                printf("Enter mode: ");
                fgets(mode, sizeof(mode), stdin);
                mode[strcspn(mode, "\n")] = 0;
                if (logic_add_new_game(&games, &game_count, title, description, version, mode) == 0) {
                    data_save_games(GAMES_JSON_PATH, games, game_count);
                    printf("New game added.\n");
                } else {
                    presentation_display_error("Failed to add game.");
                }
                break;
            }
            case 3: {
                char input_id[16];
                int game_id;
                printf("Enter the game ID to edit: ");
                fgets(input_id, sizeof(input_id), stdin);
                game_id = atoi(input_id);
                char new_title[100];
                printf("Enter the new title: ");
                fgets(new_title, sizeof(new_title), stdin);
                new_title[strcspn(new_title, "\n")] = 0;
                if (logic_edit_game(games, game_count, game_id, new_title) == 0) {
                    data_save_games(GAMES_JSON_PATH, games, game_count);
                    printf("Game edited.\n");
                } else {
                    presentation_display_error("Game not found.");
                }
                break;
            }
            case 4: {
                char input_id[16];
                int game_id;
                printf("Enter the game ID to delete: ");
                fgets(input_id, sizeof(input_id), stdin);
                game_id = atoi(input_id);
                if (logic_delete_game(games, &game_count, game_id) == 0) {
                    data_save_games(GAMES_JSON_PATH, games, game_count);
                    printf("Game deleted.\n");
                } else {
                    presentation_display_error("Game not found.");
                }
                break;
            }
            case 0:
                printf("Exiting...\n");
                break;
            default:
                presentation_display_error("Invalid option.");
                break;
        }
    } while (choice != 0);
    free(games);
}

// ===================== END CONSOLIDATED FUNCTIONS =====================
// UI/menu functions required for linking (stubs or real):
void presentation_display_user_menu(void) {
    printf("\n=== User Management Menu ===\n");
    printf("1. Print user.json data in the CLI.\n");
    printf("2. Display formatted user data to CLI.\n");
    printf("3. Add a user\n");
    printf("4. Edit a user\n");
    printf("5. Delete a user\n");
    printf("6. Add a report\n");
    printf("7. Rank Top 10 Users by Playtime\n");
    printf("8. Generate player report (usersRanked.json)\n");
    printf("0. Return to Main Menu\n");
}
int presentation_get_user_menu_choice(void) {
    char input[16];
    printf("Choose an option: ");
    fgets(input, sizeof(input), stdin);
    return atoi(input);
}
void presentation_get_full_name(char *buffer, size_t size) {
    printf("Enter full name: ");
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}
void presentation_get_gamertag(char *buffer, size_t size) {
    printf("Enter gamertag: ");
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}
void presentation_get_ssn(char *buffer, size_t size) {
    printf("Enter SSN (format XXXX-XXXXXX or XXXX XXXXXX): ");
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}
void presentation_get_email(char *buffer, size_t size) {
    printf("Enter email address: ");
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}
void presentation_get_subscription_start(char *buffer, size_t size) {
    printf("Enter subscription start date (DD.MM.YYYY): ");
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}
void presentation_get_subscription_duration(char *buffer, size_t size) {
    printf("Enter subscription duration in months (1/6/12): ");
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}
void presentation_display_error(const char *message) {
    printf("Error: %s\n", message);
}
int presentation_run() {
    // Dummy implementation
    return 0;
}
void presentation_add_user() {}
void presentation_edit_user() {}
void presentation_show_error(const char *message) {
    printf(ANSI_COLOR_RED "Error: %s\n" ANSI_COLOR_RESET, message);
}

const char* presentation_get_user_gamertag(void) {
    static char gamertag[BUFFER_SIZE];
    printf("Enter gamertag: ");
    fgets(gamertag, sizeof(gamertag), stdin);
    gamertag[strcspn(gamertag, "\n")] = 0;
    return gamertag;
}

void presentation_display_user_list(const char* json_data) {
    if (!json_data) {
        printf("(No data to display)\n");
        return;
    }
    cJSON *users = cJSON_Parse(json_data);
    if (!users || !cJSON_IsArray(users)) {
        printf("(Invalid user data)\n");
        if (users) cJSON_Delete(users);
        return;
    }
    printf("\n%-4s %-20s %-15s %-12s %-25s %-12s %-12s %-5s\n", "Nr.", "Full Name", "Gamertag", "SSN", "Email", "Sub Start", "Sub End", "Sub");
    printf("%s\n", "---------------------------------------------------------------------------------------------------------------");
    int idx = 1;
    cJSON *user = NULL;
    cJSON_ArrayForEach(user, users) {
        const char *full_name = cJSON_GetObjectItem(user, "full_name") ? cJSON_GetObjectItem(user, "full_name")->valuestring : "";
        const char *gamertag = cJSON_GetObjectItem(user, "gamertag") ? cJSON_GetObjectItem(user, "gamertag")->valuestring : "";
        const char *ssn = cJSON_GetObjectItem(user, "ssn") ? cJSON_GetObjectItem(user, "ssn")->valuestring : "";
        const char *email = cJSON_GetObjectItem(user, "email") ? cJSON_GetObjectItem(user, "email")->valuestring : "";
        const char *sub_start = cJSON_GetObjectItem(user, "sub_start") ? cJSON_GetObjectItem(user, "sub_start")->valuestring : "";
        const char *sub_end = cJSON_GetObjectItem(user, "sub_end") ? cJSON_GetObjectItem(user, "sub_end")->valuestring : "";
        int is_sub = cJSON_GetObjectItem(user, "is_subscribed") ? cJSON_GetObjectItem(user, "is_subscribed")->valueint : 0;
        printf("%-4d %-20s %-15s %-12s %-25s %-12s %-12s %-5s\n", idx++, full_name, gamertag, ssn, email, sub_start, sub_end, is_sub ? "yes" : "no");
    }
    cJSON_Delete(users);
}