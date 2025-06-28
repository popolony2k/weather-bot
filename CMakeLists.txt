#
# Copyright (c) since 2025 by PopolonY2k and Leidson Campos A. Ferreira
# 
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
# claim that you wrote the original software. If you use this software
# in a product, an acknowledgment in the product documentation would be
# appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
# misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#

cmake_minimum_required (VERSION 3.24)

project (weather_bot)

include(CMakeDependentOption)
include(GNUInstallDirs)
include(FetchContent)
include(ExternalProject)
include(FindCURL)

option(WEATHER_LIB_IS_STATIC "Build weather library as static" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(JSON-C REQUIRED)
find_package(CURL REQUIRED)
#find_package(APR REQUIRED)

# Add all include files
include_directories("src")
include_directories(${JSON-C_INCLUDE_DIR})

# Add all source files
set(APPLICATION_SOURCES
   ${PROJECT_SOURCE_DIR}/src/c/main.c
   ${PROJECT_SOURCE_DIR}/src/c/user_input.c)

# Add all header files
set(APPLICATION_HEADERS
    ${PROJECT_SOURCE_DIR}/src/c/weatherrs_lib.h
    ${PROJECT_SOURCE_DIR}/src/c/user_input.h)

#==================================================
# telebot dependency setup
#
# Unfortunately the telebot project is not FetchContent_Declare
# friendly, so we need to resolve using the old painful hard way :)
#==================================================

set(BUILD_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
find_program(MAKE_EXE NAMES make gmake nmake)
# This will be used as DESTDIR on subproject's `make install`.
set(telebot_DESTDIR ${CMAKE_CURRENT_BINARY_DIR}/lib/telebot_install)

ExternalProject_Add(telebot
                    GIT_REPOSITORY https://github.com/smartnode/telebot.git
                    GIT_TAG master 
                    PREFIX ${BUILD_PREFIX}
                    SOURCE_DIR ${BUILD_PREFIX}/telebot
                    INSTALL_COMMAND make DESTDIR=${telebot_DESTDIR} install
                    BUILD_COMMAND make)
ExternalProject_Get_Property(telebot SOURCE_DIR BINARY_DIR)

# Add telebot include
include_directories(${SOURCE_DIR}/include)

set(TELEBOT_STATIC_LIB ${BUILD_PREFIX}/src/telebot-build/${CMAKE_SHARED_LIBRARY_PREFIX}telebot_static${CMAKE_STATIC_LIBRARY_SUFFIX})
add_library(telebot_library STATIC IMPORTED)
set_target_properties(telebot_library PROPERTIES IMPORTED_LOCATION ${TELEBOT_STATIC_LIB})

#==================================================
# end of telebot setup
#==================================================

#==================================================
# (Rust) weather-lib-rs dependency setup
#==================================================

set(WEATHER_BOT_BUILD_TYPE ${CMAKE_BUILD_TYPE})

if (NOT DEFINED WEATHER_BOT_BUILD_TYPE)
    set(WEATHER_BOT_BUILD_TYPE "Debug")
endif()

find_program(CARGO_EXE NAMES cargo)
string(TOLOWER ${WEATHER_BOT_BUILD_TYPE} WEATHER_BOT_BUILD_TYPE)

if (WEATHER_BOT_BUILD_TYPE STREQUAL "debug")
    set(CARGO_CMD ${CARGO_EXE} build --manifest-path ${CMAKE_CURRENT_SOURCE_DIR}/Cargo.toml)
else ()
    set(CARGO_CMD ${CARGO_EXE} build --release --manifest-path ${CMAKE_CURRENT_SOURCE_DIR}/Cargo.toml)
endif ()

set(TARGET_DIR ${CMAKE_CURRENT_BINARY_DIR}/${WEATHER_BOT_BUILD_TYPE})

if (WEATHER_LIB_IS_STATIC)
    set(WEATHERRS_LIB ${TARGET_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}weatherrs${CMAKE_STATIC_LIBRARY_SUFFIX})
else ()
    set(WEATHERRS_LIB ${TARGET_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}weatherrs${CMAKE_SHARED_LIBRARY_SUFFIX})
endif (WEATHER_LIB_IS_STATIC)

add_custom_command(OUTPUT ${WEATHERRS_LIB}
        COMMENT "Compiling rust module"
        COMMAND CARGO_TARGET_DIR=${CMAKE_CURRENT_BINARY_DIR} ${CARGO_CMD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# cbindgen setup to generate Rust->C header bindings
find_program(CBINDGEN_EXE NAMES cbindgen)
set(WEATHER_C_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/c/weatherrs_lib.h)
set(CBINDGEN_CMD ${CBINDGEN_EXE} -l C --crate weather-lib-rs --output ${WEATHER_C_HEADER})

add_custom_command(OUTPUT ${WEATHER_C_HEADER}
        COMMENT "Generating C header file from Rust source"
        COMMAND CBINDGEN_TARGET_DIR=${CMAKE_CURRENT_BINARY_DIR} ${CBINDGEN_CMD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(wheatherrs_lib_target DEPENDS ${WEATHERRS_LIB})

if (WEATHER_LIB_IS_STATIC)
    add_library(weatherrs_library STATIC IMPORTED GLOBAL)
else ()
    add_library(weatherrs_library SHARED IMPORTED GLOBAL)
endif(WEATHER_LIB_IS_STATIC)

set_target_properties(weatherrs_library PROPERTIES IMPORTED_LOCATION ${WEATHERRS_LIB})
add_dependencies(weatherrs_library wheatherrs_lib_target)

#==================================================
# end of (Rust) weather-lib-rs setup
#==================================================

add_executable(weather_bot ${APPLICATION_SOURCES} ${APPLICATION_HEADERS})
target_link_libraries(weather_bot PRIVATE ${JSON-C_LIBRARY} ${CURL_LIBRARY} telebot_library weatherrs_library)
target_include_directories(weather_bot PRIVATE ${SOURCE_DIR}/include)

add_dependencies(weather_bot telebot)

#==================================================
# Tests
#==================================================

add_subdirectory(test)

