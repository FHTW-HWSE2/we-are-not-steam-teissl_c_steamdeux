#ifndef _MOCK_DATA_H
#define _MOCK_DATA_H

#include "unity.h"
#include "../inc/data/data.h"

// Initialisierung / Verifikation
void mock_data_Init(void);
void mock_data_Destroy(void);
void mock_data_Verify(void);

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

#endif
