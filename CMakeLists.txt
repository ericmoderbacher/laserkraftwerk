#laserKraftWerk

cmake_minimum_required(VERSION 3.15)

# Project Name
project(helios_dac_sdk LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



find_package(PkgConfig)

if (PKG_CONFIG_FOUND)
    message(hi)
else()
    message(WARNING "pkg-config not found, attempting manual libusb search.")
endif()


pkg_search_module(LIBUSB REQUIRED libusb-1.0)


if (NOT LIBUSB_FOUND)
    # macOS: Use Homebrew's default paths for libusb
    if (APPLE)
        find_path(LIBUSB_INCLUDE_DIR libusb.h PATHS /usr/local/include /opt/homebrew/include)
        find_library(LIBUSB_LIBRARY usb-1.0 PATHS /usr/local/lib /opt/homebrew/lib)
    endif()
endif ()

message(STATUS "LIBUSB_LIBRARIES: ${LIBUSB_LIBRARIES}")
message(STATUS "LIBUSB_INCLUDE_DIRS: ${LIBUSB_INCLUDE_DIRS}")

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


