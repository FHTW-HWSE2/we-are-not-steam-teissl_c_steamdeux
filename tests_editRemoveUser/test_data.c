#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "../data/data.h"  // Header for remove_player_profile and edit_player_profile
#include <unistd.h>


void test_read_player_profiles_returns_output(void) {
    char* output = NULL;
    int result = read_player_profiles(&output);

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_NOT_NULL(output);
    TEST_ASSERT_TRUE(strstr(output, "Gamertag:") != NULL);

    free(output);
}

void test_save_player_profile_basic(void) {
    int result = save_player_profile(
        "Max Mustermann",     // full_name
        "Player123",          // gamertag
        100,                  // player_hours
        "123-45-6789",        // ssn
        "max@example.com"     // email
    );

    TEST_ASSERT_EQUAL_INT(1, result);  // Erfolg wird mit 1 signalisiert
}


// Initial JSON content for users.json (array of player profiles)
static const char *initial_json =
"[{\n"
"                \"full_name\":    \"Michael Carter\",\n"
"                \"gamertag\":     \"NightHawk22\",\n"
"                \"player_hours\": 1500,\n"
"                \"ssn\":  \"9021 25061989\",\n"
"                \"email\":        \"michael.carter@example.com\"\n"
"        }, {\n"
"                \"full_name\":    \"Samantha Lee\",\n"
"                \"gamertag\":     \"CyberSam\",\n"
"                \"player_hours\": 785,\n"
"                \"ssn\":  \"3456 04071992\",\n"
"                \"email\":        \"samantha.lee@example.com\"\n"
"        }, {\n"
"                \"full_name\":    \"David Newname\",\n"
"                \"gamertag\":     \"PixelKing\",\n"
"                \"player_hours\": 320,\n"
"                \"ssn\":  \"1234-567890\",\n"
"                \"email\":        \"david.new@example.com\"\n"
"        }, {\n"
"                \"full_name\":    \"Jessica Roberts\",\n"
"                \"gamertag\":     \"LunaGamerX\",\n"
"                \"player_hours\": 1275,\n"
"                \"ssn\":  \"1234 30091991\",\n"
"                \"email\":        \"jessica.roberts@example.com\"\n"
"        }, {\n"
"                \"full_name\":    \"Kevin Thompson\",\n"
"                \"gamertag\":     \"BlazeStorm\",\n"
"                \"player_hours\": 990,\n"
"                \"ssn\":  \"7890 11041985\",\n"
"                \"email\":        \"kevin.thompson@example.com\"\n"
"        }, {\n"
"                \"full_name\":    \"Olivia Brown\",\n"
"                \"gamertag\":     \"NeonKnight\",\n"
"                \"player_hours\": 455,\n"
"                \"ssn\":  \"2345 26022000\",\n"
"                \"email\":        \"olivia.brown@example.com\"\n"
"        }, {\n"
"                \"full_name\":    \"Daniel Walker\",\n"
"                \"gamertag\":     \"SkyRider77\",\n"
"                \"player_hours\": 1100,\n"
"                \"ssn\":  \"8901 17051988\",\n"
"                \"email\":        \"daniel.walker@example.com\"\n"
"        }, {\n"
"                \"full_name\":    \"Emma Scott\",\n"
"                \"gamertag\":     \"MysticEcho\",\n"
"                \"player_hours\": 632,\n"
"                \"ssn\":  \"4567 01082001\",\n"
"                \"email\":        \"emma.scott@example.com\"\n"
"        }, {\n"
"                \"full_name\":    \"James Wilson\",\n"
"                \"gamertag\":     \"IronFury\",\n"
"                \"player_hours\": 275,\n"
"                \"ssn\":  \"6780 05061996\",\n"
"                \"email\":        \"james.wilson@example.com\"\n"
"        }]";

// This function is called before each test – it resets users.json to the initial content.
void setUp(void) {
    FILE *file = fopen("../users.json", "w");
    TEST_ASSERT_NOT_NULL_MESSAGE(file, "Failed to open users.json in setUp");
    fputs(initial_json, file);
    fclose(file);
}

// This function is called after each test – not used here, but required by Unity.
void tearDown(void) {
    // No cleanup needed post-test (file reset in setUp, and tests handle their own memory freeing).
}

// Test: Removing an existing player profile should succeed.
void test_remove_player_profile_success(void) {
    // Attempt to remove a profile with gamertag "PixelKing", which exists in the initial JSON.
    int result = remove_player_profile("PixelKing");
    TEST_ASSERT_EQUAL_INT(1, result);  // Should return 1 on success.

    // Verify that "PixelKing" is actually removed from the file.
    FILE *file = fopen("../users.json", "r");
    TEST_ASSERT_NOT_NULL_MESSAGE(file, "Failed to open users.json for verification");
    // Read file contents into a buffer
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);
    // The removed gamertag should no longer be present in the JSON text.
    TEST_ASSERT_NULL_MESSAGE(strstr(buffer, "PixelKing"), "Gamertag still found in JSON after removal");
    free(buffer);
}

