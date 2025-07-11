#include "unity.h"
#include "cJSON.h"
#include "../../inc/data/data.h"
#include "../../inc/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Override the REPORTS_FILE after includes
#ifdef REPORTS_FILE
#undef REPORTS_FILE
#endif
#define REPORTS_FILE "test/reports_load_test.json"

void setUp(void) {
    // Clean up test file before each test
    remove(REPORTS_FILE);
}

void tearDown(void) {
    // Clean up test file after each test
    remove(REPORTS_FILE);
}

void test_data_load_reports_valid_file_returns_array(void) {
    // Arrange: Create test reports file
    FILE *f = fopen(REPORTS_FILE, "w");
    fputs("["
          "{\"title\":\"Report 1\",\"description\":\"Description 1\",\"date\":\"01.01.2025\"},"
          "{\"title\":\"Report 2\",\"description\":\"Description 2\",\"date\":\"02.01.2025\"}"
          "]", f);
    fclose(f);

    // Act: Load reports
    cJSON *reports = data_load_reports();

    // Assert: Should return valid array
    TEST_ASSERT_NOT_NULL(reports);
    TEST_ASSERT_TRUE(cJSON_IsArray(reports));
    TEST_ASSERT_EQUAL_INT(2, cJSON_GetArraySize(reports));

    // Check content of first report
    cJSON *report1 = cJSON_GetArrayItem(reports, 0);
    TEST_ASSERT_NOT_NULL(report1);
    cJSON *title = cJSON_GetObjectItem(report1, "title");
    cJSON *description = cJSON_GetObjectItem(report1, "description");
    cJSON *date = cJSON_GetObjectItem(report1, "date");
    
    TEST_ASSERT_NOT_NULL(title);
    TEST_ASSERT_NOT_NULL(description);
    TEST_ASSERT_NOT_NULL(date);
    TEST_ASSERT_EQUAL_STRING("Report 1", title->valuestring);
    TEST_ASSERT_EQUAL_STRING("Description 1", description->valuestring);
    TEST_ASSERT_EQUAL_STRING("01.01.2025", date->valuestring);

    // Check content of second report
    cJSON *report2 = cJSON_GetArrayItem(reports, 1);
    TEST_ASSERT_NOT_NULL(report2);
    title = cJSON_GetObjectItem(report2, "title");
    description = cJSON_GetObjectItem(report2, "description");
    date = cJSON_GetObjectItem(report2, "date");
    
    TEST_ASSERT_NOT_NULL(title);
    TEST_ASSERT_NOT_NULL(description);
    TEST_ASSERT_NOT_NULL(date);
    TEST_ASSERT_EQUAL_STRING("Report 2", title->valuestring);
    TEST_ASSERT_EQUAL_STRING("Description 2", description->valuestring);
    TEST_ASSERT_EQUAL_STRING("02.01.2025", date->valuestring);

    cJSON_Delete(reports);
}

void test_data_load_reports_empty_file_returns_empty_array(void) {
    // Arrange: Create empty reports file
    FILE *f = fopen(REPORTS_FILE, "w");
    fputs("[]", f);
    fclose(f);

    // Act: Load reports
    cJSON *reports = data_load_reports();

    // Assert: Should return empty array
    TEST_ASSERT_NOT_NULL(reports);
    TEST_ASSERT_TRUE(cJSON_IsArray(reports));
    TEST_ASSERT_EQUAL_INT(0, cJSON_GetArraySize(reports));

    cJSON_Delete(reports);
}

void test_data_load_reports_missing_file_returns_empty_array(void) {
    // Arrange: Ensure file doesn't exist
    remove(REPORTS_FILE);

    // Act: Load reports from non-existent file
    cJSON *reports = data_load_reports();

    // Assert: Should return empty array (according to the function implementation)
    TEST_ASSERT_NOT_NULL(reports);
    TEST_ASSERT_TRUE(cJSON_IsArray(reports));
    TEST_ASSERT_EQUAL_INT(0, cJSON_GetArraySize(reports));

    cJSON_Delete(reports);
}

