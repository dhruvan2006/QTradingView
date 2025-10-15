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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("QTradingView - Multi-Pane Complex Example");
    window.resize(1600, 900);

    auto chart = new QTradingView::Chart(&window);
    window.setCentralWidget(chart);

    // Generate market data
    QList<QTradingView::CandleStick> candles;
    QList<QTradingView::DataPoint> volumeBars;
    QList<QTradingView::DataPoint> macdLine;
    QList<QTradingView::DataPoint> signalLine;
    QList<QTradingView::DataPoint> histogramBars;

    QDateTime startTime = QDateTime::currentDateTime().addDays(-300);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> priceMove(0, 2.5);
    std::uniform_real_distribution<> wickRatio(0.2, 0.8);
    std::uniform_real_distribution<> volumeBase(1000000, 5000000);
    std::normal_distribution<> volumeNoise(0, 500000);

    double basePrice = 200.0;
    double lastClose = basePrice;

    std::vector<double> prices;
    std::vector<qint64> timePoints;
    const int ema12Period = 12;
    const int ema26Period = 26;
    const int signalPeriod = 9;

    for (int i = 0; i < 300; ++i) {
        qint64 time = startTime.addSecs(i * 86400).toMSecsSinceEpoch();

        double open = lastClose;
        double priceChange = priceMove(gen);

        if (i >= 50 && i < 120) {
            priceChange += 0.5;
        } else if (i >= 180 && i < 230) {
            priceChange -= 0.4;
        }

        double close = open + priceChange;
        if (close < 100.0) close = 100.0 + std::abs(priceMove(gen));
        else if (close > 400.0) close = 400.0 - std::abs(priceMove(gen));

        double rangeSize = std::abs(close - open) * 1.8 + std::abs(priceMove(gen));
        double high = std::max(open, close) + rangeSize * wickRatio(gen);
        double low = std::min(open, close) - rangeSize * (1.0 - wickRatio(gen));
        if (low < 80.0) low = 80.0;

        timePoints.push_back(time);
        candles.append(QTradingView::CandleStick{time, open, high, low, close});
        prices.push_back(close);
        lastClose = close;

        double priceChangePercent = std::abs(priceChange) / open;
        double volumeMultiplier = 1.0 + (priceChangePercent * 3.0);
        double volume = volumeBase(gen) * volumeMultiplier + volumeNoise(gen);
        if (volume < 0) volume = volumeBase(gen);

        volumeBars.append(QTradingView::DataPoint{time, volume});
    }

    // MACD
    for (int i = 0; i < ema26Period; ++i) {
        macdLine.append(QTradingView::DataPoint{timePoints[i], std::nan("")});
    }
    for (int i = ema26Period; i < 300; ++i) {
        double sum12 = 0, sum26 = 0;
        for (int j = 0; j < ema12Period; ++j) sum12 += prices[i - j];
        for (int j = 0; j < ema26Period; ++j) sum26 += prices[i - j];
        double ema12 = sum12 / ema12Period;
        double ema26 = sum26 / ema26Period;
        double macdValue = ema12 - ema26;
        macdLine.append(QTradingView::DataPoint{timePoints[i], macdValue});
    }

    int signalStartIndex = ema26Period + signalPeriod - 1;
    for (int i = 0; i < signalStartIndex; ++i) {
        signalLine.append(QTradingView::DataPoint{timePoints[i], std::nan("")});
        histogramBars.append(QTradingView::DataPoint{timePoints[i], 0.0});
    }
    for (int i = signalStartIndex; i < 300; ++i) {
        double signalSum = 0;
        for (int j = 0; j < signalPeriod; ++j) signalSum += macdLine[i - j].value;
        double signalValue = signalSum / signalPeriod;
        signalLine.append(QTradingView::DataPoint{timePoints[i], signalValue});
        double histValue = macdLine[i].value - signalValue;
        histogramBars.append(QTradingView::DataPoint{timePoints[i], histValue});
    }

    // === Setup Chart ===
    chart->fitToData();

    // Pane 1: Price
    auto pricePane = chart->addPane(2.0);
    pricePane->setScale(QTradingView::ScaleType::Logarithmic);
    auto candleSeries = std::make_shared<QTradingView::CandleStickSeries>(candles);
    pricePane->addSeries(candleSeries);

    // SMA
    QList<QTradingView::DataPoint> smaData;
    const int smaPeriod = 20;
    for (int i = 0; i < smaPeriod - 1; ++i) {
        smaData.append(QTradingView::DataPoint{candles[i].timeMsecs, std::nan("")});
    }
    for (int i = smaPeriod - 1; i < candles.size(); ++i) {
        double sum = 0;
        for (int j = 0; j < smaPeriod; ++j) sum += candles[i - j].close;
        double sma = sum / smaPeriod;
        smaData.append(QTradingView::DataPoint{candles[i].timeMsecs, sma});
    }
    auto smaSeries = std::make_shared<QTradingView::LineSeries>(smaData);
    smaSeries->setColor(QColor(255, 152, 0));
    pricePane->addSeries(smaSeries);

    // Pane 2: Volume
    auto volumePane = chart->addPane(0.8);
    volumePane->setScale(QTradingView::ScaleType::Linear);
    auto volumeSeries = std::make_shared<QTradingView::BarSeries>(volumeBars);
    volumePane->addSeries(volumeSeries);

    // Pane 3: MACD
    auto macdPane = chart->addPane(0.8);
    macdPane->setScale(QTradingView::ScaleType::Linear);
    auto macdLineSeries = std::make_shared<QTradingView::LineSeries>(macdLine);
    macdLineSeries->setColor(QColor(33, 150, 243));
    macdPane->addSeries(macdLineSeries);

    auto signalLineSeries = std::make_shared<QTradingView::LineSeries>(signalLine);
    signalLineSeries->setColor(QColor(255, 87, 34));
    macdPane->addSeries(signalLineSeries);

    auto histogramSeries = std::make_shared<QTradingView::BarSeries>(histogramBars);
    macdPane->addSeries(histogramSeries);

    chart->setTheme(QTradingView::ChartTheme::tradingViewDark());
    chart->fitToData();
    window.showMaximized();

    return app.exec();
}
