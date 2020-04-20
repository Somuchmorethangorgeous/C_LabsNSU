cmake_minimum_required(VERSION 3.15)
project(Quicksort C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Quicksort src/main.c src/quick.c src/quick.h)
