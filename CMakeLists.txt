cmake_minimum_required(VERSION 3.8)
project(ATD_FSoD)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp FSD.cpp FSD.h KP.h Block.h)
add_executable(ATD_FSoD ${SOURCE_FILES})