set(CMAKE_SYSTEM_NAME Windows)

set(CMAKE_C_COMPILER /home/ole/mnt/arancini-exploration/build-server/clang-mingw-prefix/src/clang-mingw//bin/aarch64-w64-mingw32-clang)
set(CMAKE_CXX_COMPILER /home/ole/mnt/arancini-exploration/build-server/clang-mingw-prefix/src/clang-mingw//bin/aarch64-w64-mingw32-clang++)

set(CMAKE_FIND_ROOT_PATH /home/ole/mnt/arancini-exploration/build-server/clang-mingw-prefix/src/clang-mingw/)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

add_compile_definitions(_WIN_EXPORT)
