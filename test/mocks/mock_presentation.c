// test/mocks/mock_presentation.c

#include "../../inc/presentation/presentation.h"
#include <string.h>
#include <stdlib.h>
#include "../../src/simulation/simulation.h"  // Für start_simulation()

// === Tracking für show_error ===
int mock_presentation_show_error_called = 0;
char* mock_presentation_show_error_message = NULL;

// === Tracking für display_users_formatted ===
int mock_presentation_display_users_formatted_called = 0;
char** mock_presentation_display_users_formatted_lines = NULL;
int mock_presentation_display_users_formatted_count = 0;

// Additional tracking for logic_start_application tests
int mock_presentation_print_welcome_banner_called = 0;
int mock_presentation_show_startup_info_called = 0;
int mock_presentation_show_startup_info_removed = 0;
int mock_presentation_show_startup_info_changed = 0;
int mock_presentation_error_critical_startup_called = 0;
int mock_presentation_display_main_menu_called = 0;
int mock_presentation_info_exiting_called = 0;
int mock_presentation_error_invalid_option_called = 0;
int mock_presentation_display_user_menu_called = 0;
int mock_presentation_info_returning_to_main_menu_called = 0;
int mock_presentation_start_game_management_menu_called = 0;
int mock_start_simulation_called = 0;

// Choice sequences
int mock_main_menu_choice_count = 0;
int mock_main_menu_choice_index = 0;
int mock_main_menu_choices[100] = {0};

int mock_user_menu_choice_count = 0;
int mock_user_menu_choice_index = 0;
int mock_user_menu_choices[100] = {0};

// === Mock: Fehleranzeige ===
void presentation_show_error(const char *message) {
    mock_presentation_show_error_called++;
    if (mock_presentation_show_error_message) {
        free(mock_presentation_show_error_message);
    }
    mock_presentation_show_error_message = strdup(message ? message : "");
}

// === Mock: Benutzeranzeige (Zeilen) ===
void presentation_display_users_formatted(char **lines, int count) {
    mock_presentation_display_users_formatted_called++;
    if (mock_presentation_display_users_formatted_lines) {
        for (int i = 0; i < mock_presentation_display_users_formatted_count; ++i) {
            free(mock_presentation_display_users_formatted_lines[i]);
        }
        free(mock_presentation_display_users_formatted_lines);
    }
    mock_presentation_display_users_formatted_count = count;
    if (count > 0 && lines) {
        mock_presentation_display_users_formatted_lines = malloc(sizeof(char*) * count);
        for (int i = 0; i < count; ++i) {
            mock_presentation_display_users_formatted_lines[i] = strdup(lines[i] ? lines[i] : "");
        }
    } else {
        mock_presentation_display_users_formatted_lines = NULL;
    }
}

// Mocked functions with tracking
void presentation_print_welcome_banner(void) {
    mock_presentation_print_welcome_banner_called++;
}

void presentation_show_startup_info(int removed, int changed) {
    mock_presentation_show_startup_info_called++;
    mock_presentation_show_startup_info_removed = removed;
    mock_presentation_show_startup_info_changed = changed;
}

void presentation_error_critical_startup(void) {
    mock_presentation_error_critical_startup_called++;
}

void presentation_display_main_menu(void) {
    mock_presentation_display_main_menu_called++;
}

void presentation_info_exiting(void) {
    mock_presentation_info_exiting_called++;
}

void presentation_error_invalid_option(void) {
    mock_presentation_error_invalid_option_called++;
}

void presentation_display_user_menu(void) {
    mock_presentation_display_user_menu_called++;
}

void presentation_info_returning_to_main_menu(void) {
    mock_presentation_info_returning_to_main_menu_called++;
}

void presentation_start_game_management_menu(void) {
    mock_presentation_start_game_management_menu_called++;
}

int presentation_get_main_menu_choice(void) {
    if (mock_main_menu_choice_index < mock_main_menu_choice_count) {
        return mock_main_menu_choices[mock_main_menu_choice_index++];
    }
    return 0;
}

