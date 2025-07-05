#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE
#include <unistd.h>
#include "presentation.h"
#include "../inc/logic/logic.h"
#include "../inc/presentation/run.h"
#include "../inc/data/data.h"
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "presentation.h"
#include "../presentation/menu.h"
#include <time.h>
#include <unistd.h>
#include "../inc/error.h" // For error codes

#define BUFFER_SIZE 256

// Comparison function for sorting users by hours
static int compare_users_by_hours(const void *a, const void *b) {
    const cJSON *userA = *(const cJSON **)a;
    const cJSON *userB = *(const cJSON **)b;
    int hoursA = cJSON_GetObjectItem(userA, "player_hours")->valueint;
    int hoursB = cJSON_GetObjectItem(userB, "player_hours")->valueint;
    return hoursB - hoursA;
}

// ANSI color codes
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_CYAN    "\x1b[36;1m"
#define ANSI_COLOR_GREEN   "\x1b[32;1m"
#define ANSI_COLOR_YELLOW  "\x1b[33;1m"
#define ANSI_COLOR_RED     "\x1b[31;1m"

// ===================== SCHICHTENKOMMENTARE BEGINN =====================
// HINWEIS: Diese Datei ist die Präsentationsschicht (presentation layer).
// Sie darf NUR für Benutzereingabe/-ausgabe (I/O, printf, scanf, fgets, etc.) zuständig sein.
// KEINE Validierung, KEINE Datenzugriffe, KEINE Geschäftslogik!
// Validierung -> logic.c, Datenzugriff -> data.c
// ================================================================
//
// update_all_subscription_flags():
// SCHICHTVERLETZUNG! Diese Funktion macht Datenzugriff (FILE, cJSON) und Logik (Datum vergleichen).
// -> Alles außer printf gehört in data/logic layer ausgelagert!
//
// print_typewriter():
// OK, reine Präsentationslogik (Ausgabe).
//
// start_main_menu():
// OK, Menüführung und Ausgabe. Aber remove_expired_users() und update_all_subscription_flags() sind Schichtverletzungen, da sie Daten/Logik direkt aufrufen.
//
// read_input():
// OK für einfache Eingabe, aber Validierung (nur Leerzeichen prüfen) gehört in die Logikschicht.
//
// presentation_collect_and_save_report():
// SCHICHTVERLETZUNG! Validierung von Datum und Feldern ist hier, gehört aber in die Logikschicht. Nur read_input und printf sind erlaubt.
//
// show_top_users_terminal():
// SCHICHTVERLETZUNG! Liest und sortiert Daten direkt (FILE, cJSON, qsort). Nur printf gehört hierher. Datenzugriff und Sortierung -> data/logic layer.
//
// generate_top_users_file():
// SCHICHTVERLETZUNG! Liest, sortiert und schreibt Daten (FILE, cJSON, qsort). Nur printf gehört hierher. Datenzugriff und Sortierung -> data/logic layer.
//
// start_admin_menu():
// OK, reine Menüführung und Ausgabe.
//
// read_alpha_input(), read_email_input(), read_ssn_input():
// SCHICHTVERLETZUNG! Komplexe Validierung (Format, Zeichenprüfung) gehört in die Logikschicht. Nur Eingabeaufforderung und Weitergabe an Logik erlaubt.
//
// add_user_presentation():
// SCHICHTVERLETZUNG! Validierung von Datum, SSN, Email, etc. ist hier, gehört aber in die Logikschicht. Nur Eingabeaufforderung und Weitergabe an Logik erlaubt.
//
// display_users_presentation():
// OK, ruft Logik auf und gibt aus.
//
// remove_user_presentation():
// OK, ruft Logik auf und gibt aus.
//
// edit_user_presentation():
// SCHICHTVERLETZUNG! Validierung von Feldern, Datum, SSN, Email, etc. ist hier, gehört aber in die Logikschicht. Nur Eingabeaufforderung und Weitergabe an Logik erlaubt.
//
// start_game_management_menu():
// OK, reine Menüführung und Ausgabe.
// ===================== SCHICHTENKOMMENTARE ENDE =====================

