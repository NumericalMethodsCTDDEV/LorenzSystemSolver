cmake_minimum_required(VERSION 3.1)

project(LorenzSystemSolver)

set(CMAKE_CXX_STANDARD 14)

option(USE_QT "Enables working with qt" ON)

add_subdirectory(src)
