cmake_minimum_required(VERSION 3.15)
project(DijkstraForSequences C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(DijkstraForSequences src/main.c src/swap.c src/swap.h)