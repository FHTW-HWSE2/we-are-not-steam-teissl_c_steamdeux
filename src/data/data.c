#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "../inc/data/data.h"

#ifndef REPORTS_FILE
#define REPORTS_FILE "../reports.json"
#endif


// Magic Strings: JSON keys
static const char* KEY_FULL_NAME = "full_name";
static const char* KEY_GAMERTAG = "gamertag";
static const char* KEY_PLAYER_HOURS = "player_hours";
static const char* KEY_SSN = "ssn";
static const char* KEY_EMAIL = "email";
static const char* KEY_SUB_START = "subscription_start_date";
static const char* KEY_SUB_END = "subscription_end_date";
static const char* KEY_IS_SUBSCRIBED = "is_subscribed";
static const char* KEY_GAMES = "games";
static const char* KEY_TITLE = "title";
static const char* KEY_DESCRIPTION = "description";
static const char* KEY_VERSION = "version";
static const char* KEY_MODE = "mode";
static const char* KEY_CURRENT_STREAMS = "current_streams";

// DRY: JSON load/save helpers 
//DAVID
cJSON* load_json_from_file(const char* path) {
    FILE *file = fopen(path, "r");
    if (!file) return NULL;
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = malloc(length + 1);
    if (!data) {
        fclose(file);
        return NULL;
    }
    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);
    cJSON *json = cJSON_Parse(data);
    free(data);
    return json;
}

//DONATO
static int save_json_to_file(const char* path, cJSON* json) {
    char *json_text = cJSON_Print(json);
    if (!json_text) return ERR_STORAGE_FAILURE;
    FILE *file = fopen(path, "w");
    if (!file) {
        free(json_text);
        return ERR_STORAGE_FAILURE;
    }
    fputs(json_text, file);
    fflush(file);
    fclose(file);
    free(json_text);
    return ERR_SUCCESS;
}


// Refactored am 04.07.2025: Ausgelagert aus presentation.c
// Diese Funktion übernimmt die Dateioperationen und JSON-Logik für das Aktualisieren der Subscription-Flags.
//DONATO
int data_update_all_subscription_flags() {
    cJSON *user_array = load_json_from_file(USERS_JSON_PATH);
    if (!user_array || !cJSON_IsArray(user_array)) {
        if (user_array) cJSON_Delete(user_array);
        return 0;
    }
    time_t now = time(NULL);
    int changed = 0;
    int size = cJSON_GetArraySize(user_array);
    for (int i = 0; i < size; i++) {
        cJSON *user = cJSON_GetArrayItem(user_array, i);
        cJSON *sub_end = cJSON_GetObjectItem(user, KEY_SUB_END);
        cJSON *sub_flag = cJSON_GetObjectItem(user, KEY_IS_SUBSCRIBED);
        if (sub_end && cJSON_IsString(sub_end)) {
            struct tm end_tm = {0};
            if (strptime(sub_end->valuestring, "%d.%m.%Y", &end_tm)) {
                time_t end_time = mktime(&end_tm);
                int should_be = (difftime(end_time, now) >= 0) ? 1 : 0;
                if (!sub_flag || sub_flag->valueint != should_be) {
                    cJSON_ReplaceItemInObject(user, KEY_IS_SUBSCRIBED, cJSON_CreateBool(should_be));
                    changed++;
                }
            }
        }
    }
    if (changed) {
        save_json_to_file(USERS_JSON_PATH, user_array);
    }
    cJSON_Delete(user_array);
    return changed;
}


// data_load_reports removed - not used anywhere WAR VON BERK NICHT MEHR NÖTIG

//DAVID?
int data_save_report(cJSON *report) {
    cJSON *reports = load_json_from_file(REPORTS_FILE);
    if (!reports) {
        reports = cJSON_CreateArray();
    }
    cJSON_AddItemToArray(reports, report);
    int result = save_json_to_file(REPORTS_FILE, reports);
    cJSON_Delete(reports);
    return result;
}

