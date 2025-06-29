#include "unity/unity.h"

// "extern" sagt dem Compiler, dass diese Funktion woanders existiert und definiert ist,
// daher brauchen wir keine Header-Datei (wie z.B. test_data.h) für die Tests.
// Forward declarations for test_logic.c
extern void test_remove_user_logic_valid_gamertag(void);
extern void test_remove_user_logic_invalid_gamertag(void);
extern void test_edit_user_logic_valid_data(void);
extern void test_edit_user_logic_invalid_ssn(void);
extern void test_edit_user_logic_invalid_email(void);
extern void test_edit_user_logic_empty_fields(void);
extern void test_validate_player_profile_valid_data(void);
extern void test_validate_player_profile_empty_name(void);
extern void test_validate_player_profile_invalid_ssn_format(void);
extern void test_validate_player_profile_invalid_email_format(void);
extern void test_validate_player_profile_invalid_ssn_length(void); // added after gcoverage testing
extern void test_validate_player_profile_invalid_email_no_at(void);
extern void test_validate_player_profile_invalid_email_no_dot_after_at(void); // added after gcoverage testing
extern void test_validate_player_profile_ssn_wrong_separator(void); // added after gcoverage testing
extern void test_validate_player_profile_ssn_letter_instead_of_number(void); // added after gcoverage testing
extern void test_display_users_logic_returns_success(void); // added after gcoverage testing
extern void test_edit_user_logic_invalid_ssn_contains_letter(void); // added after gcoverage testing

// Forward declarations for test_data.c
extern void test_read_player_profiles_returns_output(void);
extern void test_save_player_profile_basic(void);
extern void test_remove_player_profile_success(void);
extern void test_remove_player_profile_nonexistent_gamertag(void);
extern void test_edit_player_profile_success(void);
extern void test_edit_player_profile_nonexistent_gamertag(void);
extern void test_read_player_profiles_file_missing(void); // added after gcoverage testing
extern void test_read_player_profiles_invalid_json_not_array(void); // added after gcoverage testing
extern void test_read_player_profiles_empty_array(void); // added after gcoverage testing
extern void test_edit_user_logic_invalid_json_format(void); // added after gcoverage testing
extern void test_remove_user_logic_invalid_json(void); // added after gcoverage testing

int main(void) {
    UNITY_BEGIN();

    // Tests from test_logic.c
    RUN_TEST(test_edit_user_logic_invalid_ssn_contains_letter); // added after gcoverage testing
    RUN_TEST(test_display_users_logic_returns_success); // added after gcoverage testing
    RUN_TEST(test_validate_player_profile_ssn_letter_instead_of_number); // added after gcoverage testing
    RUN_TEST(test_validate_player_profile_ssn_wrong_separator); // added after gcoverage testing
    RUN_TEST(test_validate_player_profile_invalid_email_no_dot_after_at); // added after gcoverage testing
    RUN_TEST(test_validate_player_profile_invalid_email_no_at); // added after gcoverage testing
    RUN_TEST(test_validate_player_profile_valid_data); // added after gcoverage testing
    RUN_TEST(test_validate_player_profile_empty_name); // added after gcoverage testing
    RUN_TEST(test_validate_player_profile_invalid_ssn_format); // added after gcoverage testing
    RUN_TEST(test_validate_player_profile_invalid_email_format); // added after gcoverage testing
    RUN_TEST(test_remove_user_logic_valid_gamertag);
    RUN_TEST(test_remove_user_logic_invalid_gamertag);
    RUN_TEST(test_edit_user_logic_valid_data);
    RUN_TEST(test_edit_user_logic_invalid_ssn);
    RUN_TEST(test_edit_user_logic_invalid_email);
    RUN_TEST(test_edit_user_logic_empty_fields);
    RUN_TEST(test_validate_player_profile_invalid_ssn_length); // added after gcoverage testing

    // Tests from test_data.c
    RUN_TEST(test_remove_user_logic_invalid_json); // added after gcoverage testing
    RUN_TEST(test_edit_user_logic_invalid_json_format); // added after gcoverage testing
    RUN_TEST(test_read_player_profiles_empty_array); // added after gcoverage testing
    RUN_TEST(test_read_player_profiles_invalid_json_not_array); // added after gcoverage testing
    RUN_TEST(test_read_player_profiles_file_missing); // added after gcoverage testing
    RUN_TEST(test_read_player_profiles_returns_output); // added after gcoverage testing
    RUN_TEST(test_save_player_profile_basic); // added after gcoverage testing
    RUN_TEST(test_remove_player_profile_success);
    RUN_TEST(test_remove_player_profile_nonexistent_gamertag);
    RUN_TEST(test_edit_player_profile_success);
    RUN_TEST(test_edit_player_profile_nonexistent_gamertag);
    
    return UNITY_END();
}
