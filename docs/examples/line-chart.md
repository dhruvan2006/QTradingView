# Line Chart Example

A simple line chart example using `LineSeries`.

```c++
#include <QApplication>
#include <QTradingView>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    std::vector<QTradingView::DataPoint> points;
    points.push_back({0, 100.0});
    points.push_back({1, 101.5});
    points.push_back({2, 98.25});
    // ... more points

    auto chart = new QTradingView::Chart();
    auto pane = chart->addPane(1.0);
    pane->addSeries(std::make_shared<QTradingView::LineSeries>(points));

    chart->fitToData();
    chart->show();

    return app.exec();
}
```

This is useful for plotting indicators, simple price series, or overlays.

