#ifndef LOGIC_H
#define LOGIC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../../cJSON/cJSON.h"
#include "../data/data.h"
#include "../error.h"
#include "../../src/data/game.h"
#include "../../src/simulation/simulation.h"
#include "../presentation/presentation.h"

// Public API for logic layer
cJSON *logic_create_report(const char *title, const char *description, const char *date);
int logic_remove_user(const char* gamertag);
int logic_is_only_spaces(const char *str);
int logic_is_valid_date_format(const char *date);
int logic_is_valid_alpha(const char *str);
int logic_is_valid_email(const char *str);
int logic_is_valid_ssn(const char *str);
cJSON *logic_get_top_users(int n);
int logic_get_all_users(cJSON **users_out);
int logic_validate_player_profile(const char* full_name, const char* gamertag, const char* ssn, const char* email, const char* sub_start, const char* sub_end, const char* is_subscribed_str);
int logic_create_user_with_duration(const char* full_name, const char* gamertag, const char* ssn, const char* email, const char* sub_start, const char* is_subscribed_str, int duration_months, int use_today_as_start);
int logic_generate_top_users_file(void);
int logic_edit_user(const char* gamertag, const char* new_full_name, const char* new_ssn, const char* new_email, const char* sub_start, const char* sub_end, const char* is_subscribed_str);
int logic_initialize_game_data_loading(const char *filename, Game **games, int *game_count);
int logic_add_new_game(Game **games, int *game_count, const char *title, const char *description, const char *version, const char *mode);
int logic_edit_game(Game games[], int game_count, int game_id, const char *new_title);
int logic_delete_game(Game games[], int *game_count, int game_id);
void logic_create_and_save_report(void);
void logic_start_application(void);
void logic_display_users_formatted(void);
int logic_get_user_lines_for_display(char*** lines_out, int* count_out);
void logic_handle_game_management_menu(void);
void logic_handle_add_game(Game **games, int *game_count);
void logic_handle_edit_game(Game *games, int game_count);
void logic_handle_delete_game(Game **games, int *game_count);
void logic_display_games_formatted(const Game *games, int game_count);
int logic_get_game_lines_for_display(const Game *games, int game_count, char ***lines_out, int *line_count_out);
int logic_validate_required_field(const char* str);
int is_valid_ssn_format(const char* ssn);
int is_valid_email_format(const char* email);
int is_valid_date_format(const char* date);
int is_valid_alpha_format(const char *str);

#endif // LOGIC_H
