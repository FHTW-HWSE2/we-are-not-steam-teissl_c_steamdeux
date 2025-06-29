cmake_minimum_required(VERSION 3.10)
project(TestRunner C)

option(COVERAGE "Enable coverage reporting" ON)

if(COVERAGE AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Building with coverage flags")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add directories for includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc/data)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc/logic)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc/presentation)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cJSON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mocks)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../tests/unity)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/CMock/src)

# Define the test executable
add_executable(test_runner
    ${CMAKE_CURRENT_SOURCE_DIR}/test_main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_logic.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_data.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/Mockdata.c            #//auskommentieren für coverage
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/Mocklogic.c           #//auskommentieren für coverage
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/Mockpresentation.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../tests/unity/unity.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/CMock/src/cmock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/presentation/run.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../cJSON/cJSON.c
)

# Enable testing
enable_testing()
add_test(NAME test_runner COMMAND test_runner)