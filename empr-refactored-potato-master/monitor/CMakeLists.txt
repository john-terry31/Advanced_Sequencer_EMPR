cmake_minimum_required(VERSION 3.9)
project(monitor)

set(CMAKE_CXX_STANDARD 11)

add_executable(monitor
        mp1.c
        serial.c
        serial.h)
