cmake_minimum_required(VERSION 3.9)
project(Term2)

set(CMAKE_CXX_STANDARD 11)

add_executable(Term2
        EMPR.c
        EMPR.h
        Generator.c
        serial.c
        serial.h)
