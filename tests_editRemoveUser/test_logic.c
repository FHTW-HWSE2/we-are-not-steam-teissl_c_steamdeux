#include "unity/unity.h"
#include "logic.h"


void test_validate_player_profile_valid_data(void) {
    int result = validate_player_profile("Max Mustermann", "PlayerOne", "1234-567890", "max@example.com");
    TEST_ASSERT_EQUAL_INT(1, result);  // Erfolgreiches Speichern
}

void test_validate_player_profile_empty_name(void) {
    int result = validate_player_profile("", "PlayerOne", "1234-567890", "max@example.com");
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_validate_player_profile_invalid_ssn_format(void) {
    int result = validate_player_profile("Max", "PlayerOne", "abcd-567890", "max@example.com");
    TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_validate_player_profile_invalid_email_format(void) {
    int result = validate_player_profile("Max", "PlayerOne", "1234-567890", "maxexample.com");
    TEST_ASSERT_EQUAL_INT(-2, result);
}

void test_remove_user_logic_valid_gamertag(void) {
    int result = remove_user_logic("PixelKing"); 
    TEST_ASSERT_EQUAL_INT(1, result); 
}

void test_remove_user_logic_invalid_gamertag(void) {
    int result = remove_user_logic("NonExistingUser");
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_edit_user_logic_valid_data(void) {
    int result = edit_user_logic("PixelKing", "David Newname", "1234-567890", "david.new@example.com");
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_edit_user_logic_invalid_ssn(void) {
    int result = edit_user_logic("PixelKing", "David Newname", "wrongssn", "david.new@example.com");
    TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_edit_user_logic_invalid_email(void) {
    int result = edit_user_logic("PixelKing", "David Newname", "1234-567890", "invalid-email");
    TEST_ASSERT_EQUAL_INT(-2, result);
}

void test_edit_user_logic_empty_fields(void) {
    int result = edit_user_logic("PixelKing", "", "1234-567890", "david.new@example.com");
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_validate_player_profile_invalid_ssn_length(void) {
    int result = validate_player_profile(
        "Max Mustermann",
        "Player1",
        "1234567890",  // Kein '-' und zu kurz (10 statt 11)
        "max@example.com"
    );
    TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_validate_player_profile_invalid_email_no_at(void) {
    int result = validate_player_profile(
        "Max Mustermann",
        "Player1",
        "1234-567890",
        "maxexample.com"  // Kein @
    );
    TEST_ASSERT_EQUAL_INT(-2, result);
}

void test_validate_player_profile_invalid_email_no_dot_after_at(void) {
    int result = validate_player_profile(
        "Max Mustermann",
        "Player1",
        "1234-567890",
        "max@com"  // Kein . nach @
    );
    TEST_ASSERT_EQUAL_INT(-2, result);
}

void test_validate_player_profile_ssn_wrong_separator(void) {
    int result = validate_player_profile("Max", "Player1", "1234*67890", "max@example.com");
    TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_validate_player_profile_ssn_letter_instead_of_number(void) {
    int result = validate_player_profile("Max", "Player1", "12a4-67890", "max@example.com");
    TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_display_users_logic_returns_success(void) {
    int result = display_users_logic();
    TEST_ASSERT_EQUAL_INT(1, result);  // You expect success
}

void test_edit_user_logic_invalid_ssn_contains_letter(void) {
    int result = edit_user_logic(
        "Player1",           // gamertag
        "Max",               // new_name
        "123a-567890",       // ← invalid: 'a' instead of digit
        "max@example.com"    // email
    );

    TEST_ASSERT_EQUAL_INT(-1, result);
}
