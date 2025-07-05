#ifndef PRESENTATION_H
#define PRESENTATION_H
#define MAX_USER_INPUT 100

#include "../src/data/game.h"

// Core presentation functions
void presentation_collect_and_save_report(void);
void presentation_start_admin_menu();
void presentation_add_user();
void presentation_display_users();
void presentation_remove_user();
void presentation_edit_user();
void presentation_start_game_management_menu();
void presentation_start_main_menu();
void presentation_show_top_users_terminal();
void presentation_generate_top_users_file();

// Consolidated functions from other presentation files
void presentation_display_games(const Game games[], int game_count);
void presentation_display_error(const char *message);
int presentation_run(void);
void presentation_start_menu(void);

#endif // PRESENTATION_H