//DONATO
int data_save_player_profile(const char* full_name, const char* gamertag, int player_hours, const char* ssn, const char* email, const char* sub_start, const char* sub_end, int is_subscribed){
    // Datenschicht: Speichert User-Profil in Datei
    cJSON *user_array = load_json_from_file(USERS_JSON_PATH);
    if (!user_array || !cJSON_IsArray(user_array)) {
        if (user_array) cJSON_Delete(user_array);
        user_array = cJSON_CreateArray();
    }
    if (!user_array) {
        return ERR_STORAGE_FAILURE;
    }
    cJSON *user = cJSON_CreateObject();
    cJSON_AddStringToObject(user, KEY_FULL_NAME, full_name);
    cJSON_AddStringToObject(user, KEY_GAMERTAG, gamertag);
    cJSON_AddNumberToObject(user, KEY_PLAYER_HOURS, player_hours);
    cJSON_AddStringToObject(user, KEY_SSN, ssn);
    cJSON_AddStringToObject(user, KEY_EMAIL, email);
    cJSON_AddStringToObject(user, KEY_SUB_START, sub_start);
    cJSON_AddStringToObject(user, KEY_SUB_END, sub_end);
    cJSON_AddBoolToObject(user, KEY_IS_SUBSCRIBED, is_subscribed);
    cJSON_AddItemToArray(user_array, user);
    int result = save_json_to_file(USERS_JSON_PATH, user_array);
    cJSON_Delete(user_array);
    return result;
}


// Neue Funktion: Gibt alle User als cJSON-Array zurück
//DAVID
int data_get_all_users(cJSON **users_out) {
    cJSON *user_array = load_json_from_file(USERS_JSON_PATH);
    if (!user_array || !cJSON_IsArray(user_array)) {
        if (user_array) cJSON_Delete(user_array);
        *users_out = cJSON_CreateArray();
        return ERR_SUCCESS;
    }
    *users_out = user_array;
    return ERR_SUCCESS;
}

//BERK
int data_remove_player_profile(const char* gamertag) {
    cJSON *user_array = load_json_from_file(USERS_JSON_PATH);
    if (!user_array || !cJSON_IsArray(user_array)) {
        if (user_array) cJSON_Delete(user_array);
        return ERR_STORAGE_FAILURE;
    }
    int array_size = cJSON_GetArraySize(user_array);
    for (int i = 0; i < array_size; i++) {
        cJSON *item = cJSON_GetArrayItem(user_array, i);
        cJSON *gtag = cJSON_GetObjectItem(item, KEY_GAMERTAG);
        if (gtag && strcmp(gtag->valuestring, gamertag) == 0) {
            cJSON_DeleteItemFromArray(user_array, i);
            int result = save_json_to_file(USERS_JSON_PATH, user_array);
            cJSON_Delete(user_array);
            return result == ERR_SUCCESS ? ERR_SUCCESS : ERR_STORAGE_FAILURE;
        }
    }
    cJSON_Delete(user_array);
    return ERR_USER_NOT_FOUND;
}

//DAVID
int data_edit_player_profile(const char* gamertag, const char* new_full_name, const char* new_ssn, const char* new_email, const char* sub_start, const char* sub_end, int is_subscribed) {
    cJSON *user_array = load_json_from_file(USERS_JSON_PATH);
    if (!user_array || !cJSON_IsArray(user_array)) {
        if (user_array) cJSON_Delete(user_array);
        return ERR_STORAGE_FAILURE;
    }
    int array_size = cJSON_GetArraySize(user_array);
    int found = 0;
    for (int i = 0; i < array_size; i++) {
        cJSON *item = cJSON_GetArrayItem(user_array, i);
        cJSON *gtag = cJSON_GetObjectItem(item, KEY_GAMERTAG);
        if (gtag && strcmp(gtag->valuestring, gamertag) == 0) {
            if (strlen(new_full_name) > 0) {
                cJSON_ReplaceItemInObject(item, KEY_FULL_NAME, cJSON_CreateString(new_full_name));
            }
            if (strlen(new_ssn) > 0) {
                cJSON_ReplaceItemInObject(item, KEY_SSN, cJSON_CreateString(new_ssn));
            }
            if (strlen(new_email) > 0) {
                cJSON_ReplaceItemInObject(item, KEY_EMAIL, cJSON_CreateString(new_email));
            }
            if (strlen(sub_start) > 0) {
                cJSON_ReplaceItemInObject(item, KEY_SUB_START, cJSON_CreateString(sub_start));
            }
            if (strlen(sub_end) > 0) {
                cJSON_ReplaceItemInObject(item, KEY_SUB_END, cJSON_CreateString(sub_end));
            }
            if (is_subscribed == 0 || is_subscribed == 1) {
                cJSON_ReplaceItemInObject(item, KEY_IS_SUBSCRIBED, cJSON_CreateBool(is_subscribed));
            }
            found = 1;
            break;
        }
    }
    if (!found) {
        cJSON_Delete(user_array);
        return ERR_USER_NOT_FOUND;
    }
    int result = save_json_to_file(USERS_JSON_PATH, user_array);
    cJSON_Delete(user_array);
    return result;
}


