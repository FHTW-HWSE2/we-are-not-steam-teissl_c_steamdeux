#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdio.h>

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
void presentation_success_report_saved(void);

// --- Startup and navigation messages ---
void presentation_info_exiting(void);
void presentation_info_returning_to_main_menu(void);
void presentation_info_add_report_selected(void);
void presentation_info_rank_top_users_selected(void);
void presentation_info_generate_player_report_selected(void);
void presentation_error_critical_startup(void);
void presentation_error_invalid_option(void);

// --- User management and report workflow ---
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
void presentation_success_user_added(void);

#endif // MESSAGES_H
