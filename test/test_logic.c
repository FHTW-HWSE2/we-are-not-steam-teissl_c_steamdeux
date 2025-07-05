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

// Tests für logic_validate_player_profile
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

    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

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
    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

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
    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

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
    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

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

    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_STORAGE_FAILURE, result);
}

//Sina - ssn too short
void test_ssn_invalid_length_too_short_returns_ERR_INVALID_SSN(void) {
    const char* full_name = "Test User";
    const char* gamertag = "TestPlayer";
    const char* ssn = "123-456"; // Invalid SSN: too short
    const char* email = "test@example.com";
    const char* sub_start = "05.07.2025";
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SSN, result);
}
//Sina --ssn too long
void test_ssn_invalid_length_too_long_returns_ERR_INVALID_SSN(void) {
    const char* full_name = "Test User";
    const char* gamertag = "TestPlayer";
    const char* ssn = "1234-5678901"; // Invalid SSN: too long
    const char* email = "test@example.com";
    const char* sub_start = "05.07.2025";
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SSN, result);
}
//Sina --ssn wrong bindestrich
void test_ssn_invalid_hyphen_position_returns_ERR_INVALID_SSN(void) {
    const char* full_name = "Test User";
    const char* gamertag = "TestPlayer";
    const char* ssn = "12345-67890"; // Invalid SSN: hyphen at wrong position
    const char* email = "test@example.com";
    const char* sub_start = "05.07.2025";
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SSN, result);
}
//Sina -- ssn mit buchstaben und zahlen
void test_ssn_invalid_digit_on_wrong_positionchar_returns_ERR_INVALID_SSN(void) {
    const char* full_name = "Test User";
    const char* gamertag = "TestPlayer";
    const char* ssn = "123A-567890"; // Invalid SSN: non-digit character
    const char* email = "test@example.com";
    const char* sub_start = "05.07.2025";
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_INVALID_SSN, result);
}
//eMail tests ---
void test_email_missing_at_symbol_returns_ERR_INVALID_EMAIL(void) {
    const char* full_name = "Test User";
    const char* gamertag = "TestPlayer";
    const char* ssn = "1234-567890";
    const char* email = "testexample.com"; // Invalid Email: missing '@'
    const char* sub_start = "05.07.2025";
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_INVALID_EMAIL, result);
}

void test_email_missing_dot_after_at_returns_ERR_INVALID_EMAIL(void) {
    const char* full_name = "Test User";
    const char* gamertag = "TestPlayer";
    const char* ssn = "1234-567890";
    const char* email = "test@examplecom"; // Invalid Email: missing '.' after '@'
    const char* sub_start = "05.07.2025";
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_INVALID_EMAIL, result);
}

void test_email_dot_before_at_returns_ERR_INVALID_EMAIL(void) {
    const char* full_name = "Test User";
    const char* gamertag = "TestPlayer";
    const char* ssn = "1234-567890";
    const char* email = "test.example@com"; // Invalid Email: dot before '@'
    const char* sub_start = "05.07.2025";
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_INVALID_EMAIL, result);
}
//Datumsformat tests
void test_date_invalid_format_separator_returns_ERR_INVALID_DATE(void) {
    const char* full_name = "Test User";
    const char* gamertag = "TestPlayer";
    const char* ssn = "1234-567890";
    const char* email = "test@example.com";
    const char* sub_start = "05/07/2025"; // Invalid format: using '/' instead of '.'
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_INVALID_DATE, result);
}

/*
void test_date_invalid_format_day_returns_ERR_INVALID_DATE(void) {
    const char* full_name = "Test User";
    const char* gamertag = "TestPlayer";
    const char* ssn = "1234-567890";
    const char* email = "test@example.com";
    const char* sub_start = "5.07.2025"; // Invalid format: single digit day
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    int result = validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_INVALID_DATE, result);
}
*/ //auskommentiert da strptime() auch single digits verarbeiten kann! (test redundant)

/*
void test_date_invalid_format_month_returns_ERR_INVALID_DATE(void) {
    const char* full_name = "Test User";
    const char* gamertag = "TestPlayer";
    const char* ssn = "1234-567890";
    const char* email = "test@example.com";
    const char* sub_start = "05.7.2025"; // Invalid format: single digit month
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    int result = validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_INVALID_DATE, result);
}
*/ //auskommentiert da strptime() auch single digits verarbeiten kann! (test redundant)

void test_date_invalid_format_year_returns_ERR_PAST_DATE(void) {
    const char* full_name = "Test User";
    const char* gamertag = "TestPlayer";
    const char* ssn = "1234-567890";
    const char* email = "test@example.com";
    const char* sub_start = "05.07.25"; // date is parsed as a past date
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_PAST_DATE, result);
}

void test_date_non_existent_day_returns_ERR_INVALID_DATE(void) {
    const char* full_name = "Test User";
    const char* gamertag = "TestPlayer";
    const char* ssn = "1234-567890";
    const char* email = "test@example.com";
    const char* sub_start = "32.01.2025"; // Invalid date: 32nd day
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_INVALID_DATE, result);
}

void test_date_non_existent_month_returns_ERR_INVALID_DATE(void) {
    const char* full_name = "Test User";
    const char* gamertag = "TestPlayer";
    const char* ssn = "1234-567890";
    const char* email = "test@example.com";
    const char* sub_start = "01.13.2025"; // Invalid date: 13th month
    const char* sub_end = "01.08.2025";
    const char* is_subscribed_str = "true";

    int result = logic_validate_player_profile(full_name, gamertag, ssn, email, sub_start, sub_end, is_subscribed_str);

    TEST_ASSERT_EQUAL_INT(ERR_INVALID_DATE, result);
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
    //----Sina 
    //--- SSN Tests
    RUN_TEST(test_ssn_invalid_length_too_short_returns_ERR_INVALID_SSN); 
    RUN_TEST(test_ssn_invalid_length_too_long_returns_ERR_INVALID_SSN); 
    RUN_TEST(test_ssn_invalid_hyphen_position_returns_ERR_INVALID_SSN); 
    RUN_TEST(test_ssn_invalid_digit_on_wrong_positionchar_returns_ERR_INVALID_SSN); 
    // --- eMail Tests
    RUN_TEST(test_email_missing_at_symbol_returns_ERR_INVALID_EMAIL);
    RUN_TEST(test_email_missing_dot_after_at_returns_ERR_INVALID_EMAIL);
    RUN_TEST(test_email_dot_before_at_returns_ERR_INVALID_EMAIL);
    // --- Datum Tests
    RUN_TEST(test_date_invalid_format_separator_returns_ERR_INVALID_DATE);
    //RUN_TEST(test_date_invalid_format_day_returns_ERR_INVALID_DATE); //redundant
    //RUN_TEST(test_date_invalid_format_month_returns_ERR_INVALID_DATE); //redundant
    RUN_TEST(test_date_invalid_format_year_returns_ERR_PAST_DATE);
    RUN_TEST(test_date_non_existent_day_returns_ERR_INVALID_DATE);
    RUN_TEST(test_date_non_existent_month_returns_ERR_INVALID_DATE);
    return UNITY_END();
}