//=======================================================================
// CODE VON DEV BRANCH EINGEFÜGT:
// Die Signatur wird geändert, um einen Output-Parameter für die Anzahl aufzunehmen.
// 20.06.2025: Funktion load_games() wurde angepasst, den Fehler zu beheben, dass bei Programmstart Spiele falsch geladen werden.
// Refactored 05.07.2025: Signatur auf robustes Fehlerbehandlungs-Muster umgestellt.


//DONATO
int data_load_games(const char *filename, Game **games_out, int *count_out) {
    if (!filename || !games_out || !count_out) {
        return ERR_STORAGE_FAILURE;
    }
    
    *games_out = NULL;
    *count_out = 0;
    
    cJSON *root = load_json_from_file(filename);
    if (!root || !cJSON_IsObject(root)) {
        if (root) cJSON_Delete(root);
        return ERR_STORAGE_FAILURE;
    }
    
    cJSON *games_array = cJSON_GetObjectItem(root, KEY_GAMES);
    if (!games_array || !cJSON_IsArray(games_array)) {
        cJSON_Delete(root);
        return ERR_STORAGE_FAILURE;
    }
    
    int game_count = cJSON_GetArraySize(games_array);
    if (game_count == 0) {
        cJSON_Delete(root);
        return ERR_SUCCESS; // Empty array is valid
    }
    
    Game *games = malloc(game_count * sizeof(Game));
    if (!games) {
        cJSON_Delete(root);
        return ERR_STORAGE_FAILURE;
    }
    
    for (int i = 0; i < game_count; i++) {
        cJSON *game_obj = cJSON_GetArrayItem(games_array, i);
        games[i].id = i + 1;
        cJSON *title_obj = cJSON_GetObjectItem(game_obj, KEY_TITLE);
        cJSON *desc_obj = cJSON_GetObjectItem(game_obj, KEY_DESCRIPTION);
        cJSON *ver_obj = cJSON_GetObjectItem(game_obj, KEY_VERSION);
        cJSON *mode_obj = cJSON_GetObjectItem(game_obj, KEY_MODE);
        cJSON *streams_obj = cJSON_GetObjectItem(game_obj, KEY_CURRENT_STREAMS);
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
    *games_out = games;
    *count_out = game_count;
    return ERR_SUCCESS;
}

//BERK
int data_save_games(const char *filename, Game games[], int game_count) {
    cJSON *root = cJSON_CreateObject();
    cJSON *games_array = cJSON_CreateArray();
    for (int i = 0; i < game_count; i++) {
        cJSON *game_obj = cJSON_CreateObject();
        cJSON_AddStringToObject(game_obj, KEY_TITLE, games[i].title);
        cJSON_AddStringToObject(game_obj, KEY_DESCRIPTION, games[i].description);
        cJSON_AddStringToObject(game_obj, KEY_VERSION, games[i].version);
        cJSON_AddStringToObject(game_obj, KEY_MODE, games[i].mode);
        cJSON_AddNumberToObject(game_obj, KEY_CURRENT_STREAMS, games[i].current_streams);
        cJSON_AddItemToArray(games_array, game_obj);
    }
    cJSON_AddItemToObject(root, KEY_GAMES, games_array);
    int result = save_json_to_file(filename, root);
    cJSON_Delete(root);
    return result;
}

// Funktion von Zinedin aus Branch feature-subscriptionEndDate eingefügt
//DAVID
int data_remove_expired_users(int *removed_count_out) {
    cJSON *user_array = load_json_from_file(USERS_JSON_PATH);
    if (!user_array || !cJSON_IsArray(user_array)) {
        if (user_array) cJSON_Delete(user_array);
        if (removed_count_out) *removed_count_out = 0;
        return ERR_STORAGE_FAILURE;
    }
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    current_time->tm_year -= 2;
    cJSON *new_array = cJSON_CreateArray();
    int removed_count = 0;
    int size = cJSON_GetArraySize(user_array);
    for (int i = 0; i < size; i++) {
        cJSON *user = cJSON_GetArrayItem(user_array, i);
        cJSON *sub_end = cJSON_GetObjectItem(user, KEY_SUB_END);
        cJSON *sub_flag = cJSON_GetObjectItem(user, KEY_IS_SUBSCRIBED);
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
    int result = save_json_to_file(USERS_JSON_PATH, new_array);
    cJSON_Delete(user_array);
    cJSON_Delete(new_array);
    if (removed_count_out) *removed_count_out = removed_count;
    return result == ERR_SUCCESS ? ERR_SUCCESS : ERR_STORAGE_FAILURE;
}

// data_get_user_by_gamertag removed - not used anywhere
