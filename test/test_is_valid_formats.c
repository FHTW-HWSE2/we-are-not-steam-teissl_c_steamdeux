#include "unity.h"
#include "logic/logic.h"

void setUp(void) {}
void tearDown(void) {}

void test_is_valid_ssn_format(void) {
    TEST_ASSERT_TRUE(is_valid_ssn_format("1234-567890"));
    TEST_ASSERT_FALSE(is_valid_ssn_format("123-567890"));
    TEST_ASSERT_FALSE(is_valid_ssn_format("12345678901"));
    TEST_ASSERT_FALSE(is_valid_ssn_format("12345-67890"));
    TEST_ASSERT_FALSE(is_valid_ssn_format("12a4-567890"));
    TEST_ASSERT_FALSE(is_valid_ssn_format(NULL));
    TEST_ASSERT_FALSE(is_valid_ssn_format(""));
}

void test_is_valid_email_format(void) {
    TEST_ASSERT_TRUE(is_valid_email_format("foo@bar.com"));
    TEST_ASSERT_TRUE(is_valid_email_format("a@b.de"));
    TEST_ASSERT_FALSE(is_valid_email_format("foobar.com"));
    TEST_ASSERT_FALSE(is_valid_email_format("foo@barcom"));
    TEST_ASSERT_FALSE(is_valid_email_format("foo.bar@com"));
    TEST_ASSERT_TRUE(is_valid_email_format("foo@bar.baz.com"));
    TEST_ASSERT_FALSE(is_valid_email_format(NULL));
    TEST_ASSERT_FALSE(is_valid_email_format(""));
}

void test_is_valid_date_format(void) {
    TEST_ASSERT_TRUE(is_valid_date_format("01.01.2020"));
    TEST_ASSERT_TRUE(is_valid_date_format("31.12.1999"));
    TEST_ASSERT_FALSE(is_valid_date_format("1.01.2020"));
    TEST_ASSERT_FALSE(is_valid_date_format("01-01-2020"));
    TEST_ASSERT_FALSE(is_valid_date_format("01.01.20"));
    TEST_ASSERT_FALSE(is_valid_date_format("01.01.20200"));
    TEST_ASSERT_FALSE(is_valid_date_format("xx.xx.xxxx"));
    TEST_ASSERT_FALSE(is_valid_date_format(NULL));
    TEST_ASSERT_FALSE(is_valid_date_format(""));
}

void test_is_valid_alpha_format(void) {
    TEST_ASSERT_TRUE(is_valid_alpha_format("Max Mustermann"));
    TEST_ASSERT_TRUE(is_valid_alpha_format("Anna-Lena"));
    TEST_ASSERT_FALSE(is_valid_alpha_format("Max123"));
    TEST_ASSERT_FALSE(is_valid_alpha_format(""));
    TEST_ASSERT_FALSE(is_valid_alpha_format(NULL));
    TEST_ASSERT_FALSE(is_valid_alpha_format("Max@Home"));
}

void test_logic_is_only_spaces(void) {
    TEST_ASSERT_TRUE(logic_is_only_spaces("   "));
    TEST_ASSERT_FALSE(logic_is_only_spaces("a  "));
    TEST_ASSERT_FALSE(logic_is_only_spaces("  a "));
    TEST_ASSERT_FALSE(logic_is_only_spaces(""));
    TEST_ASSERT_FALSE(logic_is_only_spaces(NULL));
}

void test_logic_is_valid_date_format(void) {
    // Real test for wrapper
    TEST_ASSERT_TRUE(logic_is_valid_date_format("01.01.2020"));
    TEST_ASSERT_TRUE(logic_is_valid_date_format("31.12.1999"));
    TEST_ASSERT_FALSE(logic_is_valid_date_format("1.01.2020"));
    TEST_ASSERT_FALSE(logic_is_valid_date_format("01-01-2020"));
    TEST_ASSERT_FALSE(logic_is_valid_date_format("01.01.20"));
    TEST_ASSERT_FALSE(logic_is_valid_date_format("01.01.20200"));
    TEST_ASSERT_FALSE(logic_is_valid_date_format("xx.xx.xxxx"));
    TEST_ASSERT_FALSE(logic_is_valid_date_format(NULL));
    TEST_ASSERT_FALSE(logic_is_valid_date_format(""));
}

void test_logic_is_valid_alpha(void) {
    // Real test for wrapper
    TEST_ASSERT_TRUE(logic_is_valid_alpha("Max Mustermann"));
    TEST_ASSERT_TRUE(logic_is_valid_alpha("Anna-Lena"));
    TEST_ASSERT_FALSE(logic_is_valid_alpha("Max123"));
    TEST_ASSERT_FALSE(logic_is_valid_alpha(""));
    TEST_ASSERT_FALSE(logic_is_valid_alpha(NULL));
    TEST_ASSERT_FALSE(logic_is_valid_alpha("Max@Home"));
}

void test_logic_is_valid_email(void) {
    // Real test for wrapper
    TEST_ASSERT_TRUE(logic_is_valid_email("foo@bar.com"));
    TEST_ASSERT_TRUE(logic_is_valid_email("a@b.de"));
    TEST_ASSERT_FALSE(logic_is_valid_email("foobar.com"));
    TEST_ASSERT_FALSE(logic_is_valid_email("foo@barcom"));
    TEST_ASSERT_FALSE(logic_is_valid_email("foo.bar@com"));
    TEST_ASSERT_TRUE(logic_is_valid_email("foo@bar.baz.com"));
    TEST_ASSERT_FALSE(logic_is_valid_email(NULL));
    TEST_ASSERT_FALSE(logic_is_valid_email(""));
}

