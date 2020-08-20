# cmake_minimum_required(VERSION <specify CMake version here>)
project(webserver C)

set(CMAKE_C_STANDARD 11)

add_executable(webserver webserver.c)