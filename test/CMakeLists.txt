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
add_executable(test_data_remove_expired_users_missing_file_returns_ERR_STORAGE_FAILURE
    test_data_remove_expired_users_missing_file_returns_ERR_STORAGE_FAILURE.c
    unity/unity.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
)

target_compile_definitions(test_data_remove_expired_users_missing_file_returns_ERR_STORAGE_FAILURE PRIVATE USERS_JSON_PATH=\"test/missing_users.json\")

target_include_directories(test_data_remove_expired_users_missing_file_returns_ERR_STORAGE_FAILURE PRIVATE
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

# Executable für den Test logic_validate_required_field
add_executable(test_logic_validate_required_field
    test_logic_validate_required_field.c
    unity/unity.c
    ${PROJECT_SOURCE_DIR}/src/logic/logic.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    ${PROJECT_SOURCE_DIR}/src/presentation/messages.c
    ${PROJECT_SOURCE_DIR}/src/presentation/menu.c
    ${PROJECT_SOURCE_DIR}/src/presentation/presentation.c
    ${PROJECT_SOURCE_DIR}/src/simulation/simulation.c
)

target_include_directories(test_logic_validate_required_field PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src/logic
    ${PROJECT_SOURCE_DIR}/src/data
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/src/presentation
    ${PROJECT_SOURCE_DIR}/test/unity
)

# Executable für save_json_to_file Test
add_executable(test_data_save_json_to_file
    test_data_save_json_to_file.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_include_directories(test_data_save_json_to_file PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/inc/data
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/cJSON
)

# Executable für data_update_all_subscription_flags Test
add_executable(test_data_update_all_subscription_flags
    test_data_update_all_subscription_flags.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_compile_definitions(test_data_update_all_subscription_flags PRIVATE USERS_JSON_PATH=\"test/users_update_flags.json\")

target_include_directories(test_data_update_all_subscription_flags PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/inc/data
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/src/data
)

# Executable für data_save_player_profile Test
add_executable(test_data_save_player_profile
    test_data_save_player_profile.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_compile_definitions(test_data_save_player_profile PRIVATE USERS_JSON_PATH=\"test/users_save_profile.json\")

target_include_directories(test_data_save_player_profile PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/inc/data
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/src/data
)

# Executable für data_load_games Test
add_executable(test_data_load_games
    test_data_load_games.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_include_directories(test_data_load_games PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/inc/data
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/src/data
)

# Executable für logic_validate_player_profile Test
add_executable(test_logic_validate_player_profile
    test_logic_validate_player_profile.c
    ${PROJECT_SOURCE_DIR}/src/logic/logic.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/src/presentation/presentation.c
    ${PROJECT_SOURCE_DIR}/src/presentation/messages.c
    ${PROJECT_SOURCE_DIR}/src/presentation/menu.c
    ${PROJECT_SOURCE_DIR}/src/simulation/simulation.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_include_directories(test_logic_validate_player_profile PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/inc/logic
    ${PROJECT_SOURCE_DIR}/inc/data
    ${PROJECT_SOURCE_DIR}/inc/presentation
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/src/simulation
)

# Executable für logic_create_user_with_duration Test
add_executable(test_logic_create_user_with_duration
    test_logic_create_user_with_duration.c
    ${PROJECT_SOURCE_DIR}/src/logic/logic.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/src/presentation/presentation.c
    ${PROJECT_SOURCE_DIR}/src/presentation/messages.c
    ${PROJECT_SOURCE_DIR}/src/presentation/menu.c
    ${PROJECT_SOURCE_DIR}/src/simulation/simulation.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_compile_definitions(test_logic_create_user_with_duration PRIVATE USERS_JSON_PATH=\"test/users_create_duration.json\")

target_include_directories(test_logic_create_user_with_duration PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/inc/logic
    ${PROJECT_SOURCE_DIR}/inc/data
    ${PROJECT_SOURCE_DIR}/inc/presentation
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/src/simulation
)

# Executable für logic_remove_user Test
add_executable(test_logic_remove_user
    test_logic_remove_user.c
    ${PROJECT_SOURCE_DIR}/src/logic/logic.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/src/presentation/presentation.c
    ${PROJECT_SOURCE_DIR}/src/presentation/messages.c
    ${PROJECT_SOURCE_DIR}/src/presentation/menu.c
    ${PROJECT_SOURCE_DIR}/src/simulation/simulation.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_compile_definitions(test_logic_remove_user PRIVATE USERS_JSON_PATH=\"test/users_remove_test.json\")

target_include_directories(test_logic_remove_user PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/inc/logic
    ${PROJECT_SOURCE_DIR}/inc/data
    ${PROJECT_SOURCE_DIR}/inc/presentation
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/src/simulation
)

# Executable für logic_get_all_users Test
add_executable(test_logic_get_all_users
    test_logic_get_all_users.c
    ${PROJECT_SOURCE_DIR}/src/logic/logic.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/src/presentation/presentation.c
    ${PROJECT_SOURCE_DIR}/src/presentation/messages.c
    ${PROJECT_SOURCE_DIR}/src/presentation/menu.c
    ${PROJECT_SOURCE_DIR}/src/simulation/simulation.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_compile_definitions(test_logic_get_all_users PRIVATE USERS_JSON_PATH=\"test/users_get_all_test.json\")

target_include_directories(test_logic_get_all_users PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/inc/logic
    ${PROJECT_SOURCE_DIR}/inc/data
    ${PROJECT_SOURCE_DIR}/inc/presentation
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/src/simulation
)

# Executable für logic_edit_user Test
add_executable(test_logic_edit_user
    test_logic_edit_user.c
    ${PROJECT_SOURCE_DIR}/src/logic/logic.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/src/presentation/presentation.c
    ${PROJECT_SOURCE_DIR}/src/presentation/messages.c
    ${PROJECT_SOURCE_DIR}/src/presentation/menu.c
    ${PROJECT_SOURCE_DIR}/src/simulation/simulation.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_compile_definitions(test_logic_edit_user PRIVATE USERS_JSON_PATH=\"test/users_edit_logic.json\")

target_include_directories(test_logic_edit_user PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/inc/logic
    ${PROJECT_SOURCE_DIR}/inc/data
    ${PROJECT_SOURCE_DIR}/inc/presentation
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/src/simulation
)

# Executable für logic_get_top_users Test
add_executable(test_logic_get_top_users
    test_logic_get_top_users.c
    ${PROJECT_SOURCE_DIR}/src/logic/logic.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/src/presentation/presentation.c
    ${PROJECT_SOURCE_DIR}/src/presentation/messages.c
    ${PROJECT_SOURCE_DIR}/src/presentation/menu.c
    ${PROJECT_SOURCE_DIR}/src/simulation/simulation.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
    unity/unity.c
)

target_compile_definitions(test_logic_get_top_users PRIVATE USERS_JSON_PATH=\"test/users_top_test.json\")

target_include_directories(test_logic_get_top_users PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/inc/logic
    ${PROJECT_SOURCE_DIR}/inc/data
    ${PROJECT_SOURCE_DIR}/inc/presentation
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/src/simulation
)

# Executable um alle Tests gebündelt auszuführen
add_executable(run_all_tests run_all_tests.c)

# Warnings
add_compile_options(-Wall -Wextra -Wno-unused-parameter)

# Executable für die gemeinsamen Format-Tests
add_executable(test_is_valid_formats
    test_is_valid_formats.c
    unity/unity.c
    ${PROJECT_SOURCE_DIR}/src/logic/logic.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
)

target_include_directories(test_is_valid_formats PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/test/unity
)