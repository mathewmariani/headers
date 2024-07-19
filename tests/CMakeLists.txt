cmake_minimum_required(VERSION 3.20)
project(header-test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(TESTS_SRC
    fs_test.c
    ini_test.c)

add_executable(header-test main.c ${TESTS_SRC})
target_include_directories(header-test PRIVATE ..)