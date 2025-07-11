#define _GNU_SOURCE // for strdup
#include <string.h>

// Minimal stubs for cJSON, data, and presentation functions for logic.c unit tests
#include <stdio.h>
#include <stdlib.h>
#include "../src/data/game.h"

typedef struct cJSON { int valueint; char *valuestring; } cJSON;
cJSON *cJSON_CreateObject(void) { return malloc(sizeof(cJSON)); }
int cJSON_AddStringToObject(cJSON *obj, const char *key, const char *value) { return 1; }
void cJSON_Delete(cJSON *obj) { free(obj); }
cJSON *cJSON_CreateArray(void) { return malloc(sizeof(cJSON)); }
int cJSON_IsArray(const cJSON *obj) { return 1; }
int cJSON_GetArraySize(const cJSON *obj) { return 1; }
cJSON *cJSON_GetArrayItem(const cJSON *obj, int idx) { return (cJSON*)obj; }
cJSON *cJSON_Duplicate(const cJSON *obj, int recurse) { return malloc(sizeof(cJSON)); }
int cJSON_AddItemToArray(cJSON *array, cJSON *item) { return 1; }
cJSON *cJSON_GetObjectItem(const cJSON *obj, const char *key) { static cJSON dummy; dummy.valueint = 1; dummy.valuestring = "dummy"; return &dummy; }
char *cJSON_Print(const cJSON *obj) { return strdup("{\"dummy\":1}"); }

int data_save_player_profile(const char *a, const char *b, int c, const char *d, const char *e, const char *f, const char *g, int h) { return 0; }
int data_load_games(const char *filename, Game **games, int *game_count) { return 0; }
int data_get_all_users(cJSON **users_out) { *users_out = cJSON_CreateArray(); return 0; }
int data_remove_player_profile(const char *gamertag) { return 0; }
int data_save_report(cJSON *report) { return 0; }
int data_remove_expired_users(int *removed_count) { *removed_count = 0; return 0; }
int data_update_all_subscription_flags(void) { return 0; }
int data_edit_player_profile(const char *a, const char *b, const char *c, const char *d, const char *e, const char *f, int g) { return 0; }
int data_save_games(const char *filename, Game *games, int game_count) { return 0; }

const char* presentation_get_report_title(void) { return "dummy"; }
const char* presentation_get_report_description(void) { return "dummy"; }
const char* presentation_get_report_date(void) { return "01.01.2025"; }
void presentation_error_empty_fields(void) {}
void presentation_error_date(void) {}
void presentation_error_unknown(void) {}
void presentation_error_storage(void) {}
void presentation_success_report_saved(void) {}
void presentation_print_welcome_banner(void) {}
void presentation_show_startup_info(int a, int b) {}
void presentation_error_critical_startup(void) {}
void presentation_display_main_menu(void) {}
int presentation_get_main_menu_choice(void) { return 0; }
void presentation_info_exiting(void) {}
void presentation_error_invalid_option(void) {}
void presentation_display_user_menu(void) {}
int presentation_get_user_menu_choice(void) { return 0; }
void presentation_info_returning_to_main_menu(void) {}
void presentation_remove_user(void) {}
void presentation_info_add_report_selected(void) {}
void presentation_show_top_users_terminal(void) {}
void presentation_info_rank_top_users_selected(void) {}
void presentation_generate_top_users_file(void) {}
void presentation_info_generate_player_report_selected(void) {}
void presentation_welcome_add_user(void) {}
void presentation_get_full_name(char *buf, size_t sz) { snprintf(buf, sz, "dummy"); }
void presentation_error_full_name_empty(void) {}
void presentation_error_full_name_format(void) {}
void presentation_get_gamertag(char *buf, size_t sz) { snprintf(buf, sz, "dummy"); }
void presentation_error_gamertag_empty(void) {}
void presentation_get_ssn(char *buf, size_t sz) { snprintf(buf, sz, "1234-567890"); }
void presentation_error_ssn(void) {}
void presentation_get_email(char *buf, size_t sz) { snprintf(buf, sz, "dummy@dummy.com"); }
void presentation_error_email(void) {}
void presentation_choose_subscription_start(void) {}
void presentation_get_subscription_start(char *buf, size_t sz) { snprintf(buf, sz, "01.01.2025"); }
void presentation_choose_subscription_model(void) {}
void presentation_get_subscription_duration(char *buf, size_t sz) { snprintf(buf, sz, "1"); }
void presentation_success_user_added(void) {}
void presentation_error_past_date(void) {}
void presentation_error_sub_status(void) {}
void presentation_show_error(const char *msg) {}
void presentation_display_users_formatted(char **lines, int count) {}
void presentation_display_error(const char *msg) {}
void presentation_display_game_management_menu(void) {}
int presentation_get_game_menu_choice(void) { return 0; }
void presentation_get_game_title(char *buf, size_t sz) { snprintf(buf, sz, "dummy"); }
void presentation_get_game_description(char *buf, size_t sz) { snprintf(buf, sz, "dummy"); }
void presentation_get_game_version(char *buf, size_t sz) { snprintf(buf, sz, "1.0"); }
void presentation_get_game_mode(char *buf, size_t sz) { snprintf(buf, sz, "dummy"); }
void presentation_show_message(const char *msg) {}
void presentation_get_game_id_to_edit(char *buf, size_t sz) { snprintf(buf, sz, "1"); }
void presentation_get_new_game_title(char *buf, size_t sz) { snprintf(buf, sz, "dummy"); }
void presentation_get_game_id_to_delete(char *buf, size_t sz) { snprintf(buf, sz, "1"); }
void presentation_error_full_name_format_edit(void) {}
void presentation_error_gamertag_empty_edit(void) {}
void presentation_info_enter_full_name_edit(void) {}
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
void presentation_start_game_management_menu(void) {}
void start_simulation(void) {}
