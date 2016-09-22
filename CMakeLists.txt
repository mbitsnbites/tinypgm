# -*- mode: CMake; tab-width: 2; indent-tabs-mode: nil; -*-

cmake_minimum_required(VERSION 2.8.12)

project(tinypgm)

# Enable warnings, and treat them as errors.
if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall -Wextra -pedantic -Werror")
elseif("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Weverything -Wno-padded -Werror")
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /WX")
endif()

add_subdirectory(src)
add_subdirectory(test)

