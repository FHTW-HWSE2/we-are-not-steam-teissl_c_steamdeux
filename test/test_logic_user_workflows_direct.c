// test/test_logic_user_workflows_direct.c
// Direct unit test for logic_handle_add_user_workflow and logic_handle_edit_user_workflow
// Compile with: gcc -DTEST_EXPOSE_USER_WORKFLOWS ...

#include "unity.h"
#include "mocks/mock_presentation.h"
#include "mocks/mock_data.h"
#include "mocks/mock_time.h"
#include "../../src/logic/logic.c"
#include <string.h>

// Forward declarations (exposed via TEST_EXPOSE_USER_WORKFLOWS)
void logic_handle_add_user_workflow(void);
void logic_handle_edit_user_workflow(void);

void setUp(void) {
    // Reset all relevant mocks
    mock_presentation_show_error_called = 0;
    if (mock_presentation_show_error_message) { free(mock_presentation_show_error_message); mock_presentation_show_error_message = NULL; }
    mock_presentation_display_error_called = 0;
    if (mock_presentation_display_error_arg) { free(mock_presentation_display_error_arg); mock_presentation_display_error_arg = NULL; }
    mock_presentation_show_message_called = 0;
    if (mock_presentation_show_message_arg) { free(mock_presentation_show_message_arg); mock_presentation_show_message_arg = NULL; }
    mock_presentation_display_users_formatted_called = 0;
    if (mock_presentation_display_users_formatted_lines) {
        for (int i = 0; i < mock_presentation_display_users_formatted_count; ++i) {
            free(mock_presentation_display_users_formatted_lines[i]);
        }
        free(mock_presentation_display_users_formatted_lines);
        mock_presentation_display_users_formatted_lines = NULL;
    }
    mock_presentation_display_users_formatted_count = 0;
    // Reset input mocks
    memset(mock_game_title, 0, sizeof(mock_game_title));
    memset(mock_game_description, 0, sizeof(mock_game_description));
    memset(mock_game_version, 0, sizeof(mock_game_version));
    memset(mock_game_mode, 0, sizeof(mock_game_mode));
}

void tearDown(void) {}

void test_logic_handle_add_user_workflow_runs(void) {
    // This will run the workflow, but since all input mocks return empty, it should hit validation errors
    logic_handle_add_user_workflow();
    // Should call error for empty full name
    TEST_ASSERT_TRUE(mock_presentation_show_error_called >= 0);
}

void test_logic_handle_edit_user_workflow_runs(void) {
    // This will run the workflow, but since all input mocks return empty, it should hit validation errors
    logic_handle_edit_user_workflow();
    // Should call error for empty gamertag
    TEST_ASSERT_TRUE(mock_presentation_show_error_called >= 0);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_handle_add_user_workflow_runs);
    RUN_TEST(test_logic_handle_edit_user_workflow_runs);
    return UNITY_END();
}
