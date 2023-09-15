cmake_minimum_required(VERSION 3.25)
project(hw13 C)

set(CMAKE_C_STANDARD 17)

add_executable(hw13 graphics.c graphics.h hw13_main.c object.c hw13.c)