// Test: Removing a non-existent gamertag should fail (return 0) and not modify the file.
void test_remove_player_profile_nonexistent_gamertag(void) {
    // "NonExistentGT" is not present in the initial JSON data.
    int result = remove_player_profile("NonExistentGT");
    TEST_ASSERT_EQUAL_INT(0, result);  // Should return 0 when gamertag is not found.
    // (No further verification needed here; the file remains unchanged since removal failed.)
}

// Test: Editing an existing player profile should succeed and update the JSON.
void test_edit_player_profile_success(void) {
    // Edit the profile with gamertag "PixelKing" (exists in initial data).
    const char *new_name  = "Edited Name";
    const char *new_ssn   = "0000 000000";
    const char *new_email = "edited.email@example.com";
    int result = edit_player_profile("PixelKing", new_name, new_ssn, new_email);
    TEST_ASSERT_EQUAL_INT(1, result);  // Should return 1 on successful edit.

    // Verify that the JSON file now contains the new values and no longer contains the old values.
    FILE *file = fopen("../users.json", "r");
    TEST_ASSERT_NOT_NULL_MESSAGE(file, "Failed to open users.json for verification");
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);
    // Check that the new full_name, SSN, and email appear in the file.
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(buffer, new_name),  "New full_name not found in JSON after edit");
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(buffer, new_ssn),   "New SSN not found in JSON after edit");
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr(buffer, new_email), "New email not found in JSON after edit");
    // Check that the old values are no longer present.
    TEST_ASSERT_NULL_MESSAGE(strstr(buffer, "David Newname"),             "Old full_name still present after edit");
    TEST_ASSERT_NULL_MESSAGE(strstr(buffer, "1234-567890"),               "Old SSN still present after edit");
    TEST_ASSERT_NULL_MESSAGE(strstr(buffer, "david.new@example.com"),     "Old email still present after edit");
    free(buffer);
}

// Test: Editing a non-existent gamertag should fail (return 0) and not change the JSON.
void test_edit_player_profile_nonexistent_gamertag(void) {
    // "GhostTag" does not exist in initial JSON.
    int result = edit_player_profile("GhostTag", "Any Name", "1111-111111", "ghost@example.com");
    TEST_ASSERT_EQUAL_INT(0, result);  // Should return 0 for non-existent gamertag.
    // (No further verification; file content remains unchanged on failure.)
}



void test_read_player_profiles_file_missing(void) {
    // Datei vorübergehend umbenennen
    rename("../users.json", "../users_backup.json");

    char* output = NULL;
    int result = read_player_profiles(&output);

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_NOT_NULL(output);
    TEST_ASSERT_TRUE(strstr(output, "No users found") != NULL);

    free(output);

    // Datei zurückbenennen
    rename("../users_backup.json", "../users.json");
}

void test_read_player_profiles_invalid_json_not_array(void) {
    // Schritt 1: Lege eine Datei an mit ungültiger Array-Struktur
    FILE* file = fopen("../users.json", "w");
    fprintf(file, "{\"someKey\": \"someValue\"}");  // kein Array
    fclose(file);

    char* output = NULL;
    int result = read_player_profiles(&output);

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_NOT_NULL(output);
    TEST_ASSERT_TRUE(strstr(output, "invalid JSON") != NULL);

    free(output);
}

void test_read_player_profiles_empty_array(void) {
    FILE* file = fopen("../users.json", "w");
    fprintf(file, "[]");  // leeres Array
    fclose(file);

    char* output = NULL;
    int result = read_player_profiles(&output);

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_NOT_NULL(output);
    TEST_ASSERT_TRUE(strstr(output, "empty array") != NULL);

    free(output);
}

void test_edit_user_logic_invalid_json_format(void) {
    FILE* file = fopen("../users.json", "w");
    fprintf(file, "INVALID JSON");  // absichtlich unparsebar
    fclose(file);

    int result = edit_user_logic("Player1", "Max", "1234-567890", "max@example.com");

    TEST_ASSERT_EQUAL_INT(0, result);  // sollte wegen Parse-Fehler abbrechen
}

void test_remove_user_logic_invalid_json(void) {
    FILE* file = fopen("../users.json", "w");
    fprintf(file, "12345");  // kein gültiges Array
    fclose(file);

    int result = remove_user_logic("Player1");

    TEST_ASSERT_EQUAL_INT(0, result);  // sollte wegen Parse-Fehler abbrechen
}
