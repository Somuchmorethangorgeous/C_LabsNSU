cmake_minimum_required(VERSION 3.15)
project(TopologicSort C)

set(CMAKE_C_STANDARD 99)

add_executable(TopologicSort src/main.c src/lib.c src/lib.h)