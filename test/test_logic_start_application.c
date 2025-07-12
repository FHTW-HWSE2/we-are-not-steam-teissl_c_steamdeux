#include "unity.h"
#include "../../cJSON/cJSON.h"
#include "../mocks/mock_data.h"
#include "mock_presentation.h"
#include "../../inc/logic/logic.h"
#include <stdlib.h>  // For free if needed

// Note: This test requires extensions to mock_presentation.c/.h and mock_data.c/.h as described in the thinking process.
// Specifically:
// - Tracking variables and functions for all presentation calls in the function (welcome_banner, show_startup_info with params, error_critical_startup, display_main_menu, info_exiting, error_invalid_option, display_user_menu, info_returning_to_main_menu, start_game_management_menu)
// - Sequence arrays for presentation_get_main_menu_choice and presentation_get_user_menu_choice (with counts and indices)
// - Stub for start_simulation with called counter
// - In mock_data: tracking for data_remove_expired_users (called, return, removed_out) and data_update_all_subscription_flags (called, return=changed)
// - Reset all in setUp, free strings if any in tearDown

void setUp(void) {
    // Reset all mocks (assume a reset function or individual)
    mock_presentation_print_welcome_banner_called = 0;
    mock_presentation_show_startup_info_called = 0;
    mock_presentation_show_startup_info_removed = 0;
    mock_presentation_show_startup_info_changed = 0;
    mock_presentation_error_critical_startup_called = 0;
    mock_presentation_display_main_menu_called = 0;
    mock_presentation_info_exiting_called = 0;
    mock_presentation_error_invalid_option_called = 0;
    mock_presentation_display_user_menu_called = 0;
    mock_presentation_info_returning_to_main_menu_called = 0;
    mock_presentation_start_game_management_menu_called = 0;
    mock_start_simulation_called = 0;

    mock_data_remove_expired_users_called = 0;
    mock_data_remove_expired_users_return = ERR_SUCCESS;
    mock_data_remove_expired_users_removed_out = 0;
    mock_data_update_all_subscription_flags_called = 0;
    mock_data_update_all_subscription_flags_return = 0;

    // Reset choice sequences
    mock_main_menu_choice_count = 0;
    mock_main_menu_choice_index = 0;
    mock_user_menu_choice_count = 0;
    mock_user_menu_choice_index = 0;
}

void tearDown(void) {
    // No specific frees needed if no allocations in mocks
}

// Test: Startup success, choice 0 -> exits after one menu display
void test_logic_start_application_startup_success_choice_0_exits(void) {
    mock_data_remove_expired_users_removed_out = 5;
    mock_data_update_all_subscription_flags_return = 3;

    mock_main_menu_choice_count = 1;
    mock_main_menu_choices[0] = 0;

    logic_start_application();

    TEST_ASSERT_EQUAL_INT(1, mock_presentation_print_welcome_banner_called);
    TEST_ASSERT_EQUAL_INT(1, mock_data_remove_expired_users_called);
    TEST_ASSERT_EQUAL_INT(1, mock_data_update_all_subscription_flags_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_show_startup_info_called);
    TEST_ASSERT_EQUAL_INT(5, mock_presentation_show_startup_info_removed);
    TEST_ASSERT_EQUAL_INT(3, mock_presentation_show_startup_info_changed);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_error_critical_startup_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_display_main_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_info_exiting_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_error_invalid_option_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_display_user_menu_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_info_returning_to_main_menu_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_start_game_management_menu_called);
    TEST_ASSERT_EQUAL_INT(0, mock_start_simulation_called);
}

