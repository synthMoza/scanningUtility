cmake_minimum_required(VERSION 3.14)
project(scan_util)

set(CMAKE_CXX_STANDARD 17)

# Declare folder/files variables
set(INCLUDE_DIR include)
set(SRC_DIR src)

set(HEADERS ${INCLUDE_DIR}/scanner.hpp ${INCLUDE_DIR}/threat.hpp)
set(SRC_FILES ${SRC_DIR}/scan_util.cpp ${SRC_DIR}/scanner.cpp ${SRC_DIR}/threat.cpp)

# Configure compilation
include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADERS})