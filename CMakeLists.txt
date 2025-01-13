#laserKraftWerk

cmake_minimum_required(VERSION 3.15)

# Project Name
project(helios_dac_sdk LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add directories for sources and headers
# Adjust these paths based on your project structure
set(HELIOS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/helios_dac/sdk/cpp)
set(HELIOS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/helios_dac/sdk/cpp)

# Add Helios library source files
set(HELIOS_SOURCES
        ${HELIOS_SOURCE_DIR}/HeliosDac.cpp
)

# Add the `libusb` library path
# Normally the shared library (`.so`) or `.lib` will be in the `sdk` folder
set(LIBUSB_LIB_PATH ${HELIOS_SOURCE_DIR}/libusb-1.0.so)
set(LIBUSB_INCLUDE_PATH ${HELIOS_SOURCE_DIR})

# Define the Helios SDK library target
add_library(helios_dac ${HELIOS_SOURCES})

# Include directories
target_include_directories(helios_dac PUBLIC ${HELIOS_INCLUDE_DIR} ${LIBUSB_INCLUDE_PATH})

# Link `libusb` to the Helios library
target_link_libraries(helios_dac PRIVATE ${LIBUSB_LIB_PATH})

# Optional: Add an executable to test the SDK with the USB device
#add_executable(helios_test ${HELIOS_SOURCE_DIR}/example.cpp)

# Ensure the test executable links to the Helios library
#target_link_libraries(helios_test PRIVATE helios_dac)