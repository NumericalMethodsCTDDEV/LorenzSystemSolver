cmake_minimum_required(VERSION 3.1)

project(LorenzSystemSolver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

option(USE_QT "Enables working with qt" ON)

add_subdirectory(src)
