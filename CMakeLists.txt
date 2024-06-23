cmake_minimum_required(VERSION 3.22.1)
project(contact_management_c C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(contact_management_c src/main.c src/contacts.c)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.6.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests tests/test_contacts.cpp src/contacts.c)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
