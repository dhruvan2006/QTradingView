# Multi-Pane Chart Example

This example demonstrates a chart with multiple panes (e.g., price + indicator). Each pane can host one or more `Series` types and have an independent Y-scale.

Example (outline):

```c++
#include <QApplication>
#include <QTradingView>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    // Prepare price data and indicator data
    std::vector<QTradingView::CandleStick> price;
    std::vector<QTradingView::DataPoint> indicator;
    // Fill data...

    auto chart = new QTradingView::Chart();

    // Main price pane (larger)
    auto pricePane = chart->addPane(3.0);
    pricePane->addSeries(std::make_shared<QTradingView::CandleStickSeries>(price));

    // Indicator pane (smaller)
    auto indicatorPane = chart->addPane(1.0);
    indicatorPane->addSeries(std::make_shared<QTradingView::LineSeries>(indicator));

    chart->fitToData();
    chart->show();

    return app.exec();
}
```

Notes:
- Use different height ratios when adding panes to control layout.
- Each pane can use `setScale` to choose linear/log scales.
- Use `chart->viewport()` to programmatically pan/zoom and sync behavior between panes if needed.