// Test: Startup fail on remove -> calls critical error, still runs loop with choice 0
void test_logic_start_application_startup_fail_calls_critical(void) {
    mock_data_remove_expired_users_return = ERR_STORAGE_FAILURE;

    mock_main_menu_choice_count = 1;
    mock_main_menu_choices[0] = 0;

    logic_start_application();

    TEST_ASSERT_EQUAL_INT(1, mock_presentation_print_welcome_banner_called);
    TEST_ASSERT_EQUAL_INT(1, mock_data_remove_expired_users_called);
    TEST_ASSERT_EQUAL_INT(0, mock_data_update_all_subscription_flags_called);  // Not called if remove fails
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_show_startup_info_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_error_critical_startup_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_display_main_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_info_exiting_called);
}

// Test: Invalid choice -> calls invalid option, then choice 0 exits
void test_logic_start_application_invalid_choice_calls_error(void) {
    mock_main_menu_choice_count = 2;
    mock_main_menu_choices[0] = 99;
    mock_main_menu_choices[1] = 0;

    logic_start_application();

    TEST_ASSERT_EQUAL_INT(1, mock_presentation_print_welcome_banner_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_show_startup_info_called);
    TEST_ASSERT_EQUAL_INT(2, mock_presentation_display_main_menu_called);  // Two iterations
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_error_invalid_option_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_info_exiting_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_start_game_management_menu_called);
    TEST_ASSERT_EQUAL_INT(0, mock_start_simulation_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_display_user_menu_called);
}

// Test: Choice 1 -> calls user workflow, which exits with sub-choice 0
void test_logic_start_application_choice_1_calls_user_workflow(void) {
    mock_main_menu_choice_count = 2;
    mock_main_menu_choices[0] = 1;
    mock_main_menu_choices[1] = 0;

    mock_user_menu_choice_count = 1;
    mock_user_menu_choices[0] = 0;

    logic_start_application();

    TEST_ASSERT_EQUAL_INT(1, mock_presentation_print_welcome_banner_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_show_startup_info_called);
    TEST_ASSERT_EQUAL_INT(2, mock_presentation_display_main_menu_called);  // Main loop twice
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_display_user_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_info_returning_to_main_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_info_exiting_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_error_invalid_option_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_start_game_management_menu_called);
    TEST_ASSERT_EQUAL_INT(0, mock_start_simulation_called);
}

// Test: Choice 2 -> calls game management
void test_logic_start_application_choice_2_calls_game_management(void) {
    mock_main_menu_choice_count = 2;
    mock_main_menu_choices[0] = 2;
    mock_main_menu_choices[1] = 0;

    logic_start_application();

    TEST_ASSERT_EQUAL_INT(1, mock_presentation_print_welcome_banner_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_show_startup_info_called);
    TEST_ASSERT_EQUAL_INT(2, mock_presentation_display_main_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_start_game_management_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_info_exiting_called);
    TEST_ASSERT_EQUAL_INT(0, mock_start_simulation_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_display_user_menu_called);
}

// Test: Choice 3 -> calls simulation
void test_logic_start_application_choice_3_calls_simulation(void) {
    mock_main_menu_choice_count = 2;
    mock_main_menu_choices[0] = 3;
    mock_main_menu_choices[1] = 0;

    logic_start_application();

    TEST_ASSERT_EQUAL_INT(1, mock_presentation_print_welcome_banner_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_show_startup_info_called);
    TEST_ASSERT_EQUAL_INT(2, mock_presentation_display_main_menu_called);
    TEST_ASSERT_EQUAL_INT(1, mock_start_simulation_called);
    TEST_ASSERT_EQUAL_INT(1, mock_presentation_info_exiting_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_start_game_management_menu_called);
    TEST_ASSERT_EQUAL_INT(0, mock_presentation_display_user_menu_called);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_start_application_startup_success_choice_0_exits);
    RUN_TEST(test_logic_start_application_startup_fail_calls_critical);
    RUN_TEST(test_logic_start_application_invalid_choice_calls_error);
    RUN_TEST(test_logic_start_application_choice_1_calls_user_workflow);
    RUN_TEST(test_logic_start_application_choice_2_calls_game_management);
    RUN_TEST(test_logic_start_application_choice_3_calls_simulation);
    return UNITY_END();
}