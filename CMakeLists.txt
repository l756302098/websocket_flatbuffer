cmake_minimum_required(VERSION 3.0)

project(websocket_flatbuffer VERSION 1.0)

add_definitions(-std=c++17)

if(HOST_ARCH STREQUAL "x86_64")
    set(PLATFORM "x64")
else()
    set(PLATFORM "arm")
endif()

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/${PLATFORM})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/websocketpp)

add_subdirectory(${PROJECT_SOURCE_DIR}/plugins/hmi)
