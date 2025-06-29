#ifndef DATA_H
#define DATA_H

#define USERS_JSON_PATH "users.json"
#define REPORTS_JSON_PATH "reports.json"
#define GAMES_JSON_PATH "games.json"

#include "cJSON.h"

int data_save_report(cJSON *report);
cJSON *data_load_reports(void);
int save_player_profile(const char* full_name, const char* gamertag, int player_hours, const char* ssn, const char* email, const char* sub_start, const char* sub_end, int is_subscribed);
int read_player_profiles(char** output);
void print_user_to_cli(void);
int remove_player_profile(const char* gamertag);
int edit_player_profile(const char* gamertag, const char* new_full_name, const char* new_ssn, const char* new_email, const char* sub_start, const char* sub_end, int is_subscribed);
void remove_expired_users(void);


#endif // DATA_H
