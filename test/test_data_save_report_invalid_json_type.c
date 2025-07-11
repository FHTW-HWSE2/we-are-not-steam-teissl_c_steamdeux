#define _XOPEN_SOURCE
#undef static
#define static
#define REPORTS_FILE "test/reports_invalid_object.json"

#include "unity.h"
#include "cJSON.h"
#include "../../src/data/data.c"

void setUp(void) {
    // Erzeuge eine Datei mit gültigem JSON – aber KEINEM Array
    FILE *f = fopen(REPORTS_FILE, "w");
    fputs("{\"not\": \"an array\"}", f);
    fclose(f);
}

void tearDown(void) {
    remove(REPORTS_FILE);
}

void test_data_save_report_invalid_json_object_returns_ERR_STORAGE_FAILURE(void) {
    cJSON *report = cJSON_CreateObject();
    cJSON_AddStringToObject(report, "title", "Invalid JSON");
    cJSON_AddStringToObject(report, "description", "This won't be saved");
    cJSON_AddStringToObject(report, "date", "09.07.2025");

    int result = data_save_report(report);

    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);

    cJSON_Delete(report);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_data_save_report_invalid_json_object_returns_ERR_STORAGE_FAILURE);
    return UNITY_END();
}
