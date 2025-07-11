#include "unity.h"
#include "cJSON.h"
#include "../../inc/logic/logic.h" // Assuming logic.h contains the function declaration
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_logic_create_report_success(void) {
    // Arrange
    const char *title = "Test Report Title";
    const char *description = "This is a test description for the report.";
    const char *date = "11.07.2025";

    // Act
    cJSON *report = logic_create_report(title, description, date);

    // Assert
    TEST_ASSERT_NOT_NULL(report);
    TEST_ASSERT_TRUE(cJSON_IsObject(report));

    // Verify content
    cJSON *title_item = cJSON_GetObjectItem(report, "title");
    cJSON *description_item = cJSON_GetObjectItem(report, "description");
    cJSON *date_item = cJSON_GetObjectItem(report, "date");

    TEST_ASSERT_NOT_NULL(title_item);
    TEST_ASSERT_NOT_NULL(description_item);
    TEST_ASSERT_NOT_NULL(date_item);

    TEST_ASSERT_EQUAL_STRING(title, title_item->valuestring);
    TEST_ASSERT_EQUAL_STRING(description, description_item->valuestring);
    TEST_ASSERT_EQUAL_STRING(date, date_item->valuestring);

    // Clean up
    cJSON_Delete(report);
}

void test_logic_create_report_null_title(void) {
    // Arrange
    const char *title = NULL;
    const char *description = "This is a test description.";
    const char *date = "11.07.2025";

    // Act
    cJSON *report = logic_create_report(title, description, date);

    // Assert
    TEST_ASSERT_NULL(report); // Should return NULL if title is NULL
}

void test_logic_create_report_null_description(void) {
    // Arrange
    const char *title = "Test Report Title";
    const char *description = NULL;
    const char *date = "11.07.2025";

    // Act
    cJSON *report = logic_create_report(title, description, date);

    // Assert
    TEST_ASSERT_NULL(report); // Should return NULL if description is NULL
}

void test_logic_create_report_null_date(void) {
    // Arrange
    const char *title = "Test Report Title";
    const char *description = "This is a test description.";
    const char *date = NULL;

    // Act
    cJSON *report = logic_create_report(title, description, date);

    // Assert
    TEST_ASSERT_NULL(report); // Should return NULL if date is NULL
}

void test_logic_create_report_all_null(void) {
    // Arrange
    const char *title = NULL;
    const char *description = NULL;
    const char *date = NULL;

    // Act
    cJSON *report = logic_create_report(title, description, date);

    // Assert
    TEST_ASSERT_NULL(report); // Should return NULL if all are NULL
}

void test_logic_create_report_empty_strings(void) {
    // Arrange
    const char *title = "";
    const char *description = "";
    const char *date = "";

    // Act
    cJSON *report = logic_create_report(title, description, date);

    // Assert
    TEST_ASSERT_NOT_NULL(report);
    TEST_ASSERT_TRUE(cJSON_IsObject(report));

    // Verify content - should contain empty strings
    cJSON *title_item = cJSON_GetObjectItem(report, "title");
    cJSON *description_item = cJSON_GetObjectItem(report, "description");
    cJSON *date_item = cJSON_GetObjectItem(report, "date");

    TEST_ASSERT_NOT_NULL(title_item);
    TEST_ASSERT_NOT_NULL(description_item);
    TEST_ASSERT_NOT_NULL(date_item);

    TEST_ASSERT_EQUAL_STRING("", title_item->valuestring);
    TEST_ASSERT_EQUAL_STRING("", description_item->valuestring);
    TEST_ASSERT_EQUAL_STRING("", date_item->valuestring);

    // Clean up
    cJSON_Delete(report);
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_create_report_success);
    RUN_TEST(test_logic_create_report_null_title);
    RUN_TEST(test_logic_create_report_null_description);
    RUN_TEST(test_logic_create_report_null_date);
    RUN_TEST(test_logic_create_report_all_null);
    RUN_TEST(test_logic_create_report_empty_strings);
    return UNITY_END();
}
