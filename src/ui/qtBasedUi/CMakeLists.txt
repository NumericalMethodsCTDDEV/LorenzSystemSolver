set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5DataVisualization REQUIRED CONFIG)
find_package(Gnuplot REQUIRED)
find_package(Qt5Charts REQUIRED)

add_library(UI scatterdatamodifier.cpp frontend.cpp)
target_link_libraries(UI Qt5::Widgets Qt5::DataVisualization Qt5::Charts Configs)
