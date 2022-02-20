cmake_minimum_required(VERSION 3.21)
project(ext_sort)

set(CMAKE_CPP_STANDARD 17)

set(SOURCE_FILES
        main.cpp
        ext_sort.cpp
        ext_sort.h)
add_executable(ext_sort ${SOURCE_FILES})