// Helper: Update is_subscribed for all users based on current date, NEW NEW NEW
// Refactored am 04.07.2025: Nur Präsentationslogik (printf) bleibt hier.
// Die eigentliche Logik und Datenzugriffe sind jetzt in logic.c/data.c ausgelagert.
void update_all_subscription_flags() {
    // Hinweis: Da keine Logik nötig ist, wird direkt die Data-Layer-Funktion aufgerufen.
    int changed = data_update_all_subscription_flags(); // <-- Logic-Layer übersprungen
    printf("Updated %d user subscription flag(s).\n", changed);
}

// Übermenü für User Management und Game Management Menü
void print_typewriter(const char *str, useconds_t delay, const char *color) {
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

void start_main_menu() {
    // Präsentationsschicht: Menüführung, Benutzereingaben, Aufruf anderer Präsentationsfunktionen
    print_typewriter("____ _____ _____    _    __  __   ____  _____ _   ___  __    ", 1000, ANSI_COLOR_CYAN);
    print_typewriter("/ ___|_   _| ____|  / \\  |  \\/  | |  _ \\| ____| | | \\ \\/ /    ", 1000, ANSI_COLOR_CYAN);
    print_typewriter("\\___ \\ | | |  _|   / _ \\ | |\\/| | | | | |  _| | | | |\\  /     ", 1000, ANSI_COLOR_CYAN);
    print_typewriter(" ___) || | | |___ / ___ \\| |  | | | |_| | |___| |_| |/  \\   _ ", 1000, ANSI_COLOR_CYAN);
    print_typewriter("|____/ |_| |_____/_/   \\_\\_|  |_| |____/|_____|\\___//_/\\_\\ (_)", 1000, ANSI_COLOR_CYAN);
    print_typewriter("", 1000, NULL);
    printf(ANSI_COLOR_GREEN "Welcome!" ANSI_COLOR_RESET "\n");

    // Direkt beim Start ausführen
    int removed_count = 0;
    data_remove_expired_users(&removed_count);
    update_all_subscription_flags(); // keep all flags up to date

    int choice;
    while (1) {
        printf("\n" ANSI_COLOR_YELLOW "1. User Management Menu\n" "2. Game Management Menu\n" "3. Start simulation\n" "0. Exit\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN "Choose an option: " ANSI_COLOR_RESET);
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf(ANSI_COLOR_RED "Invalid input. Please enter a valid number.\n" ANSI_COLOR_RESET);
            continue;
        }
        getchar();
        switch (choice) {
            case 1:
                update_all_subscription_flags(); // always up to date before user menu
                start_admin_menu();
                break;
            case 2:
                start_game_management_menu();
                break;
            case 3:
                start_simulation();
                break;
            case 0:
                printf(ANSI_COLOR_GREEN "Exiting...\n" ANSI_COLOR_RESET);
                return;
            default:
                printf(ANSI_COLOR_RED "Invalid input. Please enter a valid number.\n" ANSI_COLOR_RESET);
        }
    }
}

// Refactored 04.07.2025: Validierung (nur Leerzeichen) ausgelagert in logic layer
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

void presentation_collect_and_save_report(void) {
    // Präsentationsschicht: Eingabe und Ausgabe
    char title[BUFFER_SIZE];
    char description[BUFFER_SIZE];
    char date[BUFFER_SIZE];

    read_input("Enter report title: ", title, BUFFER_SIZE);
    read_input("Enter report description: ", description, BUFFER_SIZE);

    // Datumsformat-Validierung in die Logikschicht ausgelagert
    while (1) {
        read_input("Enter report date (DD.MM.YYYY): ", date, BUFFER_SIZE);
        if (logic_is_valid_date_format(date)) break;
        printf("Invalid date format. Please use DD.MM.YYYY\n");
    }

    cJSON *report = logic_create_report(title, description, date);
    if (!report) {
        printf("Error creating report.\n");
        return;
    }

    if (data_save_report(report) != ERR_SUCCESS) {
        printf("Error saving report.\n");
        cJSON_Delete(report); // Da wir es nicht gespeichert haben, müssen wir selbst löschen
        return;
    }

    printf("Report saved successfully.\n");
}

// Show top 10 in terminal
void show_top_users_terminal(void) {
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

// Generate usersRanked.json file
void generate_top_users_file(void) {
    int top_n = 10;
    cJSON *top_users = logic_get_top_users(top_n);
    if (!top_users || !cJSON_IsArray(top_users)) {
        printf("Error: Could not get top users.\n");
        if (top_users) cJSON_Delete(top_users);
        return;
    }
    char *json_str = cJSON_Print(top_users);
    FILE *out = fopen("../usersRanked.json", "w");
    if (out && json_str) {
        fputs(json_str, out);
        fclose(out);
        printf("usersRanked.json generated.\n");
    } else {
        printf("Error: Could not write usersRanked.json\n");
        if (out) fclose(out);
    }
    free(json_str);
    cJSON_Delete(top_users);
}
void start_admin_menu(){
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
                // Option 1: Rohdaten anzeigen (alte Funktion, ggf. logic_get_all_users + printf als JSON)
                {
                    cJSON *users = NULL;
                    int result = logic_get_all_users(&users);
                    if (result != ERR_SUCCESS || !users) {
                        printf("Failed to load users.\n");
                    } else {
                        char *json = cJSON_Print(users);
                        printf("%s\n", json ? json : "(null)");
                        if (json) free(json);
                        cJSON_Delete(users);
                    }
                }
                break;
            case 2:
                display_users_presentation();
                break;
            case 3:
                add_user_presentation();
                break;
            case 4:
                edit_user_presentation();
                break;
            case 5:
                remove_user_presentation();
                break;
            case 6:
                run();
                break;
            case 7:
                show_top_users_terminal();
                break;
            case 8:
                generate_top_users_file();
                break;
            case 0:
                printf("Exiting...\n");
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}


void read_alpha_input(const char *prompt, char *buffer, size_t size) {
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

void read_email_input(const char *prompt, char *buffer, size_t size) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, size, stdin)) {
            buffer[strcspn(buffer, "\n")] = '\0';
            if (logic_is_valid_email(buffer)) return;
        }
        printf("Invalid email format. Please enter a valid email (e.g., user@domain.com).\n");
    }
}

