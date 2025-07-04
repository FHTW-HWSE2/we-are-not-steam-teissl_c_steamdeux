#include "unity.h"
#include "../../inc/logic/logic.h"
#include "cJSON.h"
#include "mock_data.h" // CMock-generierter Mock für save_player_profile
#include "../../inc/error.h"

void setUp(void) {
    mock_data_Init();  // Reset aller internen Mock-Daten
}

void tearDown(void) {
    mock_data_Verify();  // Sicherstellen, dass erwartete Aufrufe passiert sind
    mock_data_Destroy();
}

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

// Tests für validate_player_profile
// Success
void test_valid_input_returns_ERR_SUCCESS(void) {
    // Eingabedaten – alle gültig
    const char* full_name = "Max Mustermann";
    const char* gamertag = "Player123";
    const char* ssn = "1234-567890";
    const char* email = "max@test.com";
    const char* sub_start = "05.07.2025";
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    // Erwarteter Aufruf von save_player_profile mit diesen Parametern
    save_player_profile_ExpectAndReturn(
        full_name,
        gamertag,
        0,               // player_hours
        ssn,
        email,
        sub_start,
        sub_end,
        1,               // is_subscribed = true
        0                // return 0 → erfolgreich
    );

    int result = validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_SUCCESS, result);
}


void test_start_date_in_past_returns_ERR_PAST_DATE(void) {
    // Alle Felder gültig – außer: Startdatum liegt in der Vergangenheit
    const char* full_name = "Old Start";
    const char* gamertag = "PastStartUser";
    const char* ssn = "1234-567890";
    const char* email = "past@example.com";
    const char* sub_start = "01.01.2000";  // Vergangenheit!
    const char* sub_end = "01.01.2100";    // Gültig
    const char* is_subscribed_str = "true";

    // Kein Mock nötig, da die Funktion vorher schon abbricht
    int result = validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_PAST_DATE, result);
}


void test_empty_field_returns_ERR_EMPTY_FIELD(void) { //siehe Kommentar validate_player_profile in logic.c
    // Mindestens ein Pflichtfeld ist leer
    const char* full_name = ""; // leer
    const char* gamertag = "Player123";
    const char* ssn = "1234-567890";
    const char* email = "max@test.com";
    const char* sub_start = "05.07.2025";
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    // Kein Mock nötig, da die Funktion vorher schon abbricht
    int result = validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_EMPTY_FIELD, result);
}


void test_invalid_subscription_status_returns_ERR_INVALID_SUB_STATUS(void) {
    // Alle Felder gültig – außer: Abo-Status ist ungültig
    const char* full_name = "Invalid Sub User";
    const char* gamertag = "InvalidSubUser";
    const char* ssn = "1234-567890";
    const char* email = "invalid@example.com";
    const char* sub_start = "05.07.2025";
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "invalid"; // ungültig!

    // Kein Mock nötig, da die Funktion vorher schon abbricht
    int result = validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SUB_STATUS, result);
}


void test_storage_failure_returns_ERR_STORAGE_FAILURE(void) {
    // Alle Felder gültig – aber save_player_profile schlägt fehl
    const char* full_name = "Storage Fail User";
    const char* gamertag = "StorageFailUser";
    const char* ssn = "1234-567890";
    const char* email = "storage@example.com";
    const char* sub_start = "05.07.2025";
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    // Erwarteter Aufruf von save_player_profile – aber mit Fehler
    save_player_profile_ExpectAndReturn(
        full_name,
        gamertag,
        0,               // player_hours
        ssn,
        email,
        sub_start,
        sub_end,
        1,               // is_subscribed = true
        1                // return 1 → Fehler beim Speichern
    );

    int result = validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_create_report_valid_inputs);
    RUN_TEST(test_logic_create_report_null_input);
    RUN_TEST(test_valid_input_returns_ERR_SUCCESS);
    RUN_TEST(test_start_date_in_past_returns_ERR_PAST_DATE);
    RUN_TEST(test_empty_field_returns_ERR_EMPTY_FIELD); // hinzugefügt
    RUN_TEST(test_invalid_subscription_status_returns_ERR_INVALID_SUB_STATUS); // hinzugefügt
    RUN_TEST(test_storage_failure_returns_ERR_STORAGE_FAILURE); // hinzugefügt
    return UNITY_END();
}
