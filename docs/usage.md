# Usage

## Basic Chart

```c++
auto chart = new QTradingView::Chart();
auto pane = chart->addPane(1.0);
pane->addSeries(std::make_shared<QTradingView::CandleStickSeries>(data));
chart->show();
```

[//]: # (TODO: Add more usage examples and details.)

## Themes
```c++
chart->setTheme(QTradingView::ChartTheme::tradingViewDark());
chart->setTheme(QTradingView::ChartTheme::tradingViewLight());
```
