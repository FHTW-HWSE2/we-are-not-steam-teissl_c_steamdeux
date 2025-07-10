#ifndef DATA_H
#define DATA_H

#define USERS_JSON_PATH "users.json" // 10.07. Angepasst und in CMakelists den Pfad geändert
#define REPORTS_JSON_PATH "reports.json"  // 10.07. Angepasst und in CMakelists den Pfad geändert
#define GAMES_JSON_PATH "games.json" // 10.07. Angepasst und in CMakelists den Pfad geändert

#include "cJSON.h"
#include "../src/data/game.h"

int data_save_report(cJSON *report);
cJSON *data_load_reports(void);
int data_save_player_profile(const char* full_name, const char* gamertag, int player_hours, const char* ssn, const char* email, const char* sub_start, const char* sub_end, int is_subscribed);
int data_remove_player_profile(const char* gamertag);
int data_edit_player_profile(const char* gamertag, const char* new_full_name, const char* new_ssn, const char* new_email, const char* sub_start, const char* sub_end, int is_subscribed);
int data_get_all_users(cJSON **users_out);
int data_remove_expired_users(int *removed_count_out);
int data_update_all_subscription_flags();
int data_load_games(const char *filename, Game **games_out, int *count_out);
int data_save_games(const char *filename, Game games[], int game_count);
int data_get_user_by_gamertag(const char* gamertag, cJSON** user_out);

#endif
