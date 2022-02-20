cmake_minimum_required(VERSION 3.21)
project(ext_sort)

set(CMAKE_CPP_STANDARD 17)

add_executable(ext_sort main.cpp ext_sort.cpp ext_sort.h)
