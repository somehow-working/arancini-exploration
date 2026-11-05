cmake_minimum_required(VERSION 3.22)
project(arancini-ir)

# Compile with -fPIC
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(IR_SRCS debug-visitor.cpp default-ir-builder.cpp dot-graph-generator.cpp)

add_library(arancini-ir SHARED ${IR_SRCS})
add_library(arancini-ir-static STATIC ${IR_SRCS})

set(INCLUDE_PATH ../../inc)
target_include_directories(arancini-ir PUBLIC ${INCLUDE_PATH} PUBLIC "${CMAKE_BINARY_DIR}/install/include")
target_include_directories(arancini-ir-static PUBLIC ${INCLUDE_PATH} PUBLIC "${CMAKE_BINARY_DIR}/install/include")

#set(xed_DIR "${CMAKE_BINARY_DIR}/install/lib/cmake/XED/")
find_library(xed xed REQUIRED NO_CMAKE_INSTALL_PREFIX PATHS "${CMAKE_BINARY_DIR}/install/lib")
#find_package(xed REQUIRED NO_CMAKE_INSTALL_PREFIX PATHS "${CMAKE_BINARY_DIR}/install/lib")
#set_target_properties(xed IMPORTED_LOCATION_NOCONFIG "${CMAKE_BINARY_DIR}/install/lib/libxed.a")
#link_directories("${CMAKE_BINARY_DIR}/install/lib")
add_library(xed STATIC IMPORTED)
set_target_properties(xed PROPERTIES
    IMPORTED_CONFIGURATIONS "NOCONFIG"
    IMPORTED_LOCATION_NOCONFIG "${CMAKE_BINARY_DIR}/install/lib/"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/install/include"
)

message(STATUS "IS THERE: ${XED_FOUND}")

find_package(fmt)

target_link_libraries(arancini-ir PUBLIC xed fmt::fmt)
target_link_libraries(arancini-ir-static PUBLIC xed)

# We need to wait for XED to be build first In nix this is already ensured
if(NOT DEFINED ENV{FLAKE_BUILD})
  add_dependencies(arancini-ir external-xed)
  add_dependencies(arancini-ir-static external-xed)
endif() # NIX
