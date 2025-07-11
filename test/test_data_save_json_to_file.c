#define _XOPEN_SOURCE
#undef static
#define static

#include "unity.h"
#include "cJSON.h"
#include "../../src/data/data.c"
#include <unistd.h>
#include <sys/stat.h>

#define TEST_JSON_FILE "test/save_json_test.json"
#define READONLY_DIR "test/readonly_dir"
#define READONLY_FILE "test/readonly_dir/test.json"

void setUp(void) {
    // Clean up test files
    remove(TEST_JSON_FILE);
    rmdir(READONLY_DIR);
}

void tearDown(void) {
    // Clean up test files
    remove(TEST_JSON_FILE);
    remove(READONLY_FILE);
    rmdir(READONLY_DIR);
}

void test_save_json_to_file_success_returns_ERR_SUCCESS(void) {
    // Arrange: Create a simple JSON object
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", "Test User");
    cJSON_AddNumberToObject(json, "id", 123);
    cJSON_AddBoolToObject(json, "active", 1);

    // Act: Save JSON to file
    int result = save_json_to_file(TEST_JSON_FILE, json);

    // Assert: Check success
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    // Verify file exists and contains correct data
    FILE *f = fopen(TEST_JSON_FILE, "r");
    TEST_ASSERT_NOT_NULL(f);
    
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *data = malloc(len + 1);
    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);

    // Parse and verify JSON content
    cJSON *loaded = cJSON_Parse(data);
    TEST_ASSERT_NOT_NULL(loaded);
    
    cJSON *name = cJSON_GetObjectItem(loaded, "name");
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Test User", name->valuestring);
    
    cJSON *id = cJSON_GetObjectItem(loaded, "id");
    TEST_ASSERT_NOT_NULL(id);
    TEST_ASSERT_EQUAL_INT(123, id->valueint);
    
    cJSON *active = cJSON_GetObjectItem(loaded, "active");
    TEST_ASSERT_NOT_NULL(active);
    TEST_ASSERT_TRUE(cJSON_IsTrue(active));

    free(data);
    cJSON_Delete(loaded);
    cJSON_Delete(json);
}

void test_save_json_to_file_complex_json_success(void) {
    // Arrange: Create complex nested JSON
    cJSON *json = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    
    for (int i = 0; i < 3; i++) {
        cJSON *item = cJSON_CreateObject();
        cJSON_AddNumberToObject(item, "index", i);
        cJSON_AddStringToObject(item, "value", "test");
        cJSON_AddItemToArray(array, item);
    }
    
    cJSON_AddItemToObject(json, "items", array);
    cJSON_AddStringToObject(json, "description", "Complex JSON test");

    // Act: Save JSON to file
    int result = save_json_to_file(TEST_JSON_FILE, json);

    // Assert: Check success
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    // Verify file exists
    FILE *f = fopen(TEST_JSON_FILE, "r");
    TEST_ASSERT_NOT_NULL(f);
    
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *data = malloc(len + 1);
    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);

    // Verify it's valid JSON
    cJSON *loaded = cJSON_Parse(data);
    TEST_ASSERT_NOT_NULL(loaded);
    
    cJSON *items = cJSON_GetObjectItem(loaded, "items");
    TEST_ASSERT_NOT_NULL(items);
    TEST_ASSERT_TRUE(cJSON_IsArray(items));
    TEST_ASSERT_EQUAL_INT(3, cJSON_GetArraySize(items));

    free(data);
    cJSON_Delete(loaded);
    cJSON_Delete(json);
}

void test_save_json_to_file_empty_object_success(void) {
    // Arrange: Create empty JSON object
    cJSON *json = cJSON_CreateObject();

    // Act: Save JSON to file
    int result = save_json_to_file(TEST_JSON_FILE, json);

    // Assert: Check success
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    // Verify file contains empty object
    FILE *f = fopen(TEST_JSON_FILE, "r");
    TEST_ASSERT_NOT_NULL(f);
    
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *data = malloc(len + 1);
    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);

    // Should be valid JSON
    cJSON *loaded = cJSON_Parse(data);
    TEST_ASSERT_NOT_NULL(loaded);
    TEST_ASSERT_TRUE(cJSON_IsObject(loaded));

    free(data);
    cJSON_Delete(loaded);
    cJSON_Delete(json);
}

void test_save_json_to_file_file_open_failure_returns_ERR_STORAGE_FAILURE(void) {
    // Arrange: Create directory with no write permissions
    mkdir(READONLY_DIR, 0444);  // Read-only directory
    
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "test", "data");

    // Act: Try to save to read-only directory
    int result = save_json_to_file(READONLY_FILE, json);

    // Assert: Should fail with storage error
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);

    cJSON_Delete(json);
}

// Test for cJSON_Print failure is tricky since we can't easily make it fail
// without modifying cJSON or using mock functions. In real scenarios,
// cJSON_Print typically fails only on memory allocation failures.
void test_save_json_to_file_null_json_returns_ERR_STORAGE_FAILURE(void) {
    // Act: Try to save NULL (which would make cJSON_Print return NULL)
    int result = save_json_to_file(TEST_JSON_FILE, NULL);

    // Assert: Should fail
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_save_json_to_file_success_returns_ERR_SUCCESS);
    RUN_TEST(test_save_json_to_file_complex_json_success);
    RUN_TEST(test_save_json_to_file_empty_object_success);
    RUN_TEST(test_save_json_to_file_file_open_failure_returns_ERR_STORAGE_FAILURE);
    RUN_TEST(test_save_json_to_file_null_json_returns_ERR_STORAGE_FAILURE);
    return UNITY_END();
}