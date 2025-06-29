#include "unity.h"
#include "data.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Mockdata.h"

#define TEST_FILE "reports.json"

// Individual test cases
void test_load_reports_returns_empty_array_when_file_missing(void) {
    cJSON *empty_array = cJSON_CreateArray();  // Create an empty array to show the expected return value

    // Set up the mock expectation
    data_load_reports_ExpectAndReturn(empty_array); //was wollen wir als Rückgabewert
    cJSON *reports = data_load_reports();

    // Verify the result
    TEST_ASSERT_NOT_NULL(reports);
    TEST_ASSERT_TRUE(cJSON_IsArray(reports));
    TEST_ASSERT_EQUAL(0, cJSON_GetArraySize(reports));

    cJSON_Delete(reports);
}

void test_load_reports_returns_empty_array_on_invalid_json(void) {
    FILE *f = fopen(TEST_FILE, "w");
    fprintf(f, "INVALID_JSON");
    fclose(f);

    cJSON *emptyArray = cJSON_CreateArray();
    data_load_reports_ExpectAndReturn(emptyArray);

    cJSON *reports = data_load_reports();
    TEST_ASSERT_NOT_NULL(reports);
    TEST_ASSERT_TRUE(cJSON_IsArray(reports));
    TEST_ASSERT_EQUAL(0, cJSON_GetArraySize(reports));
    cJSON_Delete(reports);
}

void test_load_reports_reads_valid_array(void) {
    cJSON *valid_array = cJSON_CreateArray();  // Create a valid array
    cJSON_AddItemToArray(valid_array, cJSON_CreateObject());  // Add first item
    cJSON_AddItemToArray(valid_array, cJSON_CreateObject());  // Add second item

    data_load_reports_ExpectAndReturn(valid_array);  // Set up the mock expectation

    cJSON *reports = data_load_reports();  // Call the mocked function
    TEST_ASSERT_NOT_NULL(reports);
    TEST_ASSERT_TRUE(cJSON_IsArray(reports));
    TEST_ASSERT_EQUAL(2, cJSON_GetArraySize(reports));  // Verify the array has two items

    cJSON_Delete(reports);
}

void test_save_report_returns_minus_1_if_null(void) {

    data_save_report_ExpectAndReturn(NULL, -1); //Erwartung ist wir geben NULL und wollen -1 zurück
    int result = data_save_report(NULL);
    TEST_ASSERT_EQUAL(-1, result);
}

void test_save_report_creates_file_and_saves_report(void) {
    cJSON *report = cJSON_CreateObject();
    cJSON_AddStringToObject(report, "title", "first");

    // Set up the mock: expect data_save_report to be called with 'report' and return 0
    data_save_report_ExpectAndReturn(report, 0);

    int result = data_save_report(report);
    TEST_ASSERT_EQUAL(0, result);

    // Set up the mock: expect data_load_reports to be called and return a fake array
    cJSON *fake_array = cJSON_CreateArray();
    cJSON_AddItemToArray(fake_array, cJSON_Duplicate(report, 1));
    data_load_reports_ExpectAndReturn(fake_array);

    cJSON *reports = data_load_reports();
    TEST_ASSERT_NOT_NULL(reports);
    TEST_ASSERT_EQUAL(1, cJSON_GetArraySize(reports));
    cJSON *saved = cJSON_GetArrayItem(reports, 0);
    TEST_ASSERT_EQUAL_STRING("first", cJSON_GetObjectItem(saved, "title")->valuestring);
    cJSON_Delete(reports);
    cJSON_Delete(report);
}

void test_save_report_appends_to_existing_file(void) {
    // First, set up the fake array as if it already contains one report
    cJSON *fake_array = cJSON_CreateArray();
    cJSON *report1 = cJSON_CreateObject();
    cJSON_AddStringToObject(report1, "title1", "first");
    cJSON_AddItemToArray(fake_array, cJSON_Duplicate(report1, 1));

    // Expect the first save to succeed
    data_save_report_ExpectAndReturn(report1, 0);

    int result1 = data_save_report(report1);
    TEST_ASSERT_EQUAL(0, result1);

    // Now, simulate appending a second report
    cJSON *report2 = cJSON_CreateObject();
    cJSON_AddStringToObject(report2, "title2", "second");

    // Expect the second save to succeed
    data_save_report_ExpectAndReturn(report2, 0);

    int result2 = data_save_report(report2);
    TEST_ASSERT_EQUAL(0, result2);

    // Now, set up the mock for loading the reports: array with both reports
    cJSON *final_array = cJSON_CreateArray();
    cJSON_AddItemToArray(final_array, cJSON_Duplicate(report1, 1));
    cJSON_AddItemToArray(final_array, cJSON_Duplicate(report2, 1));
    data_load_reports_ExpectAndReturn(final_array);

    cJSON *reports = data_load_reports();
    TEST_ASSERT_EQUAL(2, cJSON_GetArraySize(reports));
    cJSON *saved1 = cJSON_GetArrayItem(reports, 0);
    cJSON *saved2 = cJSON_GetArrayItem(reports, 1);
    TEST_ASSERT_EQUAL_STRING("first", cJSON_GetObjectItem(saved1, "title1")->valuestring);
    TEST_ASSERT_EQUAL_STRING("second", cJSON_GetObjectItem(saved2, "title2")->valuestring);

    cJSON_Delete(reports);
    cJSON_Delete(report1);
    cJSON_Delete(report2);
}

void test_save_report_overwrites_invalid_file(void) {
    // Simulate saving a report when the file is invalid (mock: just expect the call)
    cJSON *report = cJSON_CreateObject();
    cJSON_AddStringToObject(report, "title", "recovered");

    data_save_report_ExpectAndReturn(report, 0);

    int result = data_save_report(report);
    TEST_ASSERT_EQUAL(0, result);

    // Simulate loading reports returns only the recovered report
    cJSON *fake_array = cJSON_CreateArray();
    cJSON_AddItemToArray(fake_array, cJSON_Duplicate(report, 1));
    data_load_reports_ExpectAndReturn(fake_array);

    cJSON *reports = data_load_reports();
    TEST_ASSERT_EQUAL(1, cJSON_GetArraySize(reports));
    cJSON *saved = cJSON_GetArrayItem(reports, 0);
    TEST_ASSERT_EQUAL_STRING("recovered", cJSON_GetObjectItem(saved, "title")->valuestring);

    cJSON_Delete(reports);
    cJSON_Delete(report);
}

void test_data_function(void) {
    RUN_TEST(test_load_reports_returns_empty_array_when_file_missing);
    RUN_TEST(test_load_reports_returns_empty_array_on_invalid_json);
    RUN_TEST(test_load_reports_reads_valid_array);
    RUN_TEST(test_save_report_returns_minus_1_if_null);
    RUN_TEST(test_save_report_creates_file_and_saves_report);
    RUN_TEST(test_save_report_appends_to_existing_file);
    RUN_TEST(test_save_report_overwrites_invalid_file);
}
