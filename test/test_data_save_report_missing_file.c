#define REPORTS_FILE "test/reports_created_from_missing.json"

#include "unity.h"
#include "cJSON.h"
#include "../../inc/data/data.h"
#include "../inc/error.h"
#include <stdio.h>
#include <stdlib.h>

void setUp(void) {
    remove(REPORTS_FILE);
}

void tearDown(void) {}

void test_data_save_report_missing_file_creates_new_array(void) {
    cJSON *report = cJSON_CreateObject();
    cJSON_AddStringToObject(report, "title", "Missing File");
    cJSON_AddStringToObject(report, "description", "Created new file");
    cJSON_AddStringToObject(report, "date", "09.07.2025");

    int result = data_save_report(report);
    printf("%d", result); // Debug-Ausgabe des Rückgabewerts
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    FILE *f = fopen(REPORTS_FILE, "r");
    TEST_ASSERT_NOT_NULL(f);
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *data = malloc(len + 1);
    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);

    cJSON *json = cJSON_Parse(data);
    TEST_ASSERT_TRUE(cJSON_IsArray(json));
    TEST_ASSERT_EQUAL_INT(1, cJSON_GetArraySize(json));

    free(data);
    cJSON_Delete(json);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_data_save_report_missing_file_creates_new_array);
    return UNITY_END();
}
