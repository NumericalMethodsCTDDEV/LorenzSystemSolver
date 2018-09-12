find_package(Boost 1.66.0 REQUIRED)

add_library(MethodsImpl shared.cpp euler_implicit_impl.cpp euler_explicit_impl.cpp
    runge_kutta_impl.cpp adams_explicit_impl.cpp)
