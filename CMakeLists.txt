#
# Copyright (C) 2023 Alibaba Group Holding Limited
#

cmake_minimum_required(VERSION 3.5.1)
project(prd_utils C)
set(CMAKE_C_STANDARD 99)

# Add fpic
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Board Selection
if (${BOARD_NAME} MATCHES "light*")
  set(BOARD_NAME "light")
  include(cmake/board_light.cmake)
else ()
  message(FATAL_ERROR, "No BOARD_NAME")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -g")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

include(cmake/show_cmake_param.cmake)


# Add include dirs for all sub modules
include_directories(${PROJECT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(platform/${BOARD_NAME})
add_subdirectory(utils)
add_subdirectory(test)

install(FILES ${INSTALL_HEAD_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
