#include <stdio.h>
#include <string.h>
#include "../src/data/game.h"
#include "display_games.h"
#include "display_error.h"
#include <stdlib.h>  // Speicherfunktionen (malloc, realloc, free)
#include "../inc/data/data.h"
#include "../inc/logic/logic.h"
#include "../simulation/simulation.h"


// Menüfunktion
void start_menu() {
    Game *games = NULL;
    int game_count = 0;
    int choice;

    if (logic_initialize_game_data_loading(GAMES_JSON_PATH, &games, &game_count) != 0) {
        display_error("Failed to load games.");
        return;
    }

    do {
        printf("\n=== Game Management Menu ===\n");
        printf("1. Display all games\n");
        printf("2. Add new game\n");
        printf("3. Edit a game\n");
        printf("4. Delete a game\n");
        printf("0. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar(); // Eingabe-Puffer leeren

        switch (choice) {
            case 1:
                display_games(games, game_count);
                break;

            case 2: {
                char title[100], description[256], version[10], mode[20];
                printf("Enter title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;
                printf("Enter description: ");
                fgets(description, sizeof(description), stdin);
                description[strcspn(description, "\n")] = 0;
                printf("Enter version: ");
                fgets(version, sizeof(version), stdin);
                version[strcspn(version, "\n")] = 0;
                printf("Enter mode: ");
                fgets(mode, sizeof(mode), stdin);
                mode[strcspn(mode, "\n")] = 0;
                if (logic_add_new_game(&games, &game_count, title, description, version, mode) == 0) {
                    data_save_games(GAMES_JSON_PATH, games, game_count);
                    printf("New game added.\n");
                } else {
                    display_error("Failed to add game.");
                }
                break;
            }
            case 3: {
                int game_id;
                char new_title[100];
                printf("Enter the game ID to edit: ");
                scanf("%d", &game_id);
                getchar();
                printf("Enter the new title: ");
                fgets(new_title, sizeof(new_title), stdin);
                new_title[strcspn(new_title, "\n")] = 0;
                if (logic_edit_game(games, game_count, game_id, new_title) == 0) {
                    data_save_games(GAMES_JSON_PATH, games, game_count);
                    printf("Game edited.\n");
                } else {
                    display_error("Game not found.");
                }
                break;
            }
            case 4: {
                int game_id;
                printf("Enter the game ID to delete: ");
                scanf("%d", &game_id);
                getchar();
                if (logic_delete_game(games, &game_count, game_id) == 0) {
                    data_save_games(GAMES_JSON_PATH, games, game_count);
                    printf("Game deleted.\n");
                } else {
                    display_error("Game not found.");
                }
                break;
            }
            case 0:
                printf("Exiting...\n");
                break;
            default:
                display_error("Invalid option.");
                break;
        }
    } while (choice != 0);
    free(games);
}