void test_logic_is_valid_ssn(void) {
    // Real test for wrapper
    TEST_ASSERT_TRUE(logic_is_valid_ssn("1234-567890"));
    TEST_ASSERT_FALSE(logic_is_valid_ssn("123-567890"));
    TEST_ASSERT_FALSE(logic_is_valid_ssn("12345678901"));
    TEST_ASSERT_FALSE(logic_is_valid_ssn("12345-67890"));
    TEST_ASSERT_FALSE(logic_is_valid_ssn("12a4-567890"));
    TEST_ASSERT_FALSE(logic_is_valid_ssn(NULL));
    TEST_ASSERT_FALSE(logic_is_valid_ssn(""));
}

// --- Presentation stubs for linker (signatures must match header) ---
const char* presentation_get_report_title(void) { return ""; }
const char* presentation_get_report_description(void) { return ""; }
const char* presentation_get_report_date(void) { return ""; }
void presentation_error_empty_fields(void) {}
void presentation_error_date(void) {}
void presentation_error_unknown(void) {}
void presentation_error_storage(void) {}
void presentation_error_email(void) {}
void presentation_error_ssn(void) {}
void presentation_error_gamertag_empty(void) {}
void presentation_error_full_name_empty(void) {}
void presentation_error_full_name_format(void) {}
void presentation_choose_subscription_start(void) {}
void presentation_choose_subscription_model(void) {}
void presentation_error_past_date(void) {}
void presentation_error_sub_status(void) {}
void presentation_print_welcome_banner(void) {}
void presentation_display_main_menu(void) {}
int presentation_get_main_menu_choice(void) { return 0; }
void presentation_show_startup_info(int a, int b) {}
void presentation_display_user_menu(void) {}
int presentation_get_user_menu_choice(void) { return 0; }
void presentation_get_full_name(char *buffer, size_t size) { if (buffer && size) buffer[0] = 0; }
void presentation_get_gamertag(char *buffer, size_t size) { if (buffer && size) buffer[0] = 0; }
void presentation_get_ssn(char *buffer, size_t size) { if (buffer && size) buffer[0] = 0; }
void presentation_get_email(char *buffer, size_t size) { if (buffer && size) buffer[0] = 0; }
void presentation_get_subscription_start(char *buffer, size_t size) { if (buffer && size) buffer[0] = 0; }
void presentation_get_subscription_duration(char *buffer, size_t size) { if (buffer && size) buffer[0] = 0; }
void presentation_show_message(const char *msg) {}
void presentation_show_error(const char *message) {}
void presentation_display_users_formatted(char **lines, int count) {}
void presentation_display_error(const char *message) {}
void presentation_welcome_add_user(void) {}
void presentation_success_report_saved(void) {}
void presentation_error_critical_startup(void) {}
void presentation_info_exiting(void) {}
void presentation_start_game_management_menu(void) {}
void start_simulation(void) {}
void presentation_error_invalid_option(void) {}
void presentation_info_returning_to_main_menu(void) {}
void presentation_remove_user(void) {}
void presentation_info_add_report_selected(void) {}
void presentation_info_rank_top_users_selected(void) {}
void presentation_show_top_users_terminal(void) {}
void presentation_info_generate_player_report_selected(void) {}
void presentation_generate_top_users_file(void) {}
void presentation_success_user_added(void) {}
void presentation_error_gamertag_empty_edit(void) {}
void presentation_info_enter_full_name_edit(void) {}
void presentation_error_full_name_format_edit(void) {}
void presentation_info_enter_ssn_edit(void) {}
void presentation_error_ssn_format_edit(void) {}
void presentation_info_enter_email_edit(void) {}
void presentation_error_email_format_edit(void) {}
void presentation_info_enter_start_date_edit(void) {}
void presentation_error_date_format_edit(void) {}
void presentation_info_enter_end_date_edit(void) {}
void presentation_info_enter_sub_status_edit(void) {}
void presentation_error_sub_status_edit(void) {}
void presentation_success_user_edited(void) {}
void presentation_error_user_not_found(void) {}
void presentation_display_game_management_menu(void) {}
int presentation_get_game_menu_choice(void) { return 0; }
void presentation_get_game_title(char *buffer, size_t size) { if (buffer && size) buffer[0] = 0; }
void presentation_get_game_description(char *buffer, size_t size) { if (buffer && size) buffer[0] = 0; }
void presentation_get_game_version(char *buffer, size_t size) { if (buffer && size) buffer[0] = 0; }
void presentation_get_game_mode(char *buffer, size_t size) { if (buffer && size) buffer[0] = 0; }
void presentation_get_game_id_to_edit(char *buffer, size_t size) { if (buffer && size) buffer[0] = 0; }
void presentation_get_new_game_title(char *buffer, size_t size) { if (buffer && size) buffer[0] = 0; }
void presentation_get_game_id_to_delete(char *buffer, size_t size) { if (buffer && size) buffer[0] = 0; }

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_is_valid_ssn_format);
    RUN_TEST(test_is_valid_email_format);
    RUN_TEST(test_is_valid_date_format);
    RUN_TEST(test_is_valid_alpha_format);
    RUN_TEST(test_logic_is_only_spaces);
    RUN_TEST(test_logic_is_valid_date_format);
    RUN_TEST(test_logic_is_valid_alpha);
    RUN_TEST(test_logic_is_valid_email);
    RUN_TEST(test_logic_is_valid_ssn);
    return UNITY_END();
}
