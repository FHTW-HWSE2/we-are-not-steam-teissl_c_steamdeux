#define _XOPEN_SOURCE // Aktiviert POSIX-kompatible Funktionen, Für remove_expired_users Funktion benötigt. Für Funktionen wie strptime()
#define _GNU_SOURCE // Aktiviert GNU-spezifische Funktionen, Für remove_expired_users Funktion. Für Funktionen wie strdup()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/data/data.h"
#include "cJSON.h"
#include "save_games.h"
#include "load_games.h"
#include "data.h" // Hardcoded path from project folder, defined in data.h
#include <time.h> // Für remove_expired_users Funktion
#include "../inc/error.h" // Für Fehlercodes

#define REPORTS_FILE "../reports.json"


cJSON *data_load_reports(void) {
    FILE *file = fopen(REPORTS_FILE, "r");
    if (file == NULL) {
        // Datei existiert nicht -> neue leere JSON-Array zurückgeben
        return cJSON_CreateArray();
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *data = (char *)malloc(length + 1);
    if (!data) {
        fclose(file);
        return NULL;
    }

    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    cJSON *reports = cJSON_Parse(data);
    free(data);

    if (!reports) {
        return cJSON_CreateArray(); // Falls Datei korrupt ist -> leeres Array zurück
    }

    return reports;
}

int data_save_report(cJSON *report) {
    cJSON *reports = data_load_reports();
    if (!reports) {
        return ERR_STORAGE_FAILURE;
    }

    cJSON_AddItemToArray(reports, report);

    char *string = cJSON_Print(reports);
    if (!string) {
        cJSON_Delete(reports);
        return ERR_STORAGE_FAILURE;
    }

    FILE *file = fopen(REPORTS_FILE, "w");
    if (!file) {
        free(string);
        cJSON_Delete(reports);
        return ERR_STORAGE_FAILURE;
    }

    fprintf(file, "%s", string);
    fclose(file);

    free(string);
    cJSON_Delete(reports);

    return ERR_SUCCESS;
}

// hardcoded path from project folder --> in data.h verschoben und definiert, damit es kompiliert

int save_player_profile(const char* full_name, const char* gamertag, int player_hours, const char* ssn, const char* email, const char* sub_start, const char* sub_end, int is_subscribed){
    FILE *file = fopen(USERS_JSON_PATH, "r");

    // cJSON struct is defined in cJSON.h, represents a JSON element: either object {}, array [], number or string
    cJSON *user_array = NULL;

    // check if the file exists and load existing data from JSON file
    if(file){
        // get file length by going from end to beginning, offset (how many bytes to move) = 0
        fseek(file, 0, SEEK_END);

        // store number of bytes in the file, each char is one byte
        int length = ftell(file);

        fseek(file, 0, SEEK_SET);

        // allocate data for file content, +1 for null terminator. Read from file and store in memory
        char *data = malloc(length + 1);
        fread(data, 1, length, file);
        data[length] = '\0';
        fclose(file);
        
        // parse data in allocated memory, then free it
        user_array = cJSON_Parse(data);
        free(data);

        if (!user_array || !cJSON_IsArray(user_array)) {
            if (user_array) cJSON_Delete(user_array);  // Nur löschen, wenn es nicht NULL ist
            user_array = cJSON_CreateArray();  // Leeres neues Array anlegen
        }
    }

    if (!user_array) {
        return ERR_STORAGE_FAILURE; // JSON-Initialisierung fehlgeschlagen
    }

    // create new user object
    cJSON *user = cJSON_CreateObject();
    cJSON_AddStringToObject(user, "full_name", full_name);
    cJSON_AddStringToObject(user, "gamertag", gamertag);
    cJSON_AddNumberToObject(user, "player_hours", player_hours);
    cJSON_AddStringToObject(user, "ssn", ssn);
    cJSON_AddStringToObject(user, "email", email);
    cJSON_AddStringToObject(user, "subscription_start_date", sub_start);
    cJSON_AddStringToObject(user, "subscription_end_date", sub_end);
    cJSON_AddBoolToObject(user, "is_subscribed", is_subscribed);

    // append user oject to end of the array
    cJSON_AddItemToArray(user_array, user);

    // convert user array to string with cJSON_Print and save pointer
    char *json_text = cJSON_Print(user_array);

    file = fopen(USERS_JSON_PATH, "w");

    if (!file) {
        free(json_text);
        cJSON_Delete(user_array);
        return ERR_STORAGE_FAILURE; // Schreibfehler
    }

    // overwrite json file
    fputs(json_text, file);

    // ensure that data is written to disk
    fflush(file);  
    fclose(file);

    // free memory and clean up
    free(json_text);
    cJSON_Delete(user_array);

    // when successful
    return ERR_SUCCESS;
}


void print_user_to_cli(void){
    printf("Displaying all users...\n\n");
    system("cat ../users.json");
    printf("\n");
}



int read_player_profiles(char** output){
    FILE *file = fopen(USERS_JSON_PATH, "r");
    if(!file){
        *output = strdup("No users found (file missing).\n");
        return ERR_SUCCESS; // Still "succeeds" but with a message
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = malloc(length + 1);
    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    cJSON *user_array = cJSON_Parse(data);
    free(data);

    if (!user_array || !cJSON_IsArray(user_array)){
        cJSON_Delete(user_array);
        *output = strdup("No users found (invalid JSON).\n");
        return ERR_SUCCESS;
    }

    int array_size = cJSON_GetArraySize(user_array);
    if (array_size == 0) {
        cJSON_Delete(user_array);
        *output = strdup("No users found (empty array).\n");
        return ERR_SUCCESS;
    }

    // Calculate total length needed
    size_t total_len = 0;
    for(int i = 0; i < array_size; i++){
        cJSON *item = cJSON_GetArrayItem(user_array, i);
        char *item_str = cJSON_Print(item);
        total_len += strlen(item_str) + 50; // Extra space for formatting
        free(item_str);
    }
    total_len += 1; // Null terminator

    char *result = malloc(total_len);
    result[0] = '\0';

    for (int i = 0; i < array_size; i++) {
        cJSON *item = cJSON_GetArrayItem(user_array, i);
        cJSON *full_name = cJSON_GetObjectItem(item, "full_name");
        cJSON *gamertag = cJSON_GetObjectItem(item, "gamertag");
        cJSON *player_hours = cJSON_GetObjectItem(item, "player_hours");
        cJSON *ssn = cJSON_GetObjectItem(item, "ssn");
        cJSON *email = cJSON_GetObjectItem(item, "email");
        cJSON *sub_start = cJSON_GetObjectItem(item, "subscription_start_date");
        cJSON *sub_end = cJSON_GetObjectItem(item, "subscription_end_date");
        cJSON *is_sub = cJSON_GetObjectItem(item, "is_subscribed");

        const char *subscribed_status;
        if (is_sub->valueint){
            subscribed_status = "Yes";
        } else{
            subscribed_status = "No";
        }

        char buffer[512];
        snprintf(buffer, sizeof(buffer),
                "User %d:\n"
                "  %-20s: %s\n"
                "  %-20s: %s\n"
                "  %-20s: %d\n"
                "  %-20s: %s\n"
                "  %-20s: %s\n"
                "  %-20s: %s\n"
                "  %-20s: %s\n"
                "  %-20s: %s\n\n",
                i + 1,
                "Full Name", full_name->valuestring,
                "Gamertag", gamertag->valuestring,
                "Player Hours", player_hours->valueint,
                "SSN", ssn->valuestring,
                "E-Mail", email->valuestring,
                "Subscription Start", sub_start->valuestring,
                "Subscription End", sub_end->valuestring,
                "Currently Subscribed", subscribed_status
            );
        strncat(result, buffer, total_len - strlen(result) - 1);
    }

    cJSON_Delete(user_array);
    *output = result;
    return ERR_SUCCESS;
}

int remove_player_profile(const char* gamertag) {
    FILE *file = fopen(USERS_JSON_PATH, "r");
    if (!file) return ERR_STORAGE_FAILURE;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = malloc(length + 1);
    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    cJSON *user_array = cJSON_Parse(data);
    free(data);
    if (!user_array || !cJSON_IsArray(user_array)) {
        cJSON_Delete(user_array);
        return ERR_STORAGE_FAILURE;
    }

    int array_size = cJSON_GetArraySize(user_array);
    for (int i = 0; i < array_size; i++) {
        cJSON *item = cJSON_GetArrayItem(user_array, i);
        cJSON *gtag = cJSON_GetObjectItem(item, "gamertag");
        if (gtag && strcmp(gtag->valuestring, gamertag) == 0) {
            cJSON_DeleteItemFromArray(user_array, i);

            file = fopen(USERS_JSON_PATH, "w");
            char *new_json = cJSON_Print(user_array);
            fputs(new_json, file);
            fflush(file);
            fclose(file);

            free(new_json);
            cJSON_Delete(user_array);
            return ERR_SUCCESS; // Success
        }
    }

    cJSON_Delete(user_array);
    return ERR_USER_NOT_FOUND; // Not found
}

int edit_player_profile(const char* gamertag, const char* new_full_name, const char* new_ssn, const char* new_email, const char* sub_start, const char* sub_end, int is_subscribed) {
    FILE *file = fopen(USERS_JSON_PATH, "r");
    if (!file) return ERR_STORAGE_FAILURE;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = malloc(length + 1);
    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    cJSON *user_array = cJSON_Parse(data);
    free(data);
    if (!user_array || !cJSON_IsArray(user_array)) {
        cJSON_Delete(user_array);
        return ERR_STORAGE_FAILURE;
    }

    int array_size = cJSON_GetArraySize(user_array);
    int found = 0;

    for (int i = 0; i < array_size; i++) {
        cJSON *item = cJSON_GetArrayItem(user_array, i);
        cJSON *gtag = cJSON_GetObjectItem(item, "gamertag");
        if (gtag && strcmp(gtag->valuestring, gamertag) == 0) {
        if (strlen(new_full_name) > 0) {
            cJSON_ReplaceItemInObject(item, "full_name", cJSON_CreateString(new_full_name));
        }

        if (strlen(new_ssn) > 0) {
            cJSON_ReplaceItemInObject(item, "ssn", cJSON_CreateString(new_ssn));
        }

        if (strlen(new_email) > 0) {
            cJSON_ReplaceItemInObject(item, "email", cJSON_CreateString(new_email));
        }

        if (strlen(sub_start) > 0) {
            cJSON_ReplaceItemInObject(item, "subscription_start_date", cJSON_CreateString(sub_start));
        }

        if (strlen(sub_end) > 0) {
            cJSON_ReplaceItemInObject(item, "subscription_end_date", cJSON_CreateString(sub_end));
        }

        // Abo-Status darf 0 oder 1 sein – prüfe auf gültigen Wert
        if (is_subscribed == 0 || is_subscribed == 1) {
            cJSON_ReplaceItemInObject(item, "is_subscribed", cJSON_CreateBool(is_subscribed));
        }

            found = 1;
            break;
        }
    }

    if (!found) {
        cJSON_Delete(user_array);
        return ERR_USER_NOT_FOUND; // Gamertag nicht gefunden
    }

    char *new_json = cJSON_Print(user_array);
    file = fopen(USERS_JSON_PATH, "w");
    if (!file) {
        free(new_json);
        cJSON_Delete(user_array);
        return ERR_STORAGE_FAILURE;
    }

    fputs(new_json, file);
    fflush(file);
    fclose(file);

    free(new_json);
    cJSON_Delete(user_array);
    return ERR_SUCCESS;
}


//=======================================================================
// CODE VON DEV BRANCH EINGEFÜGT:
// Die Signatur wird geändert, um einen Output-Parameter für die Anzahl aufzunehmen.
// 20.06.2025: Funktion load_games() wurde angepasst, den Fehler zu beheben, dass bei Programmstart Spiele falsch geladen werden.
Game *load_games(const char *filename, int *count_out) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        *count_out = 0;
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = malloc(length + 1);
    if (!data) {
        fclose(file);
        *count_out = 0;
        return NULL;
    }
    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    cJSON *root = cJSON_Parse(data);
    free(data);
    if (!root || !cJSON_IsObject(root)) {
        if (root) cJSON_Delete(root);
        *count_out = 0;
        return NULL;
    }

    cJSON *games_array = cJSON_GetObjectItem(root, "games");
    if (!games_array || !cJSON_IsArray(games_array)) {
        cJSON_Delete(root);
        *count_out = 0;
        return NULL;
    }

    int game_count = cJSON_GetArraySize(games_array);
    *count_out = game_count;

    Game *games = malloc(game_count * sizeof(Game));
    if (!games) {
        cJSON_Delete(root);
        *count_out = 0;
        return NULL;
    }

    for (int i = 0; i < game_count; i++) {
        cJSON *game_obj = cJSON_GetArrayItem(games_array, i);
        games[i].id = i + 1;

        cJSON *title_obj = cJSON_GetObjectItem(game_obj, "title");
        cJSON *desc_obj = cJSON_GetObjectItem(game_obj, "description");
        cJSON *ver_obj = cJSON_GetObjectItem(game_obj, "version");
        cJSON *mode_obj = cJSON_GetObjectItem(game_obj, "mode");
        cJSON *streams_obj = cJSON_GetObjectItem(game_obj, "current_streams");

        strncpy(games[i].title, title_obj && cJSON_IsString(title_obj) ? title_obj->valuestring : "", sizeof(games[i].title) - 1);
        games[i].title[sizeof(games[i].title) - 1] = '\0';

        strncpy(games[i].description, desc_obj && cJSON_IsString(desc_obj) ? desc_obj->valuestring : "", sizeof(games[i].description) - 1);
        games[i].description[sizeof(games[i].description) - 1] = '\0';

        strncpy(games[i].version, ver_obj && cJSON_IsString(ver_obj) ? ver_obj->valuestring : "", sizeof(games[i].version) - 1);
        games[i].version[sizeof(games[i].version) - 1] = '\0';

        strncpy(games[i].mode, mode_obj && cJSON_IsString(mode_obj) ? mode_obj->valuestring : "", sizeof(games[i].mode) - 1);
        games[i].mode[sizeof(games[i].mode) - 1] = '\0';

        games[i].current_streams = streams_obj && cJSON_IsNumber(streams_obj) ? streams_obj->valueint : 0;
    }

    cJSON_Delete(root);
    return games;
}


int save_games(const char *filename, Game games[], int game_count) {
    cJSON *root = cJSON_CreateObject();
    cJSON *games_array = cJSON_CreateArray();

    for (int i = 0; i < game_count; i++) {
        cJSON *game_obj = cJSON_CreateObject();
        cJSON_AddStringToObject(game_obj, "title", games[i].title);
        cJSON_AddStringToObject(game_obj, "description", games[i].description);
        cJSON_AddStringToObject(game_obj, "version", games[i].version);
        cJSON_AddStringToObject(game_obj, "mode", games[i].mode);
        cJSON_AddNumberToObject(game_obj, "current_streams", games[i].current_streams);
        cJSON_AddItemToArray(games_array, game_obj);
    }

    cJSON_AddItemToObject(root, "games", games_array);

    char *json_str = cJSON_Print(root);
    int result = -1;
    if (json_str) {
        FILE *file = fopen(filename, "w");
        if (file) {
            result = fputs(json_str, file);
            fclose(file);
        }
        free(json_str);
    }
    cJSON_Delete(root);
    return result == 0 ? ERR_SUCCESS : ERR_STORAGE_FAILURE;
}

// Funktion von Zinedin aus Branch feature-subscriptionEndDate eingefügt
// JSON Macro angepasst auf 
void remove_expired_users() {
    FILE *file = fopen(USERS_JSON_PATH, "r");
    if (!file) {
        printf("Error: Could not open %s\n", USERS_JSON_PATH);
        return;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = malloc(length + 1);
    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    cJSON *user_array = cJSON_Parse(data);
    free(data);

    if (!user_array || !cJSON_IsArray(user_array)) {
        printf("Error: Invalid JSON format in %s\n", USERS_JSON_PATH);
        cJSON_Delete(user_array);
        return;
    }

    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    current_time->tm_year -= 2;  // 2 Jahre zurück
    time_t two_years_ago = mktime(current_time);

    cJSON *new_array = cJSON_CreateArray();
    int removed_count = 0;

    int size = cJSON_GetArraySize(user_array);
    for (int i = 0; i < size; i++) {
        cJSON *user = cJSON_GetArrayItem(user_array, i);
        cJSON *sub_end = cJSON_GetObjectItem(user, "subscription_end_date");
        cJSON *sub_flag = cJSON_GetObjectItem(user, "is_subscribed");

        int keep = 1;

        if (sub_end && cJSON_IsString(sub_end) &&
            sub_flag && cJSON_IsBool(sub_flag) &&
            sub_flag->valueint == 0) {
            
            struct tm end_tm = {0};
            if (strptime(sub_end->valuestring, "%d.%m.%Y", &end_tm)) {
                time_t end_time = mktime(&end_tm);
                if (difftime(now, end_time) >= 2 * 365 * 24 * 3600) {
                    keep = 0;
                    removed_count++;
                }
            }
        }

        if (keep) {
            cJSON_AddItemToArray(new_array, cJSON_Duplicate(user, 1));
        }
    }

    char *json_text = cJSON_Print(new_array);
    file = fopen(USERS_JSON_PATH, "w");
    if (file) {
        fputs(json_text, file);
        fflush(file);
        fclose(file);
    }

    free(json_text);
    cJSON_Delete(user_array);
    cJSON_Delete(new_array);

    printf("\nRemoved %d expired user(s).\n", removed_count);
}
