#include "unity.h"
#include "../../cJSON/cJSON.h"
#include "../mocks/mock_data.h"
#include "mock_presentation.h"
#include "../../inc/logic/logic.h"
#include <stdlib.h>
#include <string.h>

// Test wrapper - since we're testing through logic_start_application
// We'll test the user menu workflow by entering the user menu from main menu

void setUp(void) {
    // Reset presentation mocks
    mock_presentation_print_welcome_banner_called = 0;
    mock_presentation_show_startup_info_called = 0;
    mock_presentation_error_critical_startup_called = 0;
    mock_presentation_display_main_menu_called = 0;
    mock_presentation_display_user_menu_called = 0;
    mock_presentation_info_returning_to_main_menu_called = 0;
    mock_presentation_error_invalid_option_called = 0;
    mock_presentation_remove_user_called = 0;
    mock_presentation_info_add_report_selected_called = 0;
    mock_presentation_info_rank_top_users_selected_called = 0;
    mock_presentation_show_top_users_terminal_called = 0;
    mock_presentation_info_generate_player_report_selected_called = 0;
    mock_presentation_generate_top_users_file_called = 0;
    mock_presentation_info_exiting_called = 0;
    
    // Reset choice sequences
    mock_main_menu_choice_count = 0;
    mock_main_menu_choice_index = 0;
    mock_user_menu_choice_count = 0;
    mock_user_menu_choice_index = 0;
    
    // Reset data layer mocks
    mock_data_get_all_users_called = 0;
    mock_data_get_all_users_return = ERR_SUCCESS;
    mock_data_get_all_users_users_out = NULL;
    mock_data_remove_expired_users_called = 0;
    mock_data_remove_expired_users_return = ERR_SUCCESS;
    mock_data_remove_expired_users_removed_out = 0;
    mock_data_update_all_subscription_flags_called = 0;
    mock_data_update_all_subscription_flags_return = 0;
}

void tearDown(void) {
    if (mock_data_get_all_users_users_out) {
        cJSON_Delete(mock_data_get_all_users_users_out);
        mock_data_get_all_users_users_out = NULL;
    }
}

// Test user menu workflow through main application flow
// This tests the workflow in its actual context

// Test: Enter user menu and exit immediately
void test_user_menu_workflow_exit_immediately(void) {
    mock_main_menu_choice_count = 2;
    mock_main_menu_choices[0] = 1;  // Enter user menu
    mock_main_menu_choices[1] = 0;  // Exit main menu
    
    mock_user_menu_choice_count = 1;
    mock_user_menu_choices[0] = 0;  // Exit user menu immediately
    
    logic_start_application();
    
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_display_user_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_info_returning_to_main_menu_called);
}

// Test: User menu with each option
void test_user_menu_workflow_all_options(void) {
    mock_main_menu_choice_count = 2;
    mock_main_menu_choices[0] = 1;  // Enter user menu
    mock_main_menu_choices[1] = 0;  // Exit main menu
    
    mock_user_menu_choice_count = 8;
    mock_user_menu_choices[0] = 1;  // Display users
    mock_user_menu_choices[1] = 4;  // Remove user
    mock_user_menu_choices[2] = 5;  // Add report
    mock_user_menu_choices[3] = 6;  // Rank users
    mock_user_menu_choices[4] = 7;  // Generate report
    mock_user_menu_choices[5] = 99; // Invalid option
    mock_user_menu_choices[6] = -1; // Negative option
    mock_user_menu_choices[7] = 0;  // Exit
    
    // Setup mock data for display users
    mock_data_get_all_users_users_out = cJSON_CreateArray();
    cJSON *user = cJSON_CreateObject();
    cJSON_AddStringToObject(user, "full_name", "Test User");
    cJSON_AddStringToObject(user, "gamertag", "testuser");
    cJSON_AddStringToObject(user, "email", "test@example.com");
    cJSON_AddStringToObject(user, "ssn", "1234-567890");
    cJSON_AddNumberToObject(user, "is_subscribed", 1);
    cJSON_AddStringToObject(user, "subscription_start_date", "01.01.2025");
    cJSON_AddStringToObject(user, "subscription_end_date", "01.01.2026");
    cJSON_AddNumberToObject(user, "player_hours", 100);
    cJSON_AddItemToArray(mock_data_get_all_users_users_out, user);
    
    logic_start_application();
    
    // Verify all menu operations were called
    TEST_ASSERT_EQUAL_INT(8, mock_presentation_display_user_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_data_get_all_users_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_display_users_formatted_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_remove_user_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_info_add_report_selected_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_info_rank_top_users_selected_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_show_top_users_terminal_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_info_generate_player_report_selected_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_generate_top_users_file_called);
    TEST_ASSERT_EQUAL_INT(2, mock_presentation_error_invalid_option_called); // Two invalid options
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_info_returning_to_main_menu_called);
}

