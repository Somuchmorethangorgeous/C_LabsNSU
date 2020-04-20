cmake_minimum_required(VERSION 3.15)
project(CalculatorWithBrackets C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(CalculatorWithBrackets src/Calculate.c src/Calculate.h src/main.c)
