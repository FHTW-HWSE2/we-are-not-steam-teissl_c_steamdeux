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

#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_CYAN    "\x1b[36;1m"
#define ANSI_COLOR_GREEN   "\x1b[32;1m"
#define ANSI_COLOR_YELLOW  "\x1b[33;1m"
#define ANSI_COLOR_RED     "\x1b[31;1m"

#define BUFFER_SIZE 256

// Umstrukturierung 10.07.: Ich glaube das passt doch, weil presentation_show_startup_info aus logic.c aufgerufen wird
void presentation_show_startup_info(int removed_count, int changed_flags) { //statt remove count, funktion aus logic die diesen wert zurück gibt
    printf("Info: %d expired user(s) removed.\n", removed_count);
    printf("Info: %d user subscription(s) checked and updated.\n", changed_flags);
}

const char* presentation_get_report_title(void) { //schaut gut aus 
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

// valiederiugngen logic
void presentation_generate_top_users_file(void) {
    // Pure UI: call logic layer to handle file generation, only print result
    int result = logic_generate_top_users_file();
    if (result == ERR_SUCCESS) {
        printf("usersRanked.json generated.\n");
    } else {

        printf("Error: Could not generate usersRanked.json\n");
    }
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

//========================================================================
// Neue Funktion zum Starten des Game Management Menüs
void presentation_start_game_management_menu() {
    // Präsentationsschicht: Menüaufruf
    printf("\nOpening Game Management Menu...\n");
    logic_handle_game_management_menu(); // Logik übernimmt Kontrolle added 10.07.
    // presentation_start_menu();  // ruft das Menü aus menu.c auf --> 10.07. entfernt
}
// ===================== SCHICHTEN-KOMMENTARE ENDE =====================

void presentation_remove_user() { //ruft logic auf
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

// ===================== END CONSOLIDATED FUNCTIONS =====================
// UI/menu functions required for linking (stubs or real):
void presentation_display_user_menu(void) {
    printf("\n=== User Management Menu ===\n");
    printf("1. Display formatted user data to CLI.\n"); //display formatted user wieder reinholen, 1 ist raw json
    printf("2. Add a user\n");
    printf("3. Edit a user\n");
    printf("4. Delete a user\n");
    printf("5. Add a report\n");
    printf("6. Rank Top 10 Users by Playtime\n");
    printf("7. Generate player report (usersRanked.json)\n");
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

// finden valiederungen statt, logic, is schon in logic top user
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

// === All below functions were added 10.07.2025 ===

// Diese Funktion gibt eine formatierte Liste aller User zurück --> User-Menüpunkt 1. Display formatted user data to CLI
void presentation_display_users_formatted(char **lines, int count) {
    for (int i = 0; i < count; ++i) {
        printf("%s\n", lines[i]);
    }
}

// Für logic.c Funktionen: logic_handle_add_game, logic_handle_edit_game, logic_handle_delete_game
void presentation_get_game_title(char *buffer, size_t size) {
    printf("Enter title: ");
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void presentation_get_game_description(char *buffer, size_t size) {
    printf("Enter description: ");
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void presentation_get_game_version(char *buffer, size_t size) {
    printf("Enter version: ");
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void presentation_get_game_mode(char *buffer, size_t size) {
    printf("Enter mode: ");
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void presentation_get_game_id_to_edit(char *buffer, size_t size) {
    printf("Enter the game ID to edit: ");
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void presentation_get_new_game_title(char *buffer, size_t size) {
    printf("Enter the new title: ");
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void presentation_get_game_id_to_delete(char *buffer, size_t size) {
    printf("Enter the game ID to delete: ");
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void presentation_display_game_management_menu(void) {
    printf("\n=== Game Management Menu ===\n");
    printf("1. Display all games\n");
    printf("2. Add new game\n");
    printf("3. Edit a game\n");
    printf("4. Delete a game\n");
    printf("0. Return to Main Menu\n");
}

int presentation_get_game_menu_choice(void) {
    char input[16];
    printf("Choose an option: ");
    fgets(input, sizeof(input), stdin);
    return atoi(input);
}

void presentation_show_message(const char *msg) {
    printf("\033[32;1m%s\033[0m\n", msg);
}