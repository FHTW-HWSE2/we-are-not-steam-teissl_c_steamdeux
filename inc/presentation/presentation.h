#ifndef PRESENTATION_H
#define PRESENTATION_H
#define MAX_USER_INPUT 100

#include "../src/data/game.h"

// Core presentation functions
void presentation_collect_and_save_report(void);
void presentation_start_admin_menu();
void presentation_add_user();
void presentation_display_users();
void presentation_remove_user();
void presentation_edit_user();
void presentation_start_game_management_menu();
void presentation_show_top_users_terminal();
void presentation_generate_top_users_file();
void presentation_show_message(const char *msg);
const char* presentation_get_report_title(void);
const char* presentation_get_report_description(void);
const char* presentation_get_report_date(void);

// Consolidated functions from other presentation files
void presentation_display_games(const Game games[], int game_count);
void presentation_display_error(const char *message);
int presentation_run(void);
void presentation_start_menu(void);

// Deklarationen für passive UI-Funktionen
void presentation_display_main_menu(void);
int presentation_get_main_menu_choice(void);
void presentation_show_startup_info(int removed_count, int changed_flags);
// Zeigt das User Management Menü an
void presentation_display_user_menu(void);
// Holt die Auswahl für das User Management Menü
int presentation_get_user_menu_choice(void);

// Passive Eingabefunktionen für das Pull-Modell
void presentation_get_full_name(char *buffer, size_t size);
void presentation_get_gamertag(char *buffer, size_t size);
void presentation_get_ssn(char *buffer, size_t size);
void presentation_get_email(char *buffer, size_t size);
void presentation_get_subscription_start(char *buffer, size_t size);
void presentation_get_subscription_end(char *buffer, size_t size);
void presentation_get_is_subscribed(char *buffer, size_t size);
void presentation_get_subscription_duration(char *buffer, size_t size);

#endif // PRESENTATION_H
