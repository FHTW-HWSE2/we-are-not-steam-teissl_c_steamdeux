#ifndef MOCK_PRESENTATION_H
#define MOCK_PRESENTATION_H

#include <stddef.h>  // For size_t

// Mock tracking variables for presentation_show_error
extern int mock_presentation_show_error_called;
extern char* mock_presentation_show_error_message;

// Mock tracking variables for presentation_display_error
extern int mock_presentation_display_error_called;
extern char* mock_presentation_display_error_arg;

// Mock tracking variables for presentation_display_users_formatted
extern int mock_presentation_display_users_formatted_called;
extern char** mock_presentation_display_users_formatted_lines;
extern int mock_presentation_display_users_formatted_count;

// Additional tracking for logic_start_application
extern int mock_presentation_print_welcome_banner_called;
extern int mock_presentation_show_startup_info_called;
extern int mock_presentation_show_startup_info_removed;
extern int mock_presentation_show_startup_info_changed;
extern int mock_presentation_error_critical_startup_called;
extern int mock_presentation_display_main_menu_called;
extern int mock_presentation_info_exiting_called;
extern int mock_presentation_error_invalid_option_called;
extern int mock_presentation_display_user_menu_called;
extern int mock_presentation_info_returning_to_main_menu_called;
extern int mock_presentation_start_game_management_menu_called;
extern int mock_start_simulation_called;

// Choice sequences
extern int mock_main_menu_choice_count;
extern int mock_main_menu_choice_index;
extern int mock_main_menu_choices[100];

extern int mock_user_menu_choice_count;
extern int mock_user_menu_choice_index;
extern int mock_user_menu_choices[100];

// Additional tracking for user menu sub-options
extern int mock_presentation_remove_user_called;
extern int mock_presentation_info_add_report_selected_called;
extern int mock_presentation_info_rank_top_users_selected_called;
extern int mock_presentation_show_top_users_terminal_called;
extern int mock_presentation_info_generate_player_report_selected_called;
extern int mock_presentation_generate_top_users_file_called;

// Function prototypes (subset matching those used in the test; add more if needed for other tests)
void presentation_show_error(const char *message);
void presentation_display_users_formatted(char **lines, int count);

// Add prototypes for other presentation functions if needed for completeness/linkage
// (matching the stubs in mock_presentation.c)
void presentation_show_startup_info(int removed_count, int changed_flags);
void presentation_remove_user(void);
void presentation_start_game_management_menu(void);
void presentation_show_top_users_terminal(void);
void presentation_generate_top_users_file(void);
void presentation_show_message(const char *msg);
void presentation_display_error(const char *message);
void presentation_display_user_menu(void);
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
void presentation_print_welcome_banner(void);
void presentation_display_main_menu(void);
void presentation_display_game_management_menu(void);
void presentation_success_report_saved(void);
void presentation_info_exiting(void);
void presentation_info_returning_to_main_menu(void);
void presentation_info_add_report_selected(void);
void presentation_info_rank_top_users_selected(void);
void presentation_info_generate_player_report_selected(void);
void presentation_error_critical_startup(void);
void presentation_error_invalid_option(void);
void presentation_success_user_added(void);
void presentation_error_gamertag_empty_edit(void);
void presentation_error_full_name_format_edit(void);
void presentation_error_ssn_format_edit(void);
void presentation_error_email_format_edit(void);
void presentation_error_date_format_edit(void);
void presentation_error_sub_status_edit(void);
void presentation_info_enter_full_name_edit(void);
void presentation_info_enter_ssn_edit(void);
void presentation_info_enter_email_edit(void);
void presentation_info_enter_start_date_edit(void);
void presentation_info_enter_end_date_edit(void);
void presentation_info_enter_sub_status_edit(void);
void presentation_success_user_edited(void);
void presentation_error_user_not_found(void);

void presentation_get_full_name(char *buffer, size_t size);
void presentation_get_gamertag(char *buffer, size_t size);
void presentation_get_ssn(char *buffer, size_t size);
void presentation_get_email(char *buffer, size_t size);
void presentation_get_subscription_start(char *buffer, size_t size);
void presentation_get_subscription_duration(char *buffer, size_t size);
void presentation_get_game_title(char *buffer, size_t size);
void presentation_get_game_description(char *buffer, size_t size);
void presentation_get_game_version(char *buffer, size_t size);
void presentation_get_game_mode(char *buffer, size_t size);
void presentation_get_game_id_to_edit(char *buffer, size_t size);
void presentation_get_new_game_title(char *buffer, size_t size);
void presentation_get_game_id_to_delete(char *buffer, size_t size);

// Mock variables for game input
extern char mock_game_title[100];
extern char mock_game_description[256];
extern char mock_game_version[20];
extern char mock_game_mode[50];
extern int mock_presentation_show_message_called;
extern char* mock_presentation_show_message_arg;

// Game menu mock variables
extern int mock_game_menu_choice_count;
extern int mock_game_menu_choice_index;
extern int mock_game_menu_choices[100];
extern int mock_presentation_display_game_management_menu_called;

int presentation_get_user_menu_choice(void);
int presentation_get_main_menu_choice(void);
int presentation_get_game_menu_choice(void);
const char* presentation_get_report_title(void);
const char* presentation_get_report_description(void);
const char* presentation_get_report_date(void);

#endif // MOCK_PRESENTATION_H