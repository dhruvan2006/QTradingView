# QTradingView

[![Build](https://img.shields.io/github/actions/workflow/status/dhruvan2006/QTradingView/build.yml?branch=main)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)]()
[![Demo](https://img.shields.io/badge/demo-online-brightgreen)](https://dhruvan2006.github.io/QTradingView/demo)
[![C++](https://img.shields.io/badge/C++-17-blue)]()
[![Qt](https://img.shields.io/badge/Qt-6-green)]()

A **lightweight, high-performance charting library** built with **C++ and Qt**. Inspired by [TradingView's lightweight-charts](https://github.com/tradingview/lightweight-charts).

Built from scratch to deliver **interactive and scalable visualization** for financial data.

Try it live in your browser: [QTradingView Demo](https://dhruvan2006.github.io/QTradingView/demo)

<p align="center">
  <img src="screenshot.png" alt="QTradingView Demo" width="600"/>
  <br/>
  <em>Interactive multi pane candlestick chart</em>
</p>

## Highlights

- 🕹️ **Interactive Features:** Zooming, panning and crosshair tracking
- 📈 **Chart Types:** Line, Candlestick and Bar charts
- 🎨 **Themes:** TradingView inspired light and dark themes
- ⚡ **Performance:** Optimized for large datasets
- 🧩 **Modular Architecture:** Extensible class hierarchy for adding new series and scale types

## Installation

QTradingView can be easily integrated into your CMake-based project using `FetchContent`:

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
This automatically downloads, builds, and links QTradingView into your project.

Alternatively, if you installed the prebuilt binaries from the releases, you can use `find_package`:

```cmake
find_package(QTradingView CONFIG REQUIRED)

add_executable(MyApp main.cpp)
target_link_libraries(MyApp PRIVATE QTradingView::QTradingView)
```
## Usage

Creating charts is simple and modular. Start with a `Chart` add one or more `Panes` and attach series:

```c++
#include <QApplication>
#include <QTradingView>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    auto chart = new QTradingView::Chart();
    auto pane = chart->addPane(1.0);
    pane->addSeries(std::make_shared<QTradingView::CandleStickSeries>(data));
    chart->show();

    return app.exec();
}
```

More detailed examples can be found in the `examples/` directory.

## Demo

Check out the live demo built with **WebAssembly + Qt**:  
[https://dhruvan2006.github.io/QTradingView/demo](https://dhruvan2006.github.io/QTradingView/demo)

Interact with multi-pane candlestick charts directly in your browser.

## Architecture Overview

QTradingView uses a pane-based architecture inspired by TradingView.
Each Chart owns multiple Pane objects, each rendering independent series (candlestick, line, etc.).

- **Chart** — top-level container managing layout and input.
- **Pane** — isolated rendering unit with its own Y-scale.
- **Series** — modular data renderer (e.g., CandleStickSeries, LineSeries).
- **Viewport** — tracks zoom/pan state and coordinates.

This modular design allows partial redraws and efficient updates even with 10k+ data points.

```mermaid
classDiagram
    %% =========================
    %% CORE CLASSES
    %% =========================
    class Chart {
        +addPane(heightRatio: double): Pane
        +viewport(): ViewPort
        +setTheme(theme: ChartTheme)
        +render(painter: QPainter)
        +fitToData()
    }

    class Pane {
        +addSeries(series: Series)
        +setScale(type: ScaleType)
        +render(painter: QPainter, viewport: ViewPort)
    }

    class ViewPort {
        +setVisibleRange(start: int, end: int)
        +startIndex(): int
        +endIndex(): int
        +barWidth(): double
    }

    %% =========================
    %% ABSTRACT & INTERFACE LAYERS
    %% =========================
    class Series {
        <<abstract>>
        +render(painter: QPainter, viewport: ViewPort)
    }

    class IScale {
        <<interface>>
        +dataToPixel(value: double): double
        +pixelToData(pixel: double): double
        +setDomain(minValue: double, maxValue: double)
        +setRange(minPixel: double, maxPixel: double)
    }

    %% =========================
    %% STYLE
    %% =========================
    class ChartTheme {
        +backgroundColor: QColor
        +gridColor: QColor
        +font: QFont
    }

    %% =========================
    %% SCALE IMPLEMENTATIONS
    %% =========================
    class LinearScale
    class LogScale

    IScale <|-- LinearScale
    IScale <|-- LogScale

    %% =========================
    %% SERIES IMPLEMENTATIONS
    %% =========================
    class LineSeries
    class CandleStickSeries
    class BarSeries

    Series <|-- LineSeries
    Series <|-- CandleStickSeries
    Series <|-- BarSeries

    %% =========================
    %% RELATIONSHIPS
    %% =========================
    Chart "1" --> "*" Pane
    Chart "1" --> "1" ViewPort
    Chart "1" --> "1" ChartTheme

    Pane "1" --> "*" Series
    Pane "1" --> "1" IScale
    Pane "1" --> "1" ViewPort
```

## License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.
