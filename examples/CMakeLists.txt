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

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Simple Line Chart Example
add_executable(simple_line_chart
    simple_line_chart.cpp
)

target_link_libraries(simple_line_chart
    PRIVATE
    QTradingView
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

set_target_properties(simple_line_chart PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)

if(WIN32)
    add_custom_command(TARGET simple_line_chart POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:QTradingView>
        $<TARGET_FILE_DIR:simple_line_chart>
    )
endif()

add_custom_command(TARGET simple_line_chart POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/examples/btc-usd.csv
        $<TARGET_FILE_DIR:simple_line_chart>/btc-usd.csv
)

# Candlestick Chart Example
add_executable(candlestick_chart
    candlestick_chart.cpp
)

target_link_libraries(candlestick_chart
    PRIVATE
    QTradingView
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

set_target_properties(candlestick_chart PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)

if(WIN32)
    add_custom_command(TARGET candlestick_chart POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:QTradingView>
        $<TARGET_FILE_DIR:candlestick_chart>
    )
endif()

add_custom_command(TARGET simple_line_chart POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/examples/btc-usd.csv
        $<TARGET_FILE_DIR:simple_line_chart>/btc-usd.csv
)

# Multi-Pane Chart Example
add_executable(multi_pane_chart
    multi_pane_chart.cpp
)

target_link_libraries(multi_pane_chart
    PRIVATE
    QTradingView
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

set_target_properties(multi_pane_chart PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)

if(WIN32)
    add_custom_command(TARGET multi_pane_chart POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:QTradingView>
        $<TARGET_FILE_DIR:multi_pane_chart>
    )
endif()

add_custom_command(TARGET simple_line_chart POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/examples/btc-usd.csv
        $<TARGET_FILE_DIR:simple_line_chart>/btc-usd.csv
)
