set(TESTS_SRC
    fs_test.c
    ini_test.c)

add_executable(header-tests main.c ${TESTS_SRC})
target_include_directories(header-tests PRIVATE ..)