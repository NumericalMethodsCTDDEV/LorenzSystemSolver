add_subdirectory(methodsImplementation)
add_library(Solver solver.cpp)
target_link_libraries(Solver MethodsImpl Configs )