int presentation_get_user_menu_choice(void) {
    if (mock_user_menu_choice_index < mock_user_menu_choice_count) {
        return mock_user_menu_choices[mock_user_menu_choice_index++];
    }
    return 0;
}

void start_simulation(void) {
    mock_start_simulation_called++;
}

// === Stub-Funktionen (do nothing oder Rückgabe 0/default) ===
// (Keep the rest as is, or add tracking if needed for other tests)

void presentation_remove_user(void) {}
void presentation_info_add_report_selected(void) {}
void presentation_info_rank_top_users_selected(void) {}
void presentation_show_top_users_terminal(void) {}
void presentation_info_generate_player_report_selected(void) {}
void presentation_generate_top_users_file(void) {}
void presentation_error_full_name_empty(void) {}
void presentation_error_full_name_format(void) {}
void presentation_get_full_name(char *a, size_t b) { if (a) memset(a, 0, b); }
void presentation_get_gamertag(char *a, size_t b) { if (a) memset(a, 0, b); }
void presentation_error_gamertag_empty(void) {}
void presentation_get_ssn(char *a, size_t b) { if (a) memset(a, 0, b); }
void presentation_error_ssn(void) {}
void presentation_get_email(char *a, size_t b) { if (a) memset(a, 0, b); }
void presentation_error_email(void) {}
void presentation_choose_subscription_start(void) {}
void presentation_get_subscription_start(char *a, size_t b) { if (a) memset(a, 0, b); }
void presentation_choose_subscription_model(void) {}
void presentation_get_subscription_duration(char *a, size_t b) { if (a) memset(a, 0, b); }
void presentation_success_user_added(void) {}
void presentation_error_past_date(void) {}
void presentation_error_sub_status(void) {}
void presentation_error_date(void) {}
void presentation_error_empty_fields(void) {}
void presentation_error_storage(void) {}
void presentation_error_unknown(void) {}
void presentation_info_enter_full_name_edit(void) {}
void presentation_error_full_name_format_edit(void) {}
void presentation_info_enter_ssn_edit(void) {}
void presentation_error_ssn_format_edit(void) {}
void presentation_info_enter_email_edit(void) {}
void presentation_error_email_format_edit(void) {}
void presentation_info_enter_start_date_edit(void) {}
void presentation_error_date_format_edit(void) {}
void presentation_info_enter_end_date_edit(void) {}
void presentation_info_enter_sub_status_edit(void) {}
void presentation_error_sub_status_edit(void) {}
void presentation_success_user_edited(void) {}
void presentation_error_user_not_found(void) {}
void presentation_display_error(const char *msg) {}

void presentation_get_game_title(char *a, size_t b) { if (a) memset(a, 0, b); }
void presentation_get_game_description(char *a, size_t b) { if (a) memset(a, 0, b); }
void presentation_get_game_version(char *a, size_t b) { if (a) memset(a, 0, b); }
void presentation_get_game_mode(char *a, size_t b) { if (a) memset(a, 0, b); }
void presentation_show_message(const char *msg) { (void)msg; }
int presentation_get_game_menu_choice(void) { return 0; }
void presentation_display_game_management_menu(void) {}
void presentation_get_game_id_to_edit(char *a, size_t b) { if (a) memset(a, 0, b); }
void presentation_get_new_game_title(char *a, size_t b) { if (a) memset(a, 0, b); }
void presentation_get_game_id_to_delete(char *a, size_t b) { if (a) memset(a, 0, b); }
void presentation_success_report_saved(void) {}

const char* presentation_get_report_title(void) { return "stub_title"; }
const char* presentation_get_report_description(void) { return "stub_description"; }
const char* presentation_get_report_date(void) { return "12.07.2025"; }

void presentation_welcome_add_user(void) {}
void presentation_error_gamertag_empty_edit(void) {}