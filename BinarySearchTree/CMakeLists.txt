cmake_minimum_required(VERSION 3.15)
project(BinarySearchTree C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(SearchTree src/balance.c src/balance.h src/main.c)
