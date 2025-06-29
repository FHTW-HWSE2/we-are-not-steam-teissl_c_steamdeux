#include <stdio.h>
#include <string.h>
#include "../src/data/game.h"
#include "../logic/process_games.h"
#include "../data/load_games.h"
#include "../data/save_games.h"
#include "display_games.h"
#include "display_error.h"
#include <stdlib.h>  // Speicherfunktionen (malloc, realloc, free)
#include "../inc/data/data.h"
#include "../simulation/simulation.h"


// Menüfunktion
void start_menu() {
    Game *games = NULL;
    int game_count = 0;
    int choice;

    if (initialize_game_data_loading(GAMES_JSON_PATH, &games, &game_count) != 0) {
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

            case 2:
                // Neues Spiel hinzufügen
                games = realloc(games, (game_count + 1) * sizeof(Game));
                if (games == NULL) {
                    display_error("Memory allocation failed.");
                    break;
                }

                printf("Enter title: ");
                fgets(games[game_count].title, sizeof(games[game_count].title), stdin);
                games[game_count].title[strcspn(games[game_count].title, "\n")] = 0;

                printf("Enter description: ");
                fgets(games[game_count].description, sizeof(games[game_count].description), stdin);
                games[game_count].description[strcspn(games[game_count].description, "\n")] = 0;

                printf("Enter version: ");
                fgets(games[game_count].version, sizeof(games[game_count].version), stdin);
                games[game_count].version[strcspn(games[game_count].version, "\n")] = 0;

                printf("Enter mode: ");
                fgets(games[game_count].mode, sizeof(games[game_count].mode), stdin);
                games[game_count].mode[strcspn(games[game_count].mode, "\n")] = 0;

                games[game_count].id = game_count + 1;
                games[game_count].current_streams = 0;
                game_count++;

                save_games(GAMES_JSON_PATH, games, game_count);
                printf("New game added.\n");
                break;

            case 3: {
                int game_id;
                printf("Enter the game ID to edit: ");
                scanf("%d", &game_id);
                getchar();

                char new_title[100];
                printf("Enter the new title: ");
                fgets(new_title, sizeof(new_title), stdin);
                new_title[strcspn(new_title, "\n")] = 0;

                if (edit_game(games, game_count, game_id, new_title) == 0) {
                    save_games(GAMES_JSON_PATH, games, game_count);
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

                if (delete_game(games, &game_count, game_id) == 0) {
                    save_games(GAMES_JSON_PATH, games, game_count);
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