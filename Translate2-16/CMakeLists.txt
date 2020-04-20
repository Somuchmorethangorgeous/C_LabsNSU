cmake_minimum_required(VERSION 3.15)
project(Translation2-16 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Translation2-16 src/main.c src/Translation.c src/Translation.h)