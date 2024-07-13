cmake_minimum_required(VERSION 3.20...3.30)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CLANG-TIDY_COMMAND "${CLANG_TIDY_EXE} --format-style=file")
set(CMAKE_CXX_CLANG_TIDY CLANG_TIDY_EXE)


project(Game_Of_Life
    VERSION 0.1.0
    DESCRIPTION "C++ Implementation of Conway's Game Of Life"
    LANGUAGES CXX
)

add_subdirectory(src)
add_subdirectory(core)
