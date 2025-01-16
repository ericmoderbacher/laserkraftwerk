cmake_minimum_required(VERSION 3.15)

# Project Name
project(helios_dac_sdk LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directories for sources and headers
set(HELIOS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/helios_dac/sdk)
set(HELIOS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/helios_dac/sdk)

# Add Helios library source files
set(HELIOS_SOURCES
        ${HELIOS_SOURCE_DIR}/cpp/HeliosDac.cpp
        ${HELIOS_SOURCE_DIR}/cpp/idn/idn.cpp
        ${HELIOS_SOURCE_DIR}/cpp/idn/idnServerList.cpp
)

# Validate that source files exist
foreach(SRC_FILE ${HELIOS_SOURCES})
    if (NOT EXISTS ${SRC_FILE})
        message(FATAL_ERROR "Source file ${SRC_FILE} not found!")
    endif()
endforeach()

# Locate libraries
find_library(LIBUSB_LIBRARY
        NAMES usb-1.0
        PATHS /opt/homebrew/Cellar/libusb/1.0.27/lib/
)

find_library(COREFOUNDATION_LIBRARY CoreFoundation)
find_library(IOKIT_LIBRARY IOKit)
find_library(SECURITY_LIBRARY Security)

# Validate libraries
if (NOT LIBUSB_LIBRARY)
    message(FATAL_ERROR "libusb not found")
endif()
if (NOT COREFOUNDATION_LIBRARY)
    message(FATAL_ERROR "CoreFoundation not found")
endif()
if (NOT IOKIT_LIBRARY)
    message(FATAL_ERROR "IOKit not found")
endif()
if (NOT SECURITY_LIBRARY)
    message(FATAL_ERROR "Security not found")
endif()

# Define the Helios SDK library
add_library(helios_dac STATIC ${HELIOS_SOURCES})

# Include directories for Helios
target_include_directories(helios_dac PUBLIC
        ${HELIOS_INCLUDE_DIR}
        ${HELIOS_SOURCE_DIR}/cpp
        ${HELIOS_SOURCE_DIR}/cpp/idn
)

# Link dependencies to Helios
target_link_libraries(helios_dac PUBLIC
        ${LIBUSB_LIBRARY}
        ${COREFOUNDATION_LIBRARY}
        ${IOKIT_LIBRARY}
        ${SECURITY_LIBRARY}
)

# Add the example executable
add_executable(example ${HELIOS_SOURCE_DIR}/examples/cpp/main.cpp)

# Include directories for example
target_include_directories(example PUBLIC ${HELIOS_INCLUDE_DIR} ${HELIOS_SOURCE_DIR}/cpp/idn)

# Link example to Helios (dependencies are transitive)
target_link_libraries(example PUBLIC helios_dac)
