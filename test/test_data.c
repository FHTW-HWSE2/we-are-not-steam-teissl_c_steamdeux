#define _XOPEN_SOURCE
#undef static
#define static
#define USERS_JSON_PATH "test/users_testing.json"
#include "unity.h"
#include "cJSON.h"
#include "mock_data.h"

void append_expired_test_user_to_json_file(void); // Funktionsprototyp für Hilfsfunktion zum Befüllen der JSON Testdatei

void setUp(void) {
    append_expired_test_user_to_json_file();
}
void tearDown(void) {}

// Testing function load_json_from_file
// Test: Valid JSON file should load successfully
void test_load_json_from_file_valid_file_returns_json(void) {
    cJSON *json = load_json_from_file("test/users_testing.json");
    TEST_ASSERT_NOT_NULL(json);
    TEST_ASSERT_TRUE(cJSON_IsArray(json));
    TEST_ASSERT_GREATER_THAN(0, cJSON_GetArraySize(json));
    cJSON_Delete(json);
}

// Test: Non-existent file should return NULL
void test_load_json_from_file_missing_file_returns_null(void) {
    cJSON *json = load_json_from_file("test/nonexistent.json");
    TEST_ASSERT_NULL(json);
}

// Test: Empty file should return NULL
void test_load_json_from_file_empty_file_returns_null(void) {
    cJSON *json = load_json_from_file("test/empty.json");
    TEST_ASSERT_NULL(json);
}

// Test: Invalid JSON file should return NULL
void test_load_json_from_file_invalid_json_returns_null(void) {
    cJSON *json = load_json_from_file("test/broken.json");
    TEST_ASSERT_NULL(json);
}


//Testing function data_save_report
void test_data_save_report_success(void) {
    #undef REPORTS_FILE
    #define REPORTS_FILE "test/reports_testing.json"
    // Arrange: erstelle ein neues Report-Objekt
    cJSON *report = cJSON_CreateObject();
    cJSON_AddStringToObject(report, "title", "UnitTest Report");
    cJSON_AddStringToObject(report, "description", "Automatisierter Test");
    cJSON_AddStringToObject(report, "date", "07.07.2025");

    // Act: speichere das Report-Objekt
    int result = data_save_report(report);

    // Assert
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    // Kein cJSON_Delete(report), da ownership an Array übergeben wurde
}

void test_data_save_report_with_null_report_returns_failure(void) {
    int result = data_save_report(NULL);
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result); // oder was deine Funktion zurückgibt
}

void test_data_save_report_fails_when_existing_file_is_not_array(void) {
    #undef REPORTS_FILE
    #define REPORTS_FILE "test/reports_invalid_object.json"

    FILE *f = fopen(REPORTS_FILE, "r");
    if (!f) {
        TEST_FAIL_MESSAGE("Fehler: reports_invalid_object.json konnte nicht geöffnet werden!");
    }
    char buffer[256];
    fread(buffer, 1, sizeof(buffer) - 1, f);
    buffer[255] = '\0';
    printf("[DEBUG] Inhalt von reports_invalid_object.json: %s\n", buffer);
    fclose(f);

    cJSON *report = cJSON_CreateObject();
    cJSON_AddStringToObject(report, "title", "Invalid Target Test");
    cJSON_AddStringToObject(report, "description", "Test gegen Objekt statt Array");
    cJSON_AddStringToObject(report, "date", "08.07.2025");

    int result = data_save_report(report);

    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);

    cJSON_Delete(report);
}

void test_data_save_report_null_with_missing_file_returns_failure(void) {
    #undef REPORTS_FILE
    #define REPORTS_FILE "test/this_file_does_not_exist.json"
    int result = data_save_report(NULL);
    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
}

// Test funtion data_remove_expired_users
// Success case
void test_data_remove_expired_users_success_returns_ERR_SUCCESS(void) {
    #undef USERS_JSON_PATH
    #define USERS_JSON_PATH "test/users_testing.json"

    int removed = -1;
    int result = data_remove_expired_users(&removed);

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
    TEST_ASSERT_GREATER_OR_EQUAL(1, removed); // mindestens ein User entfernt?
}

// Helper Funktion damit das users_testing.json mit einem User mit abgelaufenen subscription date gefüllt ist
// Verhindert, dass der Successfall-Test fehlschlägt, wenn kein User entfernt wird.
// Die Funktion wird in setup() aufgerufen.
void append_expired_test_user_to_json_file(void) {
    const char *filepath = "test/users_testing.json";

    // Dateiinhalt laden
    cJSON *users = NULL;
    FILE *f = fopen(filepath, "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        long len = ftell(f);
        fseek(f, 0, SEEK_SET);
        char *data = malloc(len + 1);
        fread(data, 1, len, f);
        data[len] = '\0';
        fclose(f);
        users = cJSON_Parse(data);
        free(data);
    }

    if (!users || !cJSON_IsArray(users)) {
        if (users) cJSON_Delete(users);
        users = cJSON_CreateArray(); // Wenn Datei leer oder kaputt ist
    }

    // Abgelaufenen User hinzufügen
    cJSON *expired = cJSON_CreateObject();
    cJSON_AddStringToObject(expired, "full_name", "Expired Testuser");
    cJSON_AddStringToObject(expired, "gamertag", "expired999");
    cJSON_AddNumberToObject(expired, "player_hours", 0);
    cJSON_AddStringToObject(expired, "ssn", "0000-000000");
    cJSON_AddStringToObject(expired, "email", "expired@example.com");
    cJSON_AddStringToObject(expired, "subscription_start_date", "01.01.2020");
    cJSON_AddStringToObject(expired, "subscription_end_date", "01.01.2021");
    cJSON_AddBoolToObject(expired, "is_subscribed", 0);
    cJSON_AddItemToArray(users, expired);

    // Datei überschreiben
    char *json_str = cJSON_Print(users);
    FILE *out = fopen(filepath, "w");
    if (out) {
        fputs(json_str, out);
        fclose(out);
    }
    free(json_str);
    cJSON_Delete(users);
}



int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_load_json_from_file_valid_file_returns_json);
    RUN_TEST(test_load_json_from_file_missing_file_returns_null);
    RUN_TEST(test_load_json_from_file_empty_file_returns_null);
    RUN_TEST(test_load_json_from_file_invalid_json_returns_null);
    RUN_TEST(test_data_save_report_success);
    RUN_TEST(test_data_save_report_with_null_report_returns_failure);
    RUN_TEST(test_data_save_report_fails_when_existing_file_is_not_array);
    RUN_TEST(test_data_save_report_null_with_missing_file_returns_failure);
    RUN_TEST(test_data_remove_expired_users_success_returns_ERR_SUCCESS);
    return UNITY_END();
}

