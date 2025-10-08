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

# Bar Chart Example
add_executable(bar_chart
    bar_chart.cpp
)

target_link_libraries(bar_chart
    PRIVATE
    QTradingView
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

set_target_properties(bar_chart PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)

if(WIN32)
    add_custom_command(TARGET bar_chart POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:QTradingView>
        $<TARGET_FILE_DIR:bar_chart>
    )
endif()
