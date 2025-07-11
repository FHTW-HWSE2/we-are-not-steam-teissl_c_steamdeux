#define _XOPEN_SOURCE
#undef static
#define static
#define REPORTS_FILE "test/missing_or_invalid_report.json"

#include "unity.h"
#include "cJSON.h"
#include "../../src/data/data.c"

void setUp(void) {
    // Option 1: Datei existiert nicht → garantiert load_json_from_file() == NULL
    remove(REPORTS_FILE);
    
    // Alternativ (für 2. Test): Leere Datei erzeugen
    // FILE *f = fopen(REPORTS_FILE, "w");
    // fclose(f);
}

void tearDown(void) {
    remove(REPORTS_FILE);
}

void test_data_save_report_fallback_creates_array(void) {
    cJSON *report = cJSON_CreateObject();
    cJSON_AddStringToObject(report, "title", "Test");
    cJSON_AddStringToObject(report, "description", "Fallback Test");
    cJSON_AddStringToObject(report, "date", "09.07.2025");

    int result = data_save_report(report);

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    // Optional: prüfen, ob Datei jetzt wirklich ein Array enthält
    FILE *f = fopen(REPORTS_FILE, "r");
    TEST_ASSERT_NOT_NULL(f);
    char buffer[128] = {0};
    fread(buffer, 1, sizeof(buffer) - 1, f);
    fclose(f);
    TEST_ASSERT_TRUE(strchr(buffer, '[') != NULL);  // beginnt mit Array
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_data_save_report_fallback_creates_array);
    return UNITY_END();
}
