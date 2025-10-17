# Usage

This page provides practical usage patterns and small examples to get you started with QTradingView.

## Basic Chart (minimal Qt app)

```c++
#include <QApplication>
#include <QTradingView>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    auto chart = new QTradingView::Chart();
    auto pane = chart->addPane(1.0);

    // `data` should be provided in the format expected by the series (see examples)
    pane->addSeries(std::make_shared<QTradingView::CandleStickSeries>(data));

    chart->show();
    return app.exec();
}
```

## Data format (candlestick example)

QTradingView uses simple structs for data points. For candlesticks, provide a container of `CandleStick` items (timestamp, open, high, low, close, volume). See the generated API docs under `docs/api/` for exact struct fields.

## API overview

- Chart
  - `addPane(heightRatio)` — create and return a `Pane`.
  - `viewport()` — access the shared viewport for controlling visible range and bar width.
  - `setTheme(theme)` — set built-in themes (light/dark).
  - `fitToData()` — adjust viewport to fit currently attached series.

- Pane
  - `addSeries(shared_ptr<Series>)` — attach a series to a pane.
  - `setScale(ScaleType)` — choose linear or logarithmic scales.

- Series
  - Derived series classes such as `CandleStickSeries`, `LineSeries`, and `BarSeries` expose constructors that accept containers of data.

- ViewPort
  - `setVisibleRange(startIndex, endIndex)` — manually set which range of bars is visible.
  - `startIndex()` / `endIndex()` — query visible indices.

## Themes

```c++
chart->setTheme(QTradingView::ChartTheme::tradingViewDark());
chart->setTheme(QTradingView::ChartTheme::tradingViewLight());
```

## Common tasks

- Fit the chart to current data:

```c++
chart->fitToData();
```

- Update series data:

Replace the underlying container and trigger a repaint or call the series' update/refresh API (see the series class documentation in `docs/api/`).

## Examples

See `docs/examples/` for a ready-to-run candlestick example and instructions to run the examples in this repository.

If you need a snippet not covered here, check the API docs generated under `docs/api/` or open an issue describing what you want to do.
