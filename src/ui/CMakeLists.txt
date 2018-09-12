add_library(GnuPlotSupport gnuplot_support_shared.cpp)

if (USE_QT)
    add_subdirectory(qtBasedUi)
else()
    add_subdirectory(console)
endif()
