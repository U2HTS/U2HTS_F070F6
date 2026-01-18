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
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Disable logging to save memory
set(U2HTS_F070F6_SWCLK_AS_UART OFF)
# See u2hts_core/u2hts_core.h line 16 for log levels
set(U2HTS_LOG_LEVEL -1)

if(NOT U2HTS_F070F6_SWCLK_AS_UART)
    if(NOT U2HTS_LOG_LEVEL EQUAL -1)
        message(FATAL_ERROR "Set U2HTS_LOG_LEVEL > -1 requires set U2HTS_F070F6_SWCLK_AS_UART to ON!")
    endif()
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/u2hts_core)

add_library(u2hts_core_settings INTERFACE)
target_compile_definitions(u2hts_core_settings INTERFACE
    U2HTS_LOG_LEVEL=${U2HTS_LOG_LEVEL}
     $<$<BOOL:${U2HTS_F070F6_SWCLK_AS_UART}>:U2HTS_F070F6_SWCLK_AS_UART>
    U2HTS_ENABLE_LED
    U2HTS_ENABLE_PERSISTENT_CONFIG
    U2HTS_ENABLE_KEY
    U2HTS_CONFIG_TIMEOUT=3000
)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Core/Src/u2hts_f070f6.c
    ${CMAKE_CURRENT_LIST_DIR}/Core/Src/gpio_i2c.c
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
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

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    # Add user defined libraries
    u2hts_core
    u2hts_core_settings
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
