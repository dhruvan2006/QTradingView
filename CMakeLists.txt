# Copyright (c) 2025 Dhruvan Gnanadhandayuthapani
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.16)

project(QTradingView VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

file(GLOB HEADER_FILES
        include/QTradingView/QTradingView.h
        include/QTradingView/qtradingview_global.h
        include/QTradingView/Chart.h
        include/QTradingView/ViewPort.h
        include/QTradingView/Pane.h

        include/QTradingView/renderer/AxisRenderer.h
        include/QTradingView/renderer/GridRenderer.h
        include/QTradingView/renderer/CrosshairRenderer.h

        include/QTradingView/scale/IScale.h
        include/QTradingView/scale/LinearScale.h
        include/QTradingView/scale/LogScale.h
        include/QTradingView/scale/ScaleType.h

        include/QTradingView/series/Series.h
        include/QTradingView/series/LineSeries.h
        include/QTradingView/series/CandleStickSeries.h
        include/QTradingView/series/BarSeries.h

        include/QTradingView/style/ChartTheme.h
)
file(GLOB SOURCE_FILES
        src/QTradingView.cpp
        src/Chart.cpp
        src/ViewPort.cpp
        src/Pane.cpp

        src/renderer/AxisRenderer.cpp
        src/renderer/GridRenderer.cpp
        src/renderer/CrosshairRenderer.cpp

        src/scale/LinearScale.cpp
        src/scale/LogScale.cpp

        src/series/LineSeries.cpp
        src/series/CandleStickSeries.cpp
        src/series/BarSeries.cpp

        src/style/SeriesStyle.cpp
        src/style/LineStyle.cpp
        src/style/ChartTheme.cpp
)

if(EMSCRIPTEN)
    add_library(QTradingView STATIC
            ${HEADER_FILES}
            ${SOURCE_FILES}
    )
else()
    add_library(QTradingView SHARED
            ${HEADER_FILES}
            ${SOURCE_FILES}
    )
endif()

add_library(QTradingView::QTradingView ALIAS QTradingView)

target_include_directories(QTradingView
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(QTradingView PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(QTradingView PRIVATE QTRADINGVIEW_LIBRARY)

set_target_properties(QTradingView PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
)

# Installation rules
include(GNUInstallDirs)
install(TARGETS QTradingView
        EXPORT QTradingViewTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export targets for find_package
install(EXPORT QTradingViewTargets
        FILE QTradingViewTargets.cmake
        NAMESPACE QTradingView::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QTradingView
)

# Create Config file for find_package
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/QTradingViewConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/QTradingViewConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/QTradingViewConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QTradingView
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/QTradingViewConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/QTradingViewConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QTradingView
)

# Build examples (optional)
option(BUILD_EXAMPLES "Build example applications" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
