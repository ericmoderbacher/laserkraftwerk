#laserKraftWerk

cmake_minimum_required(VERSION 3.15)

# Project Name
project(laserKraftWerk LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(helios_dac)

add_library(laserKraftWerker SHARED laserKraftWerker.cpp )
target_include_directories(laserKraftWerker PUBLIC include)
target_link_libraries(laserKraftWerker PUBLIC helios_dac)
