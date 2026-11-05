# see https://cmake.org/cmake/help/book/mastering-cmake/chapter/Cross%20Compiling%20With%20CMake.html
set(CMAKE_SYSTEM_NAME Windows)

# path to the cmake(++) compiler
set(CMAKE_C_COMPILER /home/ole/mnt/llvm-mingw-20261020-ucrt-ubuntu-22.04-aarch64/bin/aarch64-w64-mingw32-clang)
set(CMAKE_CXX_COMPILER /home/ole/mnt/llvm-mingw-20261020-ucrt-ubuntu-22.04-aarch64/bin/aarch64-w64-mingw32-clang++)

# path to the llvm-mingw env (so without /bin/clang suffix)
set(CMAKE_FIND_ROOT_PATH /home/ole/mnt/llvm-mingw-20261020-ucrt-ubuntu-22.04-aarch64/)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

add_compile_definitions(_WIN_EXPORT)