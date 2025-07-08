cmake_minimum_required(VERSION 3.10)
project(GameManager C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Coverage Optionen (optional)
option(COVERAGE "Enable coverage reporting" ON)
if(COVERAGE)
    message("Coverage enabled")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov")
endif()

# Test-Option separat aktivierbar
option(ENABLE_TESTING "Enable unit tests" ON)

find_package(PkgConfig REQUIRED)

# Include-Verzeichnisse
include_directories(
    ${CMAKE_SOURCE_DIR}/inc/data
    ${CMAKE_SOURCE_DIR}/inc/logic
    ${CMAKE_SOURCE_DIR}/inc/presentation
    ${CMAKE_SOURCE_DIR}/cJSON
)

# Quellcode-Dateien
set(SOURCES
    main.c
    src/data/data.c
    src/logic/logic.c
    src/presentation/presentation.c
    cJSON/cJSON.c
    src/simulation/simulation.c
)

# Executable erstellen
add_executable(game_manager ${SOURCES})

# Main app als Test (z. B. für manuelle Launch-Tests)
enable_testing()
add_test(NAME GameManagerTestRun COMMAND game_manager)

# Test-Unterordner hinzufügen
if(ENABLE_TESTING)
    # add_subdirectory(test)  # Test-Ordner wurde entfernt
endif()
