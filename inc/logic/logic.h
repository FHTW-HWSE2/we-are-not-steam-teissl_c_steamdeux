#ifndef LOGIC_H
#define LOGIC_H

#include "cJSON.h"
#include "../src/data/game.h"

cJSON *logic_create_report(const char *title, const char *description, const char *date);
int validate_player_profile(const char* full_name, const char* gamertag, const char* ssn, const char* email, const char* sub_start, const char* sub_end, const char* is_subscribed_str);
int display_users_logic();
void print_users_logic();
int logic_remove_user(const char* gamertag); // replaces remove_user_logic
int edit_user_logic(const char* gamertag, const char* new_full_name, const char* new_ssn, const char* new_email, const char* sub_start, const char* sub_end, const char* is_subscribed_str);
int logic_is_only_spaces(const char *str);
int logic_is_valid_date_format(const char *date);
int logic_is_valid_alpha(const char *str);
int logic_is_valid_email(const char *str);
int logic_is_valid_ssn(const char *str);
/**
 * Liefert ein Array der Top-N User sortiert nach Spielzeit (player_hours).
 * Gibt ein cJSON-Array (muss vom Aufrufer gelöscht werden) zurück, oder NULL bei Fehler.
 */
cJSON *logic_get_top_users(int n);
// Neue Schnittstelle für Präsentationsschicht
int logic_get_all_users(cJSON **users_out);
int logic_validate_player_profile(const char* full_name, const char* gamertag, const char* ssn, const char* email, const char* sub_start, const char* sub_end, const char* is_subscribed_str);
int logic_create_user(const char* full_name, const char* gamertag, const char* ssn, const char* email, const char* sub_start, const char* sub_end, const char* is_subscribed_str);
int logic_create_user_with_duration(const char* full_name, const char* gamertag, const char* ssn, const char* email, const char* sub_start, const char* is_subscribed_str, int duration_months, int use_today_as_start);
int logic_generate_top_users_file(void);
int logic_update_all_subscription_flags(void);
int logic_edit_user(const char* gamertag, const char* new_full_name, const char* new_ssn, const char* new_email, const char* sub_start, const char* sub_end, const char* is_subscribed_str);
int logic_initialize_game_data_loading(const char *filename, Game **games, int *game_count);
int logic_add_new_game(Game **games, int *game_count, const char *title, const char *description, const char *version, const char *mode);
int logic_edit_game(Game games[], int game_count, int game_id, const char *new_title);
int logic_delete_game(Game games[], int *game_count, int game_id);
void logic_create_and_save_report(void);

#endif // LOGIC_H
