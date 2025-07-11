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
    failed |= run("./build/test/test_logic");
    failed |= run("./build/test/test_data");
    failed |= run("./build/test/test_data_remove_missing");
    failed |= run("./build/test/test_data_edit_profile");
    failed |= run("./build/test/test_data_edit_profile_invalid_json");
    failed |= run("./build/test/test_data_get_all_users");
    failed |= run("./build/test/test_data_save_report_missing_file");
    
    // Neue Tests für die drei zusätzlichen Funktionen
    failed |= run("./build/test/test_data_save_games");
    failed |= run("./build/test/test_data_remove_player_profile");
    failed |= run("./build/test/test_data_load_reports");

    if (failed) {
        printf("\nEinige Tests sind FEHLGESCHLAGEN.\n");
        return 1;
    } else {
        printf("\nAlle Tests wurden erfolgreich ausgeführt.\n");
        return 0;
    }
}