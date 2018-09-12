cmake_minimum_required(VERSION 3.1)

project(LorenzSystemSolver)

set(CMAKE_CXX_STANDARD 14)
option(USE_QT "Enables working with qt" ON)

include_directories(utils systemSolver)

add_subdirectory(systemSolver)
add_subdirectory(ui)
add_subdirectory(utils)

add_executable(LorenzSystemSolver main.cpp)
target_link_libraries(LorenzSystemSolver UI Solver)
