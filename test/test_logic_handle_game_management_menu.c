#include "unity.h"
#include "../../inc/logic/logic.h"
#include "../../src/data/game.h"
#include "../error.h"
#include "mock_presentation.h"
#include "mock_data.h"
#include <stdlib.h>
#include <string.h>

// We will be testing the actual logic_handle_game_management_menu function,
// so we include the .c file here. The sub-functions it calls will be mocked.
// #define TESTING_GAME_MENU
// #include "../../src/logic/logic.c"


void setUp(void) {
    // Reset mock variables
    mock_game_menu_choice_count = 0;
    mock_game_menu_choice_index = 0;
    mock_presentation_display_game_management_menu_called = 0;
    mock_presentation_display_error_called = 0;
    mock_logic_display_games_formatted_called = 0;
    mock_logic_handle_add_game_called = 0;
    mock_logic_handle_edit_game_called = 0;
    mock_logic_handle_delete_game_called = 0;
    
    // Reset data mocks
    mock_data_load_games_called = 0;
    mock_data_load_games_return = ERR_SUCCESS;
    mock_data_load_games_games_out = NULL;
    mock_data_load_games_count_out = 0;
}

void tearDown(void) {
    // No specific cleanup needed
}

// Test: Successful initialization and immediate exit
void test_logic_handle_game_management_menu_immediate_exit(void) {
    // Set up successful game loading
    mock_data_load_games_return = ERR_SUCCESS;
    mock_data_load_games_count_out = 0;
    
    // Set menu choice to exit immediately
    mock_game_menu_choice_count = 1;
    mock_game_menu_choices[0] = 0;
    
    // Call function
    logic_handle_game_management_menu();
    
    // Verify
    TEST_ASSERT_EQUAL_INT(1, mock_data_load_games_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_display_game_management_menu_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_display_error_called);
    TEST_ASSERT_EQUAL_INT(0, mock_logic_display_games_formatted_called);
}

// Test: Failed game data loading
void test_logic_handle_game_management_menu_load_failure(void) {
    // Set up failed game loading
    mock_data_load_games_return = ERR_STORAGE_FAILURE;
    
    // Call function
    logic_handle_game_management_menu();
    
    // Verify
    TEST_ASSERT_EQUAL_INT(1, mock_data_load_games_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_display_game_management_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_display_error_called);
    TEST_ASSERT_EQUAL_STRING("Failed to load games.", mock_presentation_display_error_arg);
}

// Test: Display games option (choice 1)
void test_logic_handle_game_management_menu_display_games(void) {
    // Set up successful game loading with some games
    // Allocate dynamically since the function will free it
    Game *test_games = malloc(2 * sizeof(Game));
    test_games[0].id = 1;
    strcpy(test_games[0].title, "Game 1");
    strcpy(test_games[0].description, "Desc 1");
    strcpy(test_games[0].version, "1.0");
    strcpy(test_games[0].mode, "Mode 1");
    test_games[0].current_streams = 0;
    
    test_games[1].id = 2;
    strcpy(test_games[1].title, "Game 2");
    strcpy(test_games[1].description, "Desc 2");
    strcpy(test_games[1].version, "2.0");
    strcpy(test_games[1].mode, "Mode 2");
    test_games[1].current_streams = 0;
    
    mock_data_load_games_return = ERR_SUCCESS;
    mock_data_load_games_games_out = test_games;
    mock_data_load_games_count_out = 2;
    
    // Set menu choices: display games, then exit
    mock_game_menu_choice_count = 2;
    mock_game_menu_choices[0] = 1;  // Display games
    mock_game_menu_choices[1] = 0;  // Exit
    
    // Call function
    logic_handle_game_management_menu();
    
    // Verify
    TEST_ASSERT_EQUAL_INT(1, mock_data_load_games_called);
    TEST_ASSERT_EQUAL_INT(2, mock_presentation_display_game_management_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_logic_display_games_formatted_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_display_error_called);
}

// Test: Add game option (choice 2)
void test_logic_handle_game_management_menu_add_game(void) {
    // Set up successful game loading
    mock_data_load_games_return = ERR_SUCCESS;
    mock_data_load_games_count_out = 0;
    
    // Set menu choices: add game, then exit
    mock_game_menu_choice_count = 2;
    mock_game_menu_choices[0] = 2;  // Add game
    mock_game_menu_choices[1] = 0;  // Exit
    
    // Call function
    logic_handle_game_management_menu();
    
    // Verify
    TEST_ASSERT_EQUAL_INT(1, mock_data_load_games_called);
    TEST_ASSERT_EQUAL_INT(2, mock_presentation_display_game_management_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_logic_handle_add_game_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_display_error_called);
}

