include_directories(utils systemSolver)

add_subdirectory(systemSolver)
add_subdirectory(ui)
add_subdirectory(utils)

add_executable(LorenzSystemSolver main.cpp)
target_link_libraries(LorenzSystemSolver UI Solver)
