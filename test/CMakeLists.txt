# test/CMakeLists.txt

# Test-Projekt
add_executable(test_logic
    test_logic.c
    unity/unity.c
    mock_load_games.c
    ${PROJECT_SOURCE_DIR}/src/logic/logic.c
    ${PROJECT_SOURCE_DIR}/src/data/data.c
    ${PROJECT_SOURCE_DIR}/cJSON/cJSON.c
)

# Includes
target_include_directories(test_logic PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/inc/data
    ${PROJECT_SOURCE_DIR}/test/unity
    ${PROJECT_SOURCE_DIR}/test/mocks
    ${PROJECT_SOURCE_DIR}/cJSON
    ${PROJECT_SOURCE_DIR}/src/data
)

# Falls cJSON als Source-Datei eingebunden ist (statt über pkg-config)
# target_sources(test_logic PRIVATE ${PROJECT_SOURCE_DIR}/external/cJSON.c)
# target_include_directories(test_logic PRIVATE ${PROJECT_SOURCE_DIR}/external)

# Force show all warnings
add_compile_options(-Wall -Wextra -Wno-unused-parameter)

