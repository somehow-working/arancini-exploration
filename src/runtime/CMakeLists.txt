cmake_minimum_required(VERSION 3.22)
project(arancini-runtime LANGUAGES CXX ASM)

# Compile with -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(RUNTIME_SRCS entry.cpp exec/execution-thread.cpp exec/execution-context.cpp
                 exec/x86/x86-cpu-state.cpp dbt/translation-engine.cpp)

set(INCLUDE_PATH ../../inc)

add_library(arancini-runtime SHARED ${RUNTIME_SRCS})
add_library(arancini-runtime-static STATIC ${RUNTIME_SRCS})
add_executable(arancini-runtime-server exec_ctx_server.cpp)

if(TARGET arancini-output-x86)
  add_library(arancini-trampoline OBJECT dbt/trampoline.x86_64.S)
  target_link_libraries(arancini-runtime PUBLIC arancini-output-x86)
elseif(TARGET arancini-output-arm64)
  add_library(arancini-trampoline OBJECT dbt/trampoline.arm64.S)
  target_link_libraries(arancini-runtime PUBLIC arancini-output-arm64)
elseif(TARGET arancini-output-riscv64)
  add_library(arancini-trampoline OBJECT dbt/trampoline.riscv64.S)
  target_link_libraries(arancini-runtime PUBLIC arancini-output-riscv64)
  target_link_libraries(arancini-runtime-static
                        PRIVATE arancini-output-riscv64-static)
else()
  message(FATAL_ERROR "No output DBT library was built; set ARCH correctly")
endif()

target_link_libraries(
  arancini-runtime PRIVATE xed arancini-ir arancini-input-x86
                           arancini-trampoline arancini-logger)
target_link_libraries(
  arancini-runtime-static
  PRIVATE xed arancini-ir-static arancini-input-x86-static arancini-trampoline
          arancini-logger)

#target_link_libraries(arancini-runtime-server PRIVATE )
target_include_directories(arancini-runtime-server PRIVATE ${INCLUDE_PATH})

install(TARGETS arancini-runtime LIBRARY)
install(TARGETS arancini-runtime-server RUNTIME)

# We need to wait for XED to be build first In nix this is already ensured
if(NOT DEFINED ENV{FLAKE_BUILD})
  add_dependencies(arancini-runtime external-xed)
  add_dependencies(arancini-runtime-static external-xed)
endif() # NIX

include(ExternalProject)

if(NOT DEFINED CLANG_MINGW_PATH)
  message(FATAL_ERROR "This should be defined here!")
endif()


configure_file("${CMAKE_CURRENT_SOURCE_DIR}/win/TC-mingw-in.cmake" "${CMAKE_CURRENT_SOURCE_DIR}/win/TC-mingw.cmake" @ONLY)

ExternalProject_Add(
    win-rt
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/win/"
    CMAKE_ARGS "-DCMAKE_TOOLCHAIN_FILE=<SOURCE_DIR>/TC-mingw.cmake"
    #BUILD_COMMAND ${CMAKE_COMMAND} --build ./build
    INSTALL_COMMAND "${CMAKE_COMMAND}" -E copy <BINARY_DIR>/libarancini-runtime-win.dll <BINARY_DIR>/libarancini-runtime-win-static.a "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/"
    PREFIX win-rt-prefix
    DEPENDS clang-mingw
)