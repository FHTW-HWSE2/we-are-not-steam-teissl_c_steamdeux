#define USERS_JSON_PATH "test/users_edit.json"

#include "unity.h"
#include "cJSON.h"
#include "../../inc/data/data.h"
#include "../inc/error.h"
#include <stdio.h>
#include <stdlib.h>

// Tests für die Funktion data_edit_player_profile

// Ersatz für static load_json_from_file aus data.c, helper function: JSON-Datei einlesen
static cJSON* load_json_from_testfile(const char* path) {
    FILE *file = fopen(path, "r");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = malloc(length + 1);
    if (!data) {
        fclose(file);
        return NULL;
    }

    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    cJSON *json = cJSON_Parse(data);
    free(data);
    return json;
}

void setUp(void) {
    FILE *f = fopen(USERS_JSON_PATH, "w");
    fputs("[{\"full_name\":\"Altname\",\"gamertag\":\"editme123\",\"player_hours\":42,"
          "\"ssn\":\"1234-567890\",\"email\":\"alt@example.com\","
          "\"subscription_start_date\":\"01.01.2024\",\"subscription_end_date\":\"01.01.2025\","
          "\"is_subscribed\":true}]", f);
    fclose(f);
}

void tearDown(void) {}

void test_data_edit_player_profile_success_returns_ERR_SUCCESS(void) {
    int result = data_edit_player_profile(
        "editme123",        // gamertag
        "Geändert",         // new_full_name
        "", "", "", "",     // keine weiteren Änderungen
        -1                  // -1 = keine Änderung für Abo-Status
    );

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    // Verifizieren, ob der Name geändert wurde
    cJSON *users = load_json_from_testfile(USERS_JSON_PATH);
    TEST_ASSERT_NOT_NULL(users);
    cJSON *user = cJSON_GetArrayItem(users, 0);
    TEST_ASSERT_NOT_NULL(user);

    cJSON *name = cJSON_GetObjectItem(user, "full_name");
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Geändert", name->valuestring);

    cJSON_Delete(users);
}

// === Test: Gamertag nicht vorhanden ===
void test_data_edit_profile_user_not_found_returns_ERR_USER_NOT_FOUND(void) {
    // Datei mit anderem User vorbereiten
    FILE *f = fopen(USERS_JSON_PATH, "w");
    fputs("[{\"full_name\":\"Test\",\"gamertag\":\"notme\",\"player_hours\":0,"
          "\"ssn\":\"0000-000000\",\"email\":\"none@example.com\","
          "\"subscription_start_date\":\"01.01.2023\",\"subscription_end_date\":\"01.01.2024\","
          "\"is_subscribed\":false}]", f);
    fclose(f);

    int result = data_edit_player_profile("editme123", "Neuer Name", "", "", "", "", -1);
    TEST_ASSERT_EQUAL_INT(ERR_USER_NOT_FOUND, result);
}

// === Test: E-Mail wird aktualisiert ===
void test_data_edit_profile_email_updated_successfully(void) {
    int result = data_edit_player_profile("editme123", "", "", "new@example.com", "", "", -1);
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    cJSON *users = load_json_from_testfile(USERS_JSON_PATH);
    cJSON *user = cJSON_GetArrayItem(users, 0);
    TEST_ASSERT_EQUAL_STRING("new@example.com", cJSON_GetObjectItem(user, "email")->valuestring);
    cJSON_Delete(users);
}

// === Test: Abo-Status wird gesetzt ===
void test_data_edit_profile_subscription_flag_updated_successfully(void) {
    int result = data_edit_player_profile("editme123", "", "", "", "", "", 0); // setze auf false
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    cJSON *users = load_json_from_testfile(USERS_JSON_PATH);
    cJSON *user = cJSON_GetArrayItem(users, 0);
    TEST_ASSERT_EQUAL_INT(0, cJSON_GetObjectItem(user, "is_subscribed")->valueint);
    cJSON_Delete(users);
}

// === Test: SSN wird aktualisiert ===
void test_data_edit_profile_ssn_updated_successfully(void) {
    int result = data_edit_player_profile("editme123", "", "9999-123456", "", "", "", -1);
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    cJSON *users = load_json_from_testfile(USERS_JSON_PATH);
    TEST_ASSERT_NOT_NULL(users);
    cJSON *user = cJSON_GetArrayItem(users, 0);
    TEST_ASSERT_NOT_NULL(user);

    cJSON *ssn = cJSON_GetObjectItem(user, "ssn");
    TEST_ASSERT_NOT_NULL(ssn);
    TEST_ASSERT_EQUAL_STRING("9999-123456", ssn->valuestring);

    cJSON_Delete(users);
}

// === Test: Abo-Start-Datum wird aktualisiert ===
void test_data_edit_profile_sub_start_updated_successfully(void) {
    int result = data_edit_player_profile("editme123", "", "", "", "15.08.2025", "", -1);
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    cJSON *users = load_json_from_testfile(USERS_JSON_PATH);
    TEST_ASSERT_NOT_NULL(users);
    cJSON *user = cJSON_GetArrayItem(users, 0);

    cJSON *start = cJSON_GetObjectItem(user, "subscription_start_date");
    TEST_ASSERT_NOT_NULL(start);
    TEST_ASSERT_EQUAL_STRING("15.08.2025", start->valuestring);

    cJSON_Delete(users);
}

// === Test: Abo-End-Datum wird aktualisiert ===
void test_data_edit_profile_sub_end_updated_successfully(void) {
    int result = data_edit_player_profile("editme123", "", "", "", "", "31.12.2025", -1);
    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);

    cJSON *users = load_json_from_testfile(USERS_JSON_PATH);
    TEST_ASSERT_NOT_NULL(users);
    cJSON *user = cJSON_GetArrayItem(users, 0);

    cJSON *end = cJSON_GetObjectItem(user, "subscription_end_date");
    TEST_ASSERT_NOT_NULL(end);
    TEST_ASSERT_EQUAL_STRING("31.12.2025", end->valuestring);

    cJSON_Delete(users);
}


// === Main ===
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_data_edit_player_profile_success_returns_ERR_SUCCESS);
    RUN_TEST(test_data_edit_profile_user_not_found_returns_ERR_USER_NOT_FOUND);
    RUN_TEST(test_data_edit_profile_email_updated_successfully);
    RUN_TEST(test_data_edit_profile_subscription_flag_updated_successfully);
    RUN_TEST(test_data_edit_profile_ssn_updated_successfully);
    RUN_TEST(test_data_edit_profile_sub_start_updated_successfully);
    RUN_TEST(test_data_edit_profile_sub_end_updated_successfully);

    return UNITY_END();
}