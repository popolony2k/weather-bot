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
project (weather_bot_tests)

include(FetchContent)
include(ExternalProject)

#
# Setup CLang to C++17 (minimum for gtest)
#
set(CMAKE_CXX_STANDARD 17)

#
# Add all weather-bot app include directories
#
include_directories("../src/c/")
include_directories("test//src/c")

set(WEATHER_TESTS_SOURCES
   ${PROJECT_SOURCE_DIR}/src/c/main.cpp 
   ${PROJECT_SOURCE_DIR}/src/c/user_input_tests.cc)

# Add all weather-bot app source files
set(WEATHER_APP_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/../src/c/user_input.c)

##################################
# Download and install GoogleTest

ExternalProject_Add(gtest
  URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
  # Comment above line, and uncomment line below to use subversion.
  # SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/ 
  # Uncomment line below to freeze a revision (here the one for 1.7.0)
  # SVN_REVISION -r700

  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
  INSTALL_COMMAND ""
)
ExternalProject_Get_Property(gtest SOURCE_DIR BINARY_DIR)

################
# Define a test
add_executable(weather_bot_tests
               ${WEATHER_APP_SOURCES}
               ${WEATHER_TESTS_SOURCES})

######################################
# Configure the test to use GoogleTest
#
# If used often, could be made a macro.

add_dependencies(weather_bot_tests gtest)

#
# GTest include and generated link libraries paths
# (respect the link order because GTest is problematic with link order) 
#
include_directories(${SOURCE_DIR}/googletest/include)
include_directories(${SOURCE_DIR}/googlemock/include)
target_link_libraries(weather_bot_tests ${BINARY_DIR}/lib/libgtest.a)
target_link_libraries(weather_bot_tests ${BINARY_DIR}/lib/libgtest_main.a)

##################################
# Just make the test runnable with
#   $ make test

enable_testing()
add_test(NAME    weather_bot_tests 
         COMMAND weather_bot_tests)
