#include "unity.h"
#include "../../inc/logic/logic.h"
#include "cJSON.h"

void setUp(void) {}    // optional
void tearDown(void) {} // optional

void test_logic_create_report_valid_inputs(void) {
    const char* title = "Monthly Report";
    const char* description = "Summary of user activity";
    const char* date = "01.06.2025";

    cJSON* report = logic_create_report(title, description, date);

    TEST_ASSERT_NOT_NULL(report);
    TEST_ASSERT_EQUAL_STRING(title, cJSON_GetObjectItem(report, "title")->valuestring);
    TEST_ASSERT_EQUAL_STRING(description, cJSON_GetObjectItem(report, "description")->valuestring);
    TEST_ASSERT_EQUAL_STRING(date, cJSON_GetObjectItem(report, "date")->valuestring);

    cJSON_Delete(report);
}

void test_logic_create_report_null_input(void) {
    cJSON* report = logic_create_report(NULL, "desc", "01.06.2025");
    TEST_ASSERT_NULL(report);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_create_report_valid_inputs);
    RUN_TEST(test_logic_create_report_null_input);
    return UNITY_END();
}
