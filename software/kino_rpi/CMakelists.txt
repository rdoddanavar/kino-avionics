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
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # for shared libs; delete in future if unnecessary

#Examples of C pigpio programs.
#If your program is called foobar.c then build with
#gcc -Wall -pthread -o foobar foobar.c -lpigpio -lrt

#-------------------------------------------------------------------#

## BUILD SOURCE

# Build libraries 
add_subdirectory(pigpio)

# Build flight software
set(SRC exec.c)
#set(HEADER )
add_executable(kino_rpi ${SRC})
target_include_directories(kino_rpi pigpio)
target_link_libraries(kino_rpi pigpio)