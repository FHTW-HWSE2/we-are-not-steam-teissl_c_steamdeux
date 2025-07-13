#include "mock_data.h"
#include "unity.h"
#include <string.h>
#include "../src/data/game.h"
#include "cJSON.h"

static int expected_return_value;
static int function_called;

int mock_data_load_games_called = 0;
int mock_data_load_games_return = ERR_SUCCESS;
Game* mock_data_load_games_games_out = NULL;
int mock_data_load_games_count_out = 0;

// === Kontrollvariablen für Tests ===
int mock_data_get_all_users_called = 0;
int mock_data_get_all_users_return = ERR_SUCCESS;
cJSON* mock_data_get_all_users_users_out = NULL;

int mock_data_remove_expired_users_called = 0;
int mock_data_remove_expired_users_return = ERR_SUCCESS;
int mock_data_remove_expired_users_removed_out = 0;

int mock_data_update_all_subscription_flags_called = 0;
int mock_data_update_all_subscription_flags_return = 0;

// Kontrollvariable für das Rückgabeverhalten von data_save_games
int mock_data_save_games_return = ERR_SUCCESS;

// === Setup/Teardown ===
void mock_data_Init(void) {
    expected_return_value = 0;
    function_called = 0;
}

void mock_data_Destroy(void) {}

void mock_data_Verify(void) {
    // Optional: Nur für save_player_profile enforced
}

// ==== save_player_profile ====
void save_player_profile_ExpectAndReturn(
    const char* full_name,
    const char* gamertag,
    int player_hours,
    const char* ssn,
    const char* email,
    const char* sub_start,
    const char* sub_end,
    int is_subscribed,
    int return_value
) {
    (void)full_name; (void)gamertag; (void)player_hours; (void)ssn;
    (void)email; (void)sub_start; (void)sub_end; (void)is_subscribed;
    expected_return_value = return_value;
    function_called = 0;
}

int save_player_profile(
    const char* full_name,
    const char* gamertag,
    int player_hours,
    const char* ssn,
    const char* email,
    const char* sub_start,
    const char* sub_end,
    int is_subscribed
) {
    (void)full_name; (void)gamertag; (void)player_hours; (void)ssn;
    (void)email; (void)sub_start; (void)sub_end; (void)is_subscribed;
    function_called = 1;
    return expected_return_value;
}

int data_save_player_profile(
    const char* full_name,
    const char* gamertag,
    int player_hours,
    const char* ssn,
    const char* email,
    const char* sub_start,
    const char* sub_end,
    int is_subscribed
) {
    (void)full_name; (void)gamertag; (void)player_hours; (void)ssn;
    (void)email; (void)sub_start; (void)sub_end; (void)is_subscribed;
    function_called = 1;
    return expected_return_value;
}

// ==== remove_player_profile ====
void remove_player_profile_ExpectAndReturn(const char* gamertag, int return_value) {
    (void)gamertag;
    expected_return_value = return_value;
}

int remove_player_profile(const char* gamertag) {
    (void)gamertag;
    return expected_return_value;
}

int data_remove_player_profile(const char* gamertag) {
    (void)gamertag;
    return expected_return_value;
}

// ==== edit_player_profile ====
void edit_player_profile_ExpectAndReturn(
    const char* gamertag,
    const char* new_full_name,
    const char* new_ssn,
    const char* new_email,
    const char* sub_start,
    const char* sub_end,
    int is_subscribed,
    int return_value
) {
    (void)gamertag; (void)new_full_name; (void)new_ssn;
    (void)new_email; (void)sub_start; (void)sub_end; (void)is_subscribed;
    expected_return_value = return_value;
}

int edit_player_profile(const char* gamertag,
                        const char* new_full_name,
                        const char* new_ssn,
                        const char* new_email,
                        const char* sub_start,
                        const char* sub_end,
                        int is_subscribed) {
    (void)gamertag; (void)new_full_name; (void)new_ssn;
    (void)new_email; (void)sub_start; (void)sub_end; (void)is_subscribed;
    return expected_return_value;
}

int data_edit_player_profile(const char* gamertag,
                        const char* new_full_name,
                        const char* new_ssn,
                        const char* new_email,
                        const char* sub_start,
                        const char* sub_end,
                        int is_subscribed) {
    (void)gamertag; (void)new_full_name; (void)new_ssn;
    (void)new_email; (void)sub_start; (void)sub_end; (void)is_subscribed;
    return expected_return_value;
}

// ==== read_player_profiles ====
void read_player_profiles_ExpectAndReturn(char** output, int return_value) {
    (void)output;
    expected_return_value = return_value;
}

int read_player_profiles(char** output) {
    *output = strdup("Mocked profile output.\n");
    return expected_return_value;
}

// ==== print_user_to_cli ====
void print_user_to_cli_Expect(void) {
    function_called = 0;
}

void print_user_to_cli(void) {
    function_called = 1;
}

// ==== data_get_all_users (erweitert für Tests) ====
int data_get_all_users(cJSON **users_out) {
    mock_data_get_all_users_called++;
    if (users_out) {
        *users_out = mock_data_get_all_users_users_out;
    }
    return mock_data_get_all_users_return;
}

// ==== Reports ====
int data_save_report(cJSON *report) {
    (void)report;
    return ERR_SUCCESS;
}

// ==== Spiele ====
int data_save_games(const char *filename, Game games[], int game_count) {
    (void)filename;
    (void)games;
    (void)game_count;
    return mock_data_save_games_return;
}

int data_load_games(const char *filename, Game **games_out, int *count_out) {
    (void)filename;
    mock_data_load_games_called++;
    if (games_out) *games_out = mock_data_load_games_games_out;
    if (count_out) *count_out = mock_data_load_games_count_out;
    return mock_data_load_games_return;
}

// ==== Abo-Status aktualisieren ====
int data_update_all_subscription_flags(void) {
    mock_data_update_all_subscription_flags_called++;
    return mock_data_update_all_subscription_flags_return;
}

// ==== Expired Users entfernen ====
int data_remove_expired_users(int *removed_count_out) {
    mock_data_remove_expired_users_called++;
    if (removed_count_out) *removed_count_out = mock_data_remove_expired_users_removed_out;
    return mock_data_remove_expired_users_return;
}