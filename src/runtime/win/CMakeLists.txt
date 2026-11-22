cmake_minimum_required(VERSION 3.22)

project(win-hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -std=c++17 -Wno-unused-function")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

set(INCLUDE_PATH ../../../inc)

add_library(arancini-runtime-win SHARED entry_win.cpp)
add_library(arancini-runtime-win-static STATIC entry_win.cpp)

target_include_directories(arancini-runtime-win PRIVATE ${INCLUDE_PATH})
target_include_directories(arancini-runtime-win-static PRIVATE ${INCLUDE_PATH})

target_link_libraries(arancini-runtime-win PRIVATE ws2_32)
target_link_libraries(arancini-runtime-win-static PRIVATE ws2_32)