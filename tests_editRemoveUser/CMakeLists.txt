cmake_minimum_required(VERSION 3.10) 
project(gameplatform_tests C)

# Coverage
if(COVERAGE)
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

add_custom_target(coverage
    COMMAND gcovr
        --root ..
        --object-directory .
        --filter '../data/'
        --filter '../logic/'
        --filter '../presentation/'
        --exclude '../data/cJSON.c'
        --html --html-details
        -o coverage_report.html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Include the source code directories (data, logic, presentation)
include_directories(
    ../data
    ../logic
    ../presentation
    ./unity
)

# List of source files for testing
set(TEST_SOURCES
    test_runner.c
    test_logic.c
    unity/unity.c
    test_data.c
)

# Link to real source code (only the modules you want to test)
set(SOURCE_UNDER_TEST
    ../presentation/presentation.c
    ../logic/logic.c
    ../data/data.c
    ../data/cJSON.c
)

# Create the test executable
add_executable(runTests
    ${TEST_SOURCES}
    ${SOURCE_UNDER_TEST}
)

# Force show all warnings
add_compile_options(-Wall -Wextra -Wno-unused-parameter)


# --- Ausführen coverage ---
# cd built_tests
# rm -rf *
# cmake -DCOVERAGE=ON ../tests
# make VERBOSE=1
# ./runTests
# make coverage