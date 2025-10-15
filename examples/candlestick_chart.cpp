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
#include "QTradingView/scale/LinearScale.h"
#include <QFile>
#include <QTextStream>

#include "utils.h"
#include "QTradingView/scale/ScaleType.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("QTradingView Candlestick Chart Example");
    window.resize(1400, 700);

    QList<QTradingView::CandleStick> candles = Utils::loadCandles(":/btc-usd.csv");

    auto chart = new QTradingView::Chart(&window);
    window.setCentralWidget(chart);

    auto mainPane = chart->addPane(1.0);
    auto candleSeries = std::make_shared<QTradingView::CandleStickSeries>(candles);
    mainPane->addSeries(candleSeries);
    mainPane->setScale(QTradingView::ScaleType::Logarithmic);

    chart->setTheme(QTradingView::ChartTheme::tradingViewDark());
    chart->fitToData();

    window.show();
    return app.exec();
}
