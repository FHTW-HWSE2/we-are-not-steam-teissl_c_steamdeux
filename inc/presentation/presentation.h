#ifndef PRESENTATION_H
#define PRESENTATION_H
#define MAX_USER_INPUT 100

#include "../src/data/game.h"

// Core presentation functions
void presentation_collect_and_save_report(void);
void start_admin_menu();
void add_user_presentation();
void display_users_presentation();
void remove_user_presentation();
void edit_user_presentation();
void start_game_management_menu(); // neue Funktionsdeklaration
void start_main_menu();

// Consolidated functions from other presentation files
void display_games(const Game games[], int game_count);
void display_error(const char *message);
int run(void);
void start_menu(void);

#endif // PRESENTATION_H
