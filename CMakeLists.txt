cmake_minimum_required(VERSION 3.16)
project(PCForest)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_definitions(-DUNICODE -D_UNICODE)

add_executable(PCForest main.cpp BiMap.hpp PCForest.hpp Printer.h Printer.cpp Tester.cpp Tester.h)