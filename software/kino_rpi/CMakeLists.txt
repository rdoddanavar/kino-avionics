#-------------------------------------------------------------------#
# High Power Rocketry - Avionics Hardware & Software
# MIT License
# Copyright (c) 2020 Roshan Doddanavar
# https://rdoddanavar.github.io
#  _   _                       _          _       
# | |_(_)_ _  ___ ___ __ ___ _(_)___ _ _ (_)__ ___
# | / / | ' \/ _ \___/ _` \ V / / _ \ ' \| / _(_-<
# |_\_\_|_||_\___/   \__,_|\_/|_\___/_||_|_\__/__/
#
# Path:
#     kino-avionics/software/kino_rpi/CMakelists.txt
# Created:
#     2020-07-26
# Type:
#     <Bash/Makefile/YAML> <script/config file>
# Description:
#     Kino Raspberry Pi flight software - build script
# Dependencies:
#     <none>
#-------------------------------------------------------------------#

## PROJECT SETTINGS

cmake_minimum_required(VERSION 3.13.4) # Raspberry Pi default

project(kino-rpi DESCRIPTION "Kino Avionics - RPI Flight Software"
                 LANGUAGES C)

# Build settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./")
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # for shared libs; delete in future if unnecessary
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pthread")

# -pthread Adds support for multithreading with the pthreads library.
# This option sets flags for both the preprocessor and linker.

# pigpio C build example:
# gcc -Wall -pthread -o foobar foobar.c -lpigpio -lrt

#-------------------------------------------------------------------#

## BUILD SOURCE

# Build libraries 
set(pigpio_DIR "./pigpio/cmake")
find_package(pigpio CONFIG REQUIRED)

# Build flight software
set(SRC "./exec.c")
add_executable(kino_rpi ${SRC})
target_include_directories(kino_rpi PUBLIC "./pigpio")
target_link_libraries(kino_rpi PUBLIC pigpio rt)