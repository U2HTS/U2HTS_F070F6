cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME U2HTS_F070F6)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
# Core project settings
project(${CMAKE_PROJECT_NAME} C CXX ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

add_library(u2hts_core_settings INTERFACE)
target_compile_definitions(u2hts_core_settings INTERFACE
    U2HTS_LOG_LEVEL=-1
    U2HTS_ENABLE_LED
    U2HTS_ENABLE_PERSISTENT_CONFIG
    U2HTS_ENABLE_KEY
    U2HTS_CONFIG_TIMEOUT=3000
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/u2hts_core)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/u2hts_touch_controllers/gt9xx.c
    ${CMAKE_CURRENT_LIST_DIR}/u2hts_touch_controllers/ft54x6.c
    ${CMAKE_CURRENT_LIST_DIR}/u2hts_touch_controllers/rmi_common.c
    ${CMAKE_CURRENT_LIST_DIR}/u2hts_touch_controllers/rmi_f11.c
    ${CMAKE_CURRENT_LIST_DIR}/u2hts_touch_controllers/cst8xx.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    u2hts_core
    u2hts_core_settings
    # Add user defined libraries
)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin)
set(LST_FILE ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.lst)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${BIN_FILE}
        COMMAND ${CMAKE_OBJDUMP} --all-headers --demangle --disassemble $<TARGET_FILE:${CMAKE_PROJECT_NAME}> > ${LST_FILE}
        BYPRODUCTS ${HEX_FILE} ${BIN_FILE} ${LST_FILE}
)