void test_data_load_reports_invalid_json_returns_empty_array(void) {
    // Arrange: Create file with invalid JSON
    FILE *f = fopen(REPORTS_FILE, "w");
    fputs("{\"invalid\": \"json format\"", f); // Missing closing brace
    fclose(f);

    // Act: Load reports from invalid JSON file
    cJSON *reports = data_load_reports();

    // Assert: Should return empty array (fallback behavior)
    TEST_ASSERT_NOT_NULL(reports);
    TEST_ASSERT_TRUE(cJSON_IsArray(reports));
    TEST_ASSERT_EQUAL_INT(0, cJSON_GetArraySize(reports));

    cJSON_Delete(reports);
}

void test_data_load_reports_single_report_returns_array_with_one_element(void) {
    // Arrange: Create file with single report
    FILE *f = fopen(REPORTS_FILE, "w");
    fputs("[{\"title\":\"Single Report\",\"description\":\"Only one report\",\"date\":\"10.07.2025\"}]", f);
    fclose(f);

    // Act: Load reports
    cJSON *reports = data_load_reports();

    // Assert: Should return array with one element
    TEST_ASSERT_NOT_NULL(reports);
    TEST_ASSERT_TRUE(cJSON_IsArray(reports));
    TEST_ASSERT_EQUAL_INT(1, cJSON_GetArraySize(reports));

    // Check content
    cJSON *report = cJSON_GetArrayItem(reports, 0);
    TEST_ASSERT_NOT_NULL(report);
    cJSON *title = cJSON_GetObjectItem(report, "title");
    TEST_ASSERT_EQUAL_STRING("Single Report", title->valuestring);

    cJSON_Delete(reports);
}

void test_data_load_reports_complex_reports_with_additional_fields(void) {
    // Arrange: Create file with complex report structure
    FILE *f = fopen(REPORTS_FILE, "w");
    fputs("["
          "{\"title\":\"Complex Report\",\"description\":\"Detailed description\","
          "\"date\":\"15.07.2025\",\"author\":\"Test Author\",\"priority\":\"high\"},"
          "{\"title\":\"Another Report\",\"description\":\"Another description\","
          "\"date\":\"16.07.2025\",\"status\":\"completed\"}"
          "]", f);
    fclose(f);

    // Act: Load reports
    cJSON *reports = data_load_reports();

    // Assert: Should return array with complex data intact
    TEST_ASSERT_NOT_NULL(reports);
    TEST_ASSERT_TRUE(cJSON_IsArray(reports));
    TEST_ASSERT_EQUAL_INT(2, cJSON_GetArraySize(reports));

    // Check first report has additional fields
    cJSON *report1 = cJSON_GetArrayItem(reports, 0);
    cJSON *author = cJSON_GetObjectItem(report1, "author");
    cJSON *priority = cJSON_GetObjectItem(report1, "priority");
    TEST_ASSERT_NOT_NULL(author);
    TEST_ASSERT_NOT_NULL(priority);
    TEST_ASSERT_EQUAL_STRING("Test Author", author->valuestring);
    TEST_ASSERT_EQUAL_STRING("high", priority->valuestring);

    // Check second report has different additional field
    cJSON *report2 = cJSON_GetArrayItem(reports, 1);
    cJSON *status = cJSON_GetObjectItem(report2, "status");
    TEST_ASSERT_NOT_NULL(status);
    TEST_ASSERT_EQUAL_STRING("completed", status->valuestring);

    cJSON_Delete(reports);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_data_load_reports_valid_file_returns_array);
    RUN_TEST(test_data_load_reports_empty_file_returns_empty_array);
    RUN_TEST(test_data_load_reports_missing_file_returns_empty_array);
    RUN_TEST(test_data_load_reports_invalid_json_returns_empty_array);
    RUN_TEST(test_data_load_reports_single_report_returns_array_with_one_element);
    RUN_TEST(test_data_load_reports_complex_reports_with_additional_fields);
    return UNITY_END();
}