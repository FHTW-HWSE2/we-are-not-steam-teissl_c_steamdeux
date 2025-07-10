#include "../../inc/presentation/messages.h"
#include <stdio.h>
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_GREEN   "\x1b[32;1m"
#define ANSI_COLOR_RED     "\x1b[31;1m"

void presentation_error_gamertag_empty_edit(void) {
    printf(ANSI_COLOR_RED "Gamertag must not be empty. Please try again.\n" ANSI_COLOR_RESET);
}
void presentation_error_full_name_format_edit(void) {
    printf(ANSI_COLOR_RED "Full name must only contain letters, spaces, or hyphens. Please try again or leave empty.\n" ANSI_COLOR_RESET);
}
void presentation_error_ssn_format_edit(void) {
    printf(ANSI_COLOR_RED "SSN must be in format XXXX-XXXXXX (e.g., 1234-567890). Please try again or leave empty.\n" ANSI_COLOR_RESET);
}
void presentation_error_email_format_edit(void) {
    printf(ANSI_COLOR_RED "The email format is invalid. Please try again or leave empty.\n" ANSI_COLOR_RESET);
}
void presentation_error_date_format_edit(void) {
    printf(ANSI_COLOR_RED "Invalid date format. Please use DD.MM.YYYY or leave empty.\n" ANSI_COLOR_RESET);
}
void presentation_error_sub_status_edit(void) {
    printf(ANSI_COLOR_RED "Please enter 1 (yes), 0 (no), or leave empty to keep current.\n" ANSI_COLOR_RESET);
}
void presentation_info_enter_full_name_edit(void) {
    printf("Enter new full name (or leave empty to keep current):\n");
}
void presentation_info_enter_ssn_edit(void) {
    printf("Enter new SSN (or leave empty to keep current):\n");
}
void presentation_info_enter_email_edit(void) {
    printf("Enter new email (or leave empty to keep current):\n");
}
void presentation_info_enter_start_date_edit(void) {
    printf("Enter new subscription start date (DD.MM.YYYY) (or leave empty to keep current):\n");
}
void presentation_info_enter_end_date_edit(void) {
    printf("Enter new subscription end date (DD.MM.YYYY) (or leave empty to keep current):\n");
}
void presentation_info_enter_sub_status_edit(void) {
    printf("Is subscribed? (1 = yes, 0 = no, or leave empty to keep current):\n");
}
void presentation_success_user_edited(void) {
    printf(ANSI_COLOR_GREEN "User edited successfully.\n" ANSI_COLOR_RESET);
}
void presentation_error_user_not_found(void) {
    printf(ANSI_COLOR_RED "User not found.\n" ANSI_COLOR_RESET);
}
void presentation_success_report_saved(void) {
    printf(ANSI_COLOR_GREEN "Report saved successfully.\n" ANSI_COLOR_RESET);
}
void presentation_info_exiting(void) {
    printf("Exiting...\n");
}
void presentation_info_returning_to_main_menu(void) {
    printf("Returning to main menu...\n");
}
void presentation_info_display_formatted_user_data(void) {
    printf("Display formatted user data selected.\n"); // is now implemented
}
void presentation_info_delete_user_selected(void) {
    printf("Delete user selected.\n");
}
void presentation_info_add_report_selected(void) {
    printf("Add report selected.\n");
}
void presentation_info_rank_top_users_selected(void) {
    printf("Rank Top 10 Users selected.\n");
}
void presentation_info_generate_player_report_selected(void) {
    printf("Generate player report selected.\n");
}
void presentation_error_critical_startup(void) {
    printf(ANSI_COLOR_RED "Critical error during startup tasks.\n" ANSI_COLOR_RESET);
}
void presentation_error_invalid_option(void) {
    printf(ANSI_COLOR_RED "Invalid option. Please try again.\n" ANSI_COLOR_RESET);
}
void presentation_welcome_add_user(void) {
    printf("Please add a new user.\n");
}
void presentation_error_full_name_empty(void) {
    printf(ANSI_COLOR_RED "Full name must not be empty.\n" ANSI_COLOR_RESET);
}
void presentation_error_full_name_format(void) {
    printf(ANSI_COLOR_RED "Full name must only contain letters, spaces, or hyphens.\n" ANSI_COLOR_RESET);
}
void presentation_error_gamertag_empty(void) {
    printf(ANSI_COLOR_RED "Gamertag must not be empty.\n" ANSI_COLOR_RESET);
}
void presentation_error_ssn(void) {
    printf(ANSI_COLOR_RED "SSN must be in format XXXX-XXXXXX (e.g., 1234-567890).\n" ANSI_COLOR_RESET);
}
void presentation_error_email(void) {
    printf(ANSI_COLOR_RED "The email format is invalid.\n" ANSI_COLOR_RESET);
}
void presentation_choose_subscription_start(void) {
    printf("Choose subscription start date:\n1. Use today's date\n0. Enter a future date\n");
}
void presentation_choose_subscription_model(void) {
    printf("Choose subscription model:\n1. 1 month\n2. 6 months\n3. 12 months\n");
}
void presentation_error_past_date(void) {
    printf(ANSI_COLOR_RED "The start date cannot be in the past.\n" ANSI_COLOR_RESET);
}
void presentation_error_sub_status(void) {
    printf(ANSI_COLOR_RED "Subscription status must be 'true' or 'false'.\n" ANSI_COLOR_RESET);
}
void presentation_error_date(void) {
    printf(ANSI_COLOR_RED "Invalid date format. Please use DD.MM.YYYY.\n" ANSI_COLOR_RESET);
}
void presentation_error_empty_fields(void) {
    printf(ANSI_COLOR_RED "All fields must be non-empty.\n" ANSI_COLOR_RESET);
}
void presentation_error_storage(void) {
    printf(ANSI_COLOR_RED "Could not save data.\n" ANSI_COLOR_RESET);
}
void presentation_error_unknown(void) {
    printf(ANSI_COLOR_RED "An unknown error occurred.\n" ANSI_COLOR_RESET);
}
void presentation_success_user_added(void) {
    printf(ANSI_COLOR_GREEN "User added successfully!\n" ANSI_COLOR_RESET);
}
