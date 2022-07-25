cmake_minimum_required(VERSION 3.21)
project(AviGenerator C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-static -municode")

add_executable(AviGenerator WIN32 main.c funcs.h)
add_executable(delete delete.c)
add_executable(Testing Testing.c)