#include "unity.h"
#include "logic.h"
#include "cJSON.h"
#include "Mocklogic.h"


void test_create_report_should_return_valid_json_when_all_fields_are_provided(void) {
    // Set up the mock expectation
    logic_create_report_ExpectAndReturn("TestTitel", "TestInhalt", "TestDatum", cJSON_CreateObject());

    // Call the mocked function
    cJSON *report = logic_create_report("TestTitel", "TestInhalt", "TestDatum");

    // Verify the result
    TEST_ASSERT_NOT_NULL(report);

    // Clean up
    cJSON_Delete(report);
}

void test_create_report_should_return_null_if_title_is_null(void) {
    // Set up the mock expectation
    logic_create_report_ExpectAndReturn(NULL, "TestInhalt", "TestDatum", NULL);

    // Call the function under test
    cJSON *report = logic_create_report(NULL, "TestInhalt", "TestDatum");

    // Verify the result
    TEST_ASSERT_NULL(report);
}

void test_create_report_should_return_null_if_description_is_null(void) {
    logic_create_report_ExpectAndReturn("TestTitel", NULL, "TestDatum", NULL);
    cJSON *report = logic_create_report("TestTitel", NULL, "TestDatum");
    TEST_ASSERT_NULL(report);
}

void test_create_report_should_return_null_if_date_is_null(void) {
    logic_create_report_ExpectAndReturn("TestTitel", "TestInhalt", NULL, NULL);
    cJSON *report = logic_create_report("TestTitel", "TestInhalt", NULL);
    TEST_ASSERT_NULL(report);
}

void test_create_report_should_return_null_if_all_fields_are_null(void) {
    logic_create_report_ExpectAndReturn(NULL, NULL, NULL, NULL);
    cJSON *report = logic_create_report(NULL, NULL, NULL);
    TEST_ASSERT_NULL(report);
}

// Entry point for all tests in this file
void test_logic_function(void) {
    RUN_TEST(test_create_report_should_return_valid_json_when_all_fields_are_provided);
    RUN_TEST(test_create_report_should_return_null_if_title_is_null);
    RUN_TEST(test_create_report_should_return_null_if_description_is_null);
    RUN_TEST(test_create_report_should_return_null_if_date_is_null);
    RUN_TEST(test_create_report_should_return_null_if_all_fields_are_null);
}
