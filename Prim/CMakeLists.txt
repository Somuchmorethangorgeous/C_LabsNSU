cmake_minimum_required(VERSION 3.15)
project(Prim C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Prim src/lib.c src/lib.h src/main.c)
