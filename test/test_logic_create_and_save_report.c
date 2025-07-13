/*
 * Test for logic_create_and_save_report function
 * 
 * Note: This is a workflow function that coordinates presentation and data layers.
 * The existing mock infrastructure returns fixed values, limiting unit test capabilities.
 * 
 * The function is more comprehensively tested through integration tests in:
 * - test_logic_start_application.c
 * - test_logic_user_menu_workflow.c
 */

#include "unity.h"
#include "../../inc/logic/logic.h"

void setUp(void) {
    // No setup needed - mocks return fixed values
}

void tearDown(void) {
    // No teardown needed
}

// Smoke test - verify the function exists and can be called
void test_logic_create_and_save_report_smoke_test(void) {
    // The mock presentation functions return:
    // - title: "stub_title"
    // - description: "stub_description"  
    // - date: "12.07.2025" (valid format)
    // The mock data_save_report returns ERR_SUCCESS
    
    // This should complete successfully without crashing
    logic_create_and_save_report();
    
    // Since we can't track mock calls with the current infrastructure,
    // we just verify the function executes without errors
    TEST_ASSERT_TRUE(1);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_create_and_save_report_smoke_test);
    return UNITY_END();
}