cmake_minimum_required(VERSION 3.8)
project(task5)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        mp3.c
        serial.c
        serial.h)

add_executable(task5 ${SOURCE_FILES})