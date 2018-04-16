cmake_minimum_required(VERSION 3.10)
project(empr_refactored_potato_master C)

set(CMAKE_C_STANDARD 11)

include_directories(generator)
include_directories("Mini Projects/MP1")
include_directories("Mini Projects/MP2")
include_directories("Mini Projects/MP2_2")
include_directories("Mini Projects/MP2_calc")
include_directories("Mini Projects/MP2_combined")
include_directories("Mini Projects/MP3/combined")
include_directories("Mini Projects/MP3/task1")
include_directories("Mini Projects/MP3/task2")
include_directories("Mini Projects/MP3/task3")
include_directories("Mini Projects/MP3/task4")
include_directories("Mini Projects/MP3/task5")
include_directories(monitor)

add_executable(empr_refactored_potato_master
        generator/EMPR.c
        generator/EMPR.h
        generator/Generator.c
        generator/Generator.h
        generator/vector.c
        generator/vector.h
        generator/JTsequencer.c
        generator/JTsequencer.h
        generator/keypad.c
        generator/keypad.h
        generator/lcd.c
        generator/lcd.h
        generator/leds.c
        generator/leds.h
        generator/serial.c
        generator/serial.h
        "Mini Projects/MP1/mp1.c"
        "Mini Projects/MP1/serial.c"
        "Mini Projects/MP1/serial.h"
        "Mini Projects/MP2/mp2.c"
        "Mini Projects/MP2/serial.c"
        "Mini Projects/MP2/serial.h"
        "Mini Projects/MP2_2/mp2.c"
        "Mini Projects/MP2_2/serial.c"
        "Mini Projects/MP2_2/serial.h"
        "Mini Projects/MP2_calc/mp2.c"
        "Mini Projects/MP2_calc/serial.c"
        "Mini Projects/MP2_calc/serial.h"
        "Mini Projects/MP2_combined/mp2.c"
        "Mini Projects/MP2_combined/serial.c"
        "Mini Projects/MP2_combined/serial.h"
        "Mini Projects/MP3/combined/mp3.c"
        "Mini Projects/MP3/combined/serial.c"
        "Mini Projects/MP3/combined/serial.h"
        "Mini Projects/MP3/task1/mp3.c"
        "Mini Projects/MP3/task1/serial.c"
        "Mini Projects/MP3/task1/serial.h"
        "Mini Projects/MP3/task2/mp3.c"
        "Mini Projects/MP3/task2/serial.c"
        "Mini Projects/MP3/task2/serial.h"
        "Mini Projects/MP3/task3/mp3.c"
        "Mini Projects/MP3/task3/serial.c"
        "Mini Projects/MP3/task3/serial.h"
        "Mini Projects/MP3/task4/mp3.c"
        "Mini Projects/MP3/task4/serial.c"
        "Mini Projects/MP3/task4/serial.h"
        "Mini Projects/MP3/task5/mp3.c"
        "Mini Projects/MP3/task5/serial.c"
        "Mini Projects/MP3/task5/serial.h"
        misc/monitor.c
        monitor/keypad.c
        monitor/lcd.c
        monitor/lcd.h
        monitor/leds.c
        monitor/leds.h
        monitor/monitor.c
        monitor/serial.c
        monitor/serial.h generator/sequences.c generator/sequences.h)
