#ifndef LOGIC_H
#define LOGIC_H

#include "cJSON.h"

cJSON *logic_create_report(const char *title, const char *description, const char *date);
int validate_player_profile(const char* full_name, const char* gamertag, const char* ssn, const char* email, const char* sub_start, const char* sub_end, const char* is_subscribed_str);
int display_users_logic();
void print_users_logic();
int remove_user_logic(const char* gamertag);
int edit_user_logic(const char* gamertag, const char* new_full_name, const char* new_ssn, const char* new_email, const char* sub_start, const char* sub_end, const char* is_subscribed_str);


#endif // LOGIC_H
