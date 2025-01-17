#laserKraftWerk

cmake_minimum_required(VERSION 3.15)

# Project Name
project(helios_dac_sdk LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig)
pkg_search_module(LIBUSB REQUIRED libusb-1.0)
add_subdirectory(helios_dac/sdk/cpp/idn)

# Add directories for sources and headers
# Adjust these paths based on your project structure
set(HELIOS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/helios_dac/sdk/cpp)
set(HELIOS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/helios_dac/sdk/cpp ${CMAKE_CURRENT_SOURCE_DIR}/helios_dac/sdk/cpp/idn)

# Add Helios library source files
set(HELIOS_SOURCES
        ${HELIOS_SOURCE_DIR}/HeliosDac.cpp
#        ${HELIOS_SOURCE_DIR}/idn/plt/plt-posix.h
#        ${HELIOS_SOURCE_DIR}/idn/plt/plt-posix.c
)


# Define the Helios SDK library target
add_library(helios_dac STATIC ${HELIOS_SOURCES})

# Include directories
target_include_directories(helios_dac PUBLIC ${HELIOS_INCLUDE_DIR} )


# Link `libusb` to the Helios library
target_link_libraries(helios_dac PUBLIC ${LIBUSB_LIBRARIES} idn)

# Optional: Add an executable to test the SDK with the USB device
#add_executable(helios_test ${HELIOS_SOURCE_DIR}/example.cpp)

# Ensure the test executable links to the Helios library
#target_link_libraries(helios_test PRIVATE helios_dac)