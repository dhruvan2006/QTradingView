// Copyright (c) 2025 Dhruvan Gnanadhandayuthapani
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <QApplication>
#include <QMainWindow>
#include <QDateTime>
#include "QTradingView/Chart.h"
#include "QTradingView/Data.h"
#include "QTradingView/series/CandleStickSeries.h"
#include "QTradingView/series/BarSeries.h"
#include "QTradingView/series/LineSeries.h"
#include "QTradingView/scale/LinearScale.h"
#include "QTradingView/scale/LogScale.h"
#include "QTradingView/scale/ScaleType.h"
#include <random>
#include <algorithm>
#include <cmath>
#include "utils.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("QTradingView Multi-Pane Complex Example");
    window.resize(1600, 900);

    // === CHART ===
    auto chart = new QTradingView::Chart(&window);
    window.setCentralWidget(chart);

    // Pane 1: Price + SMA
    auto candles = Utils::loadCandles(":/btc-usd.csv");
    auto pricePane = chart->addPane(2.0);
    pricePane->setScale(QTradingView::ScaleType::Logarithmic);
    auto candleSeries = std::make_shared<QTradingView::CandleStickSeries>(candles);
    pricePane->addSeries(candleSeries);

    auto smaData = Utils::computeSMA(candles, 20);
    auto smaSeries = std::make_shared<QTradingView::LineSeries>(smaData);
    smaSeries->setColor(QColor(255, 152, 0));
    pricePane->addSeries(smaSeries);

    // Pane 2: MACD
    auto macdPane = chart->addPane(0.8);
    macdPane->setScale(QTradingView::ScaleType::Linear);
    auto macdResult = Utils::computeMACD(candles);
    auto macdLineSeries = std::make_shared<QTradingView::LineSeries>(macdResult.macdLine);
    macdLineSeries->setColor(QColor(33,150,243));
    macdPane->addSeries(macdLineSeries);
    auto signalLineSeries = std::make_shared<QTradingView::LineSeries>(macdResult.signalLine);
    signalLineSeries->setColor(QColor(255,87,34));
    macdPane->addSeries(signalLineSeries);
    auto histogramSeries = std::make_shared<QTradingView::BarSeries>(macdResult.histogram);
    macdPane->addSeries(histogramSeries);

    // Fit to data and set theme
    chart->setTheme(QTradingView::ChartTheme::tradingViewDark());
    chart->show();

    window.show();
    return app.exec();
}
