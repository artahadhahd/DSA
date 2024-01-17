cmake_minimum_required(VERSION 3.10)

set(MAKE_C_STANDARD, 99)
set(MAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(DSA)
set(_src_files main.c)

add_executable(DSA ${_src_files})