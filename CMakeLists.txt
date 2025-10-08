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

project(QTradingView LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

file(GLOB_RECURSE HEADER_FILES
        include/QTradingView.h
        include/qtradingview_global.h
        include/Chart.h
        include/ViewPort.h
        include/Pane.h

        include/renderer/AxisRenderer.h
        include/renderer/GridRenderer.h
        include/renderer/CrosshairRenderer.h

        include/data/TimePoint.h
        include/data/DataPoint.h
        include/data/CandleStick.h
        include/data/Bar.h
        include/data/IDataProvider.h
        include/data/DataProvider.h
        include/data/CandleStickProvider.h
        include/data/BarProvider.h

        include/scale/IScale.h
        include/scale/LinearScale.h

        include/series/ISeries.h
        include/series/LineSeries.h
        include/series/CandleStickSeries.h
        include/series/BarSeries.h

        include/style/SeriesStyle.h
        include/style/LineStyle.h
        include/style/CandleStickStyle.h
        include/style/BarStyle.h
        include/style/ChartTheme.h
)
file(GLOB_RECURSE SOURCE_FILES
        src/QTradingView.cpp
        src/Chart.cpp
        src/ViewPort.cpp
        src/Pane.cpp

        src/renderer/AxisRenderer.cpp
        src/renderer/GridRenderer.cpp
        src/renderer/CrosshairRenderer.cpp

        src/data/TimePoint.cpp
        src/data/DataPoint.cpp
        src/data/DataProvider.cpp
        src/data/CandleStick.cpp
        src/data/CandleStickProvider.cpp
        src/data/Bar.cpp
        src/data/BarProvider.cpp

        src/scale/LinearScale.cpp

        src/series/LineSeries.cpp
        src/series/CandleStickSeries.cpp
        src/series/BarSeries.cpp

        src/style/SeriesStyle.cpp
        src/style/LineStyle.cpp
        src/style/ChartTheme.cpp
)

add_library(QTradingView SHARED
        ${HEADER_FILES}
        ${SOURCE_FILES}
)

target_include_directories(QTradingView PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(QTradingView PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(QTradingView PRIVATE QTRADINGVIEW_LIBRARY)

# Build examples (optional)
option(BUILD_EXAMPLES "Build example applications" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