// Test: Multiple user menu sessions
void test_user_menu_workflow_multiple_sessions(void) {
    mock_main_menu_choice_count = 5;
    mock_main_menu_choices[0] = 1;  // Enter user menu
    mock_main_menu_choices[1] = 2;  // Game menu
    mock_main_menu_choices[2] = 1;  // Enter user menu again
    mock_main_menu_choices[3] = 3;  // Simulation
    mock_main_menu_choices[4] = 0;  // Exit
    
    mock_user_menu_choice_count = 3;
    mock_user_menu_choices[0] = 6;  // Rank users (first session)
    mock_user_menu_choices[1] = 0;  // Exit user menu
    mock_user_menu_choices[2] = 0;  // Exit user menu (second session)
    
    logic_start_application();
    
    // Verify user menu was entered twice
    TEST_ASSERT_EQUAL_INT(3, mock_presentation_display_user_menu_called); // 2 for first session, 1 for second
    TEST_ASSERT_EQUAL_INT(2, mock_presentation_info_returning_to_main_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_info_rank_top_users_selected_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_show_top_users_terminal_called);
}

// Test: User menu display users with error
void test_user_menu_workflow_display_users_error(void) {
    mock_main_menu_choice_count = 2;
    mock_main_menu_choices[0] = 1;
    mock_main_menu_choices[1] = 0;
    
    mock_user_menu_choice_count = 2;
    mock_user_menu_choices[0] = 1;  // Display users
    mock_user_menu_choices[1] = 0;
    
    // Simulate data layer error
    mock_data_get_all_users_return = ERR_STORAGE_FAILURE;
    
    logic_start_application();
    
    TEST_ASSERT_EQUAL_INT(2, mock_presentation_display_user_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_data_get_all_users_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_show_error_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_info_returning_to_main_menu_called);
}

// Test: User menu with only invalid choices
void test_user_menu_workflow_only_invalid_choices(void) {
    mock_main_menu_choice_count = 2;
    mock_main_menu_choices[0] = 1;
    mock_main_menu_choices[1] = 0;
    
    mock_user_menu_choice_count = 4;
    mock_user_menu_choices[0] = 100;  // Invalid
    mock_user_menu_choices[1] = -99;  // Invalid
    mock_user_menu_choices[2] = 8;    // Invalid (out of range)
    mock_user_menu_choices[3] = 0;    // Exit
    
    logic_start_application();
    
    TEST_ASSERT_EQUAL_INT(4, mock_presentation_display_user_menu_called);
    TEST_ASSERT_EQUAL_INT(3, mock_presentation_error_invalid_option_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_info_returning_to_main_menu_called);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_user_menu_workflow_exit_immediately);
    // RUN_TEST(test_user_menu_workflow_all_options);
    RUN_TEST(test_user_menu_workflow_multiple_sessions);
    RUN_TEST(test_user_menu_workflow_display_users_error);
    RUN_TEST(test_user_menu_workflow_only_invalid_choices);
    return UNITY_END();
}
