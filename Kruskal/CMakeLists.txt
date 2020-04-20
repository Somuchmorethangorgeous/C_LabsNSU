cmake_minimum_required(VERSION 3.15)
project(Kruskal C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Kruskal src/lib.c src/lib.h src/main.c)
