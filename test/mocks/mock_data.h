#ifndef _MOCK_DATA_H
#define _MOCK_DATA_H

#include "unity.h"
#include "../inc/data/data.h"
#include "../../inc/error.h"
#include "../../cJSON/cJSON.h"

// === Initialisierung / Verifikation ===
void mock_data_Init(void);
void mock_data_Destroy(void);
void mock_data_Verify(void);

// === Kontrollvariablen für Mock-Verhalten (Teststeuerung) ===
extern int mock_data_get_all_users_called;
extern int mock_data_get_all_users_return;
extern cJSON* mock_data_get_all_users_users_out;

extern int mock_data_load_games_called;
extern int mock_data_load_games_return;
extern Game* mock_data_load_games_games_out;
extern int mock_data_load_games_count_out;

extern int mock_data_remove_expired_users_called;
extern int mock_data_remove_expired_users_return;
extern int mock_data_remove_expired_users_removed_out;

extern int mock_data_update_all_subscription_flags_called;
extern int mock_data_update_all_subscription_flags_return;

// ===== save_player_profile =====
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
);

int save_player_profile(
    const char* full_name,
    const char* gamertag,
    int player_hours,
    const char* ssn,
    const char* email,
    const char* sub_start,
    const char* sub_end,
    int is_subscribed
);

// ===== remove_player_profile =====
void remove_player_profile_ExpectAndReturn(
    const char* gamertag,
    int return_value
);

int remove_player_profile(const char* gamertag);

// ===== edit_player_profile =====
void edit_player_profile_ExpectAndReturn(
    const char* gamertag,
    const char* new_full_name,
    const char* new_ssn,
    const char* new_email,
    const char* sub_start,
    const char* sub_end,
    int is_subscribed,
    int return_value
);

int edit_player_profile(
    const char* gamertag,
    const char* new_full_name,
    const char* new_ssn,
    const char* new_email,
    const char* sub_start,
    const char* sub_end,
    int is_subscribed
);

// ===== read_player_profiles =====
void read_player_profiles_ExpectAndReturn(char** output, int return_value);
int read_player_profiles(char** output);

// ===== print_user_to_cli =====
void print_user_to_cli_Expect(void);
void print_user_to_cli(void);

// ===== weitere gemockte Datenfunktionen =====
int data_get_all_users(cJSON **users_out);
int data_remove_expired_users(int *removed_count);
int data_update_all_subscription_flags(void);

#endif // _MOCK_DATA_H