// Test: Edit game option (choice 3)
void test_logic_handle_game_management_menu_edit_game(void) {
    // Set up successful game loading
    mock_data_load_games_return = ERR_SUCCESS;
    mock_data_load_games_count_out = 1;
    
    // Set menu choices: edit game, then exit
    mock_game_menu_choice_count = 2;
    mock_game_menu_choices[0] = 3;  // Edit game
    mock_game_menu_choices[1] = 0;  // Exit
    
    // Call function
    logic_handle_game_management_menu();
    
    // Verify
    TEST_ASSERT_EQUAL_INT(1, mock_data_load_games_called);
    TEST_ASSERT_EQUAL_INT(2, mock_presentation_display_game_management_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_logic_handle_edit_game_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_display_error_called);
}

// Test: Delete game option (choice 4)
void test_logic_handle_game_management_menu_delete_game(void) {
    // Set up successful game loading
    mock_data_load_games_return = ERR_SUCCESS;
    mock_data_load_games_count_out = 1;
    
    // Set menu choices: delete game, then exit
    mock_game_menu_choice_count = 2;
    mock_game_menu_choices[0] = 4;  // Delete game
    mock_game_menu_choices[1] = 0;  // Exit
    
    // Call function
    logic_handle_game_management_menu();
    
    // Verify
    TEST_ASSERT_EQUAL_INT(1, mock_data_load_games_called);
    TEST_ASSERT_EQUAL_INT(2, mock_presentation_display_game_management_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_logic_handle_delete_game_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_display_error_called);
}

// Test: Invalid option handling
void test_logic_handle_game_management_menu_invalid_option(void) {
    // Set up successful game loading
    mock_data_load_games_return = ERR_SUCCESS;
    mock_data_load_games_count_out = 0;
    
    // Set menu choices: invalid option, then exit
    mock_game_menu_choice_count = 2;
    mock_game_menu_choices[0] = 99;  // Invalid
    mock_game_menu_choices[1] = 0;   // Exit
    
    // Call function
    logic_handle_game_management_menu();
    
    // Verify
    TEST_ASSERT_EQUAL_INT(1, mock_data_load_games_called);
    TEST_ASSERT_EQUAL_INT(2, mock_presentation_display_game_management_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_display_error_called);
    TEST_ASSERT_EQUAL_STRING("Invalid option.", mock_presentation_display_error_arg);
}

// Test: Multiple menu iterations
void test_logic_handle_game_management_menu_multiple_operations(void) {
    // Set up successful game loading
    mock_data_load_games_return = ERR_SUCCESS;
    mock_data_load_games_count_out = 0;
    
    // Set menu choices: multiple operations then exit
    mock_game_menu_choice_count = 5;
    mock_game_menu_choices[0] = 1;  // Display games
    mock_game_menu_choices[1] = 2;  // Add game
    mock_game_menu_choices[2] = 1;  // Display games again
    mock_game_menu_choices[3] = 4;  // Delete game
    mock_game_menu_choices[4] = 0;  // Exit
    
    // Call function
    logic_handle_game_management_menu();
    
    // Verify
    TEST_ASSERT_EQUAL_INT(1, mock_data_load_games_called);
    TEST_ASSERT_EQUAL_INT(5, mock_presentation_display_game_management_menu_called);
    TEST_ASSERT_EQUAL_INT(2, mock_logic_display_games_formatted_called);
    TEST_ASSERT_EQUAL_INT(1, mock_logic_handle_add_game_called);
    TEST_ASSERT_EQUAL_INT(1, mock_logic_handle_delete_game_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_display_error_called);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_handle_game_management_menu_immediate_exit);
    RUN_TEST(test_logic_handle_game_management_menu_load_failure);
    RUN_TEST(test_logic_handle_game_management_menu_display_games);
    RUN_TEST(test_logic_handle_game_management_menu_add_game);
    RUN_TEST(test_logic_handle_game_management_menu_edit_game);
    RUN_TEST(test_logic_handle_game_management_menu_delete_game);
    RUN_TEST(test_logic_handle_game_management_menu_invalid_option);
    RUN_TEST(test_logic_handle_game_management_menu_multiple_operations);
    return UNITY_END();
}