#include "mock_data.h"
#include "unity.h"
#include <string.h>
#include "../src/data/game.h"
#include "cJSON.h"

static int expected_return_value;
static int function_called;

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

// Updated function with data_ prefix
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

// Dummy-Implementierung für Test-Builds, damit Linker zufrieden ist
int data_get_all_users(cJSON **users_out) {
    *users_out = cJSON_CreateArray();
    return 0;
}

// Missing data_load_games function - updated signature
int data_load_games(const char *filename, Game **games_out, int *count_out) {
    (void)filename;
    *games_out = NULL;
    *count_out = 0;
    return 0; // Mock returns success
}