void read_ssn_input(const char *prompt, char *buffer, size_t size) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, size, stdin)) {
            buffer[strcspn(buffer, "\n")] = '\0';
            if (logic_is_valid_ssn(buffer)) return;
        }
        printf("Invalid SSN. Please use format XXXX-XXXXXX or XXXX XXXXXX (e.g., 1234-567890 or 1234 567890).\n");
    }
} // Eingabefunktion endet  >validate profile

void add_user_presentation() {
    char full_name[MAX_USER_INPUT] = {};
    char gamertag[MAX_USER_INPUT] = {};
    char ssn[MAX_USER_INPUT] = {};
    char email[MAX_USER_INPUT] = {};
    char subscription_start[MAX_USER_INPUT] = {};
    char subscription_end[MAX_USER_INPUT] = {};

    printf("Please add a new user.\n");
    read_input("Enter full name: ", full_name, MAX_USER_INPUT);
    read_input("Enter gamertag: ", gamertag, MAX_USER_INPUT);
    read_input("Enter SSN (format XXXX-XXXXXX or XXXX XXXXXX): ", ssn, MAX_USER_INPUT);
    read_input("Enter email address: ", email, MAX_USER_INPUT);

    int start_choice = -1;
    while (start_choice != 0 && start_choice != 1) {
        printf("Choose subscription start date:\n");
        printf("1. Use today's date\n");
        printf("0. Enter a future date\n");
        printf("Enter your choice (1/0): ");
        char start_choice_str[MAX_USER_INPUT];
        fgets(start_choice_str, MAX_USER_INPUT, stdin);
        start_choice_str[strcspn(start_choice_str, "\n")] = '\0';
        if (strlen(start_choice_str) == 0) {
            printf("Invalid choice. Please enter 1 or 0.\n");
            continue;
        }
        start_choice = atoi(start_choice_str);
        if (start_choice != 0 && start_choice != 1) {
            printf("Invalid choice. Please enter 1 or 0.\n");
        }
    }

    if (start_choice == 1) {
        time_t now = time(NULL);
        struct tm *now_tm = localtime(&now);
        strftime(subscription_start, sizeof(subscription_start), "%d.%m.%Y", now_tm);
        printf("Subscription start date set to today: %s\n", subscription_start);
    } else {
        read_input("Enter subscription start date (DD.MM.YYYY): ", subscription_start, MAX_USER_INPUT);
    }

    int duration = 0;
    while (duration != 1 && duration != 6 && duration != 12) {
        printf("Choose subscription model:\n");
        printf("1. 1 month\n");
        printf("2. 6 months\n");
        printf("3. 12 months\n");
        printf("Enter your choice (1/2/3): ");
        char duration_choice[MAX_USER_INPUT];
        fgets(duration_choice, MAX_USER_INPUT, stdin);
        duration_choice[strcspn(duration_choice, "\n")] = '\0';
        if (strlen(duration_choice) == 0) {
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
            continue;
        }
        int option = atoi(duration_choice);
        if (option == 1) duration = 1;
        else if (option == 2) duration = 6;
        else if (option == 3) duration = 12;
        else printf("Invalid choice. Please enter 1, 2, or 3.\n");
    }

    // Let logic layer handle date calculation and validation
    // Pass empty string for subscription_end, logic will compute it
    const char* subscription_flag = "true";
    int result = logic_create_user(full_name, gamertag, ssn, email, subscription_start, "", subscription_flag);
    if (result == ERR_SUCCESS) {
        printf("\nUser added successfully!\n");
    } else {
        printf("\nFailed to add user.\n");
        switch (result) {
            case ERR_INVALID_SSN:
                printf("Error: SSN must be in format XXXX-XXXXXX (e.g., 1234-567890).\n");
                break;
            case ERR_INVALID_EMAIL:
                printf("Error: Email must contain '@' and a domain with a '.' (e.g., user@domain.com).\n");
                break;
            case ERR_INVALID_SUB_STATUS:
                printf("Error: Subscription status must be 'true' or 'false'.\n");
                break;
            case ERR_INVALID_DATE:
                printf("Error: Invalid date format. Please use DD.MM.YYYY.\n");
                break;
            case ERR_EMPTY_FIELD:
                printf("Error: All fields must be non-empty.\n");
                break;
            case ERR_PAST_DATE:
                printf("Error: Subscription start date must be today or in the future.\n");
                break;
            case ERR_STORAGE_FAILURE:
            default:
                printf("Error: Could not save user data.\n");
                break;
        }
        printf("\nData could not be saved! Please try again.\n\n");
        add_user_presentation();
    }
}

