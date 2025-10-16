# Installation

QTradingView can be easily integrated using **CMake**:

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyProject)

include(FetchContent)

FetchContent_Declare(
        QTradingView
        GIT_REPOSITORY https://github.com/dhruvan2006/QTradingView.git
        GIT_TAG v1.0.0
)

FetchContent_MakeAvailable(QTradingView)

add_executable(MyApp main.cpp)
target_link_libraries(MyApp PRIVATE QTradingView::QTradingView)
```