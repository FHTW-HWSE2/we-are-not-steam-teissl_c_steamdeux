#include <stdio.h>
#include <stdlib.h>

int run(const char* path) {
    printf("\n=== Running: %s ===\n", path);
    int code = system(path);
    if (code != 0) {
        printf("❌ FAILED: %s (Exit Code: %d)\n", path, code);
    } else {
        printf("✅ PASSED: %s\n", path);
    }
    return code;
}

int main(void) {
    int failed = 0;

    // Ausführbare Testdateien im Build-Ordner
    failed |= run("./build/test/test_data");
    failed |= run("./build/test/test_data_remove_expired_users_missing_file_returns_ERR_STORAGE_FAILURE");
    failed |= run("./build/test/test_data_get_all_users");
    failed |= run("./build/test/test_data_save_report_missing_file");
    failed |= run("./build/test/test_data_load_json_malloc_fail");
    failed |= run("./build/test/test_data_save_report_fallback_array");
    failed |= run("./build/test/test_data_save_report_invalid_json_type");
    failed |= run("./build/test/test_data_remove_expired_users_all_users_valid");
    // Neue Tests für die drei zusätzlichen Funktionen
    failed |= run("./build/test/test_data_save_games");
    failed |= run("./build/test/test_data_remove_player_profile");
    failed |= run("./build/test/test_logic_validate_required_field");
    failed |= run("./build/test/test_is_valid_formats"); // 🍺 Gemeinsame Format-Tests
    //Sina
    failed |= run("./build/test/test_logic_create_report");
    failed |= run("./build/test/test_logic_create_and_save_report");
    failed |= run("./build/test/test_data_save_json_to_file");
    failed |= run("./build/test/test_data_update_all_subscription_flags");
    failed |= run("./build/test/test_data_save_player_profile");
    failed |= run("./build/test/test_data_load_games");
    failed |= run("./build/test/test_logic_validate_player_profile");
    failed |= run("./build/test/test_logic_create_user_with_duration");
    failed |= run("./build/test/test_logic_remove_user");
    failed |= run("./build/test/test_logic_get_all_users");
    failed |= run("./build/test/test_logic_edit_user");
    failed |= run("./build/test/test_logic_get_top_users");
    failed |= run("./build/test/test_logic_generate_top_users_file");
    // Ergänzte Kern-Validierungsfunktionen
    failed |= run("./build/test/test_logic_validate_subscription_status");
    failed |= run("./build/test/test_is_date_in_future");
    // failed |= run("./build/test/test_logic_is_only_spaces"); // Deaktiviert, da Binary nicht existiert
    failed |= run("./build/test/test_read_input");
    failed |= run("./build/test/test_logic_get_user_lines_for_display");
    failed |= run("./build/test/test_logic_display_users_formatted");
    failed |= run("./build/test/test_logic_initialize_game_data_loading");
    failed |= run("./build/test/test_logic_edit_game");
    failed |= run("./build/test/test_logic_delete_game");
    failed |= run("./build/test/test_logic_add_new_game");
    failed |= run("./build/test/test_logic_display_games_formatted");
    failed |= run("./build/test/test_simulation");
    failed |= run("./build/test/test_run_simulation");
    failed |= run("./build/test/test_logic_start_application");
    failed |= run("./build/test/test_logic_user_menu_workflow");
    failed |= run("./build/test/test_logic_handle_add_game");
    failed |= run("./build/test/test_logic_handle_game_management_menu");
    failed |= run("./build/test/test_logic_handle_game_workflows");
    failed |= run("./build/test/test_data_remove_expired_users_invalid_cases");
    failed |= run("./build/test/test_data_remove_expired_users_save_failure");

    if (failed) {
        printf("\nEinige Tests sind FEHLGESCHLAGEN.\n");
        return 1;
    } else {
        printf("\nAlle Tests wurden erfolgreich ausgeführt.\n");
        return 0;
    }
}
