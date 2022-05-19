cmake_minimum_required(VERSION 3.22)
project(shunting_yard)

set(CMAKE_CXX_STANDARD 17)

add_executable(shunting_yard
        main.cpp
        eval.h
        eval.cpp)
