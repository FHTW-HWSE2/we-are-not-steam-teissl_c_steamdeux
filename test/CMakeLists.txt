# test/CMakeLists.txt

# GCOVR flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

# Zweites Test-Projekt: test_data
add_executable(test_data
    test_data.c
    unity/unity.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
)

target_include_directories(test_data PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/inc/data
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/test/mocks
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/src/data
)

# Executable für den Test "test_data_remove_expired_users_missing_file_returns_ERR_STORAGE_FAILURE"
add_executable(test_data_remove_missing
    test_data_remove_expired_users_missing_file_returns_ERR_STORAGE_FAILURE.c
    unity/unity.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
)

target_compile_definitions(test_data_remove_missing PRIVATE USERS_JSON_PATH=\"test/missing_users.json\")

target_include_directories(test_data_remove_missing PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src/data
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/test/mocks
    ${PROJECT_SOURCE_DIR}/cJSON
)

# Executable für den Tests der Funktion data_edit_player_profile
add_executable(test_data_edit_profile
    test_data_edit_player_profile.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_compile_definitions(test_data_edit_profile PRIVATE USERS_JSON_PATH=\"test/users_edit.json\")

target_include_directories(test_data_edit_profile PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src/data
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/test/mocks
)

# Executable für den Test der Funktion data_edit_player_profile mit ungültigem JSON
add_executable(test_data_edit_profile_invalid_json
    test_data_edit_player_profile_invalid_json.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_compile_definitions(test_data_edit_profile_invalid_json PRIVATE USERS_JSON_PATH=\"test/users_invalid_object.json\")

target_include_directories(test_data_edit_profile_invalid_json PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src/data
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/test/mocks
    ${PROJECT_SOURCE_DIR}/cJSON
)

# Executable für den Test der Funktion data_get_all_users
add_executable(test_data_get_all_users
    test_data_get_all_users.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_compile_definitions(test_data_get_all_users PRIVATE USERS_JSON_PATH=\"test/users_get.json\")

target_include_directories(test_data_get_all_users PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src/data
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/test/mocks
)

# Executable für den Test der Funktion data_save_report
add_executable(test_data_save_report_missing_file
    test_data_save_report_missing_file.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_compile_definitions(test_data_save_report_missing_file PRIVATE REPORTS_FILE=\"test/reports_created_from_missing.json\")

target_include_directories(test_data_save_report_missing_file PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src/data
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/test/mocks
)

# Executable für den Test der Funktion data_load_json mit malloc Fehler
add_executable(test_data_load_json_malloc_fail
    test_data_load_json_malloc_fail.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_compile_definitions(test_data_load_json_malloc_fail PRIVATE USERS_JSON_PATH=\"test/users_malloc_fail.json\")

target_include_directories(test_data_load_json_malloc_fail PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src/data
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/test/unity
)

# Executable für den Test der Funktion data_save_report mit Fallback auf Array
add_executable(test_data_save_report_fallback_array
    test_data_save_report_fallback_array.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_compile_definitions(test_data_save_report_fallback_array PRIVATE REPORTS_FILE=\"test/missing_or_invalid_report.json\")

target_include_directories(test_data_save_report_fallback_array PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src/data
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/test/unity
)

# Executable für den Test der Funktion data_save_report mit ungültigem JSON
add_executable(test_data_save_report_invalid_json_type
    test_data_save_report_invalid_json_type.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_compile_definitions(test_data_save_report_invalid_json_type PRIVATE REPORTS_FILE=\"test/reports_invalid_object.json\")

target_include_directories(test_data_save_report_invalid_json_type PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src/data
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/test/unity
)

# === NEUE TESTS FÜR DIE DREI ZUSÄTZLICHEN FUNKTIONEN ===

# Executable für den Test der Funktion data_save_games
add_executable(test_data_save_games
    test_data_save_games.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_include_directories(test_data_save_games PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src/data
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/test/mocks
)

# Executable für den Test der Funktion data_remove_player_profile
add_executable(test_data_remove_player_profile
    test_data_remove_player_profile.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_include_directories(test_data_remove_player_profile PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src/data
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/test/mocks
)


# Executable um alle Tests gebündelt auszuführen
add_executable(run_all_tests run_all_tests.c)

# Warnings
add_compile_options(-Wall -Wextra -Wno-unused-parameter)