void edit_user_presentation() {
    char gamertag[MAX_USER_INPUT] = {};
    char full_name[MAX_USER_INPUT] = {};
    char ssn[MAX_USER_INPUT] = {};
    char email[MAX_USER_INPUT] = {};
    char subscription_start[MAX_USER_INPUT] = {};
    char subscription_end[MAX_USER_INPUT] = {};
    const char* subscription_flag = NULL;

    printf("Enter gamertag of user to edit: ");
    fgets(gamertag, MAX_USER_INPUT, stdin);
    gamertag[strcspn(gamertag, "\n")] = '\0';

    printf("Enter new full name (or 0 to keep current): ");
    fgets(full_name, MAX_USER_INPUT, stdin);
    full_name[strcspn(full_name, "\n")] = '\0';
    printf("Enter new SSN (format XXXX-XXXXXX or XXXX XXXXXX, or 0 to keep current): ");
    fgets(ssn, MAX_USER_INPUT, stdin);
    ssn[strcspn(ssn, "\n")] = '\0';
    printf("Enter new email address (or 0 to keep current): ");
    fgets(email, MAX_USER_INPUT, stdin);
    email[strcspn(email, "\n")] = '\0';

    int start_choice = -1;
    while (start_choice != 0 && start_choice != 1) {
        printf("Choose subscription start date:\n");
        printf("1. Use today's date\n");
        printf("0. Enter a future date\n");
        printf("Enter your choice (1/0, or 0 to keep current): ");
        char start_choice_str[MAX_USER_INPUT];
        fgets(start_choice_str, MAX_USER_INPUT, stdin);
        start_choice_str[strcspn(start_choice_str, "\n")] = '\0';
        if (strlen(start_choice_str) == 0) {
            printf("Invalid choice. Please enter 1 or 0.\n");
            continue;
        }
        start_choice = atoi(start_choice_str);
        if (start_choice != 0 && start_choice != 1) {
            printf("Invalid choice. Please enter 1 or 0.\n");
        }
    }
    if (start_choice == 1) {
        time_t now = time(NULL);
        struct tm *now_tm = localtime(&now);
        strftime(subscription_start, sizeof(subscription_start), "%d.%m.%Y", now_tm);
        printf("Subscription start date set to today: %s\n", subscription_start);
        subscription_flag = "true";
    } else if (start_choice == 0) {
        printf("Enter subscription start date (DD.MM.YYYY) [or 0 to keep current]: ");
        fgets(subscription_start, MAX_USER_INPUT, stdin);
        subscription_start[strcspn(subscription_start, "\n")] = '\0';
        if (strcmp(subscription_start, "0") == 0) subscription_start[0] = '\0';
        else subscription_flag = "false";
    }

    printf("Enter new subscription end date (or 0 to keep current): ");
    fgets(subscription_end, MAX_USER_INPUT, stdin);
    subscription_end[strcspn(subscription_end, "\n")] = '\0';
    if (strcmp(full_name, "0") == 0) full_name[0] = '\0';
    if (strcmp(ssn, "0") == 0) ssn[0] = '\0';
    if (strcmp(email, "0") == 0) email[0] = '\0';
    if (strcmp(subscription_start, "0") == 0) subscription_start[0] = '\0';
    if (strcmp(subscription_end, "0") == 0) subscription_end[0] = '\0';

    int result = logic_edit_user(gamertag, full_name, ssn, email, subscription_start, subscription_end, subscription_flag);
    if (result == ERR_SUCCESS) {
        printf("User updated successfully.\n");
    } else if (result == ERR_USER_NOT_FOUND) {
        printf("\nError: Gamertag not found. User could not be edited.\n");
    } else if (result == ERR_INVALID_SSN) {
        printf("\nError: Invalid SSN format. Must be XXXX-XXXXXX or XXXX XXXXXX.\n");
    } else if (result == ERR_INVALID_EMAIL) {
        printf("\nError: Invalid email format. Must contain '@' and '.'\n");
    } else if (result == ERR_INVALID_SUB_STATUS) {
        printf("\nError: Subscription status must be 'true' or 'false'.\n");
    } else if (result == ERR_INVALID_DATE) {
        printf("\nError: Invalid date format. Use DD.MM.YYYY.\n");
    } else if (result == ERR_EMPTY_FIELD) {
        printf("\nError: Fields cannot be empty.\n");
    } else if (result == ERR_STORAGE_FAILURE) {
        printf("\nError: Could not save updated data.\n");
    } else {
        printf("\nUnknown error occurred.\n");
    }
}

//========================================================================
// Neue Funktion zum Starten des Game Management Menüs
void start_game_management_menu() {
    // Präsentationsschicht: Menüaufruf
    printf("\nOpening Game Management Menu...\n");
    start_menu();  // ruft das Menü aus menu.c auf
}
// ===================== SCHICHTEN-KOMMENTARE ENDE =====================

void display_users_presentation() {
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

void remove_user_presentation() {
    char gamertag[MAX_USER_INPUT] = {};
    printf("Enter gamertag of user to remove: ");
    fgets(gamertag, MAX_USER_INPUT, stdin);
    gamertag[strcspn(gamertag, "\n")] = '\0';
    if (logic_remove_user(gamertag)) {
        printf("\nUser removed successfully.\n");
    } else {
        printf("\nError: User not found.\n");
    }
}