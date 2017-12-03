cmake_minimum_required(VERSION 3.8)
project(ATD_FSoD)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp myCW/FSD.cpp myCW/FSD.h myCW/KP.h myCW/Note.h myCW/InterBase.h)
add_executable(ATD_FSoD ${SOURCE_FILES})