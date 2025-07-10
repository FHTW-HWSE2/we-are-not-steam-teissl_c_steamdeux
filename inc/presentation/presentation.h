#ifndef PRESENTATION_H
#define PRESENTATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "menu.h"
#include "messages.h"
#include "run.h"
#include "../logic/logic.h"
#include "../data/data.h"
#include "../error.h"
#include "../src/simulation/simulation.h"

#define MAX_USER_INPUT 100

// Core presentation functions
void presentation_remove_user();
void presentation_start_game_management_menu();
void presentation_show_top_users_terminal();
void presentation_generate_top_users_file();
void presentation_show_message(const char *msg);
const char* presentation_get_report_title(void);
const char* presentation_get_report_description(void);
const char* presentation_get_report_date(void);
void presentation_display_users_formatted(char **lines, int count); // --> added 10.07.2025

// Consolidated functions from other presentation files
void presentation_display_error(const char *message);

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

// Vorwärtsdeklarationen für Funktionen, die in menu.c implementiert sind
void presentation_print_welcome_banner(void);
void presentation_display_main_menu(void);
int presentation_get_main_menu_choice(void);

// Below function added 10.07.2025
// --- Game Management Eingabe-Funktionen (für logic_handle_*) ---
void presentation_get_game_title(char *buffer, size_t size);
void presentation_get_game_description(char *buffer, size_t size);
void presentation_get_game_version(char *buffer, size_t size);
void presentation_get_game_mode(char *buffer, size_t size);
void presentation_get_game_id_to_edit(char *buffer, size_t size);
void presentation_get_new_game_title(char *buffer, size_t size);
void presentation_get_game_id_to_delete(char *buffer, size_t size);
void presentation_display_game_management_menu(void);
int presentation_get_game_menu_choice(void);
void presentation_show_message(const char *msg);

#endif // PRESENTATION_H
