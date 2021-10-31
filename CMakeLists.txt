cmake_minimum_required(VERSION 3.14)
project(scan_util)

set(CMAKE_CXX_STANDARD 17)

# Declare folder/files variables
set(INCLUDE_DIR include)
set(SRC_DIR src)
set(TEST_DIR tests)
set(TEST_CASES_DIR ${TEST_DIR}/e2e)

set(HEADERS ${INCLUDE_DIR}/scanner.hpp ${INCLUDE_DIR}/threat.hpp ${INCLUDE_DIR}/timer.hpp)
set(SRC_FILES ${SRC_DIR}/scan_util.cpp ${SRC_DIR}/scanner.cpp ${SRC_DIR}/timer.cpp)
set(TESTS ${TEST_DIR}/tests.cpp)

# Google test definition
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e
)

enable_testing()

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Configure compilation
# Main executable
include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADERS})

# Target for using the application with threads
add_executable(${PROJECT_NAME}_parallel ${SRC_FILES} ${HEADERS})
target_compile_definitions(${PROJECT_NAME}_parallel PRIVATE PARALLEL)

# Target for testing single-core version using gtests
add_executable(${PROJECT_NAME}_tests ${SRC_FILES} ${HEADERS} ${TESTS})
target_compile_definitions(${PROJECT_NAME}_tests PRIVATE TEST)

# Target for testing single-core version using gtests
add_executable(${PROJECT_NAME}_tests_parallel ${SRC_FILES} ${HEADERS} ${TESTS})
target_compile_definitions(${PROJECT_NAME}_tests_parallel PRIVATE TEST PARALLEL)

# Configure gtests
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)

set(TEST_DIR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_CASES_DIR})
configure_file(${TESTS} ${TESTS} @ONLY)

target_link_libraries(${PROJECT_NAME}_tests gtest_main)
target_link_libraries(${PROJECT_NAME}_tests_parallel gtest_main)