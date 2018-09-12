set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets 5.9.2 REQUIRED)
find_package(Qt5DataVisualization 5.9.2 REQUIRED CONFIG)
#find_package(Gnuplot 5.2.4 REQUIRED)
find_package(Qt5Charts 5.9.2 REQUIRED)

add_library(UI scatterdatamodifier.cpp frontend.cpp)
target_link_libraries(UI Qt5::Widgets Qt5::DataVisualization Qt5::Charts Configs)
