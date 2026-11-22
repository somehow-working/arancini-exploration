cmake_minimum_required(VERSION 3.22)
project(get-clang-mingw)

message("Getting clang-mingw cross compiler for windows")

set(CLANG_MINGW_PREFIX "clang-mingw-prefix")

include(ExternalProject)

# have not seen a way to only download specific assets from a git release so we just use URL
ExternalProject_Add(
    clang-mingw
    URL "https://github.com/mstorsjo/llvm-mingw/releases/download/20261104/llvm-mingw-20261104-ucrt-ubuntu-22.04-aarch64.tar.xz"
    CONFIGURE_COMMAND "${CMAKE_COMMAND}" -E true
    BUILD_COMMAND "${CMAKE_COMMAND}" -E true
    INSTALL_COMMAND "${CMAKE_COMMAND}" -E true
    PREFIX "${CLANG_MINGW_PREFIX}"
)

set(CLANG_MINGW_PATH "${CMAKE_BINARY_DIR}/${CLANG_MINGW_PREFIX}/src/clang-mingw/")
set(CLANG_MINGW_C_PATH "${CLANG_MINGW_PATH}/bin/aarch64-w64-mingw32-clang")
set(CLANG_MINGW_CXX_PATH "${CLANG_MINGW_PATH}/bin/aarch64-w64-mingw32-clang++")

unset(CLANG_MINGW_PREFIX)