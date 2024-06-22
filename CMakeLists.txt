cmake_minimum_required(VERSION 3.22.1)
project(contact_management_c C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(contact_management_c src/main.c src/contacts.c)
