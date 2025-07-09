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

// --- moved to menu.h ---
// void presentation_display_main_menu(void);
// int presentation_get_main_menu_choice(void);
// void presentation_print_welcome_banner(void);
// --- end moved ---

#include "menu.h"

// Deklarationen für passive UI-Funktionen
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

// Neue Präsentationsfunktionen für spezifische Nachrichten
void presentation_welcome_add_user(void);
void presentation_error_full_name_empty(void);
void presentation_error_full_name_format(void);
void presentation_error_gamertag_empty(void);
void presentation_error_ssn(void);
void presentation_error_email(void);
void presentation_choose_subscription_start(void);
void presentation_choose_subscription_model(void);
void presentation_error_past_date(void);
void presentation_error_sub_status(void);
void presentation_error_date(void);
void presentation_error_empty_fields(void);
void presentation_error_storage(void);
void presentation_error_unknown(void);
void presentation_show_error(const char *message);
const char* presentation_get_user_gamertag(void);
void presentation_display_user_list(const char* json_data);

// --- moved to messages.h ---
// void presentation_error_gamertag_empty_edit(void);
// void presentation_error_full_name_format_edit(void);
// void presentation_error_ssn_format_edit(void);
// void presentation_error_email_format_edit(void);
// void presentation_error_date_format_edit(void);
// void presentation_error_sub_status_edit(void);
// void presentation_info_enter_full_name_edit(void);
// void presentation_info_enter_ssn_edit(void);
// void presentation_info_enter_email_edit(void);
// void presentation_info_enter_start_date_edit(void);
// void presentation_info_enter_end_date_edit(void);
// void presentation_info_enter_sub_status_edit(void);
// void presentation_success_user_edited(void);
// void presentation_error_user_not_found(void);
// void presentation_success_report_saved(void);
// void presentation_info_exiting(void);
// void presentation_info_returning_to_main_menu(void);
// void presentation_info_display_formatted_user_data(void);
// void presentation_info_delete_user_selected(void);
// void presentation_info_add_report_selected(void);
// void presentation_info_rank_top_users_selected(void);
// void presentation_info_generate_player_report_selected(void);
// void presentation_error_critical_startup(void);
// void presentation_error_invalid_option(void);
// --- end moved ---

#include "messages.h"

// Vorwärtsdeklarationen für Funktionen, die in menu.c implementiert sind
void presentation_print_welcome_banner(void);
void presentation_display_main_menu(void);
int presentation_get_main_menu_choice(void);

#endif // PRESENTATION_H
