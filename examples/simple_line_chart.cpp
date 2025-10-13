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
#include <QVBoxLayout>
#include <QDateTime>
#include "QTradingView/Chart.h"
#include "QTradingView/data/DataProvider.h"
#include "QTradingView/data/DataPoint.h"
#include "QTradingView/series/LineSeries.h"
#include "QTradingView/scale/LinearScale.h"
#include <cmath>
#include <random>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create main window
    QMainWindow window;
    window.setWindowTitle("QTradingView - Enhanced Chart Example");
    window.resize(1400, 700);

    // Create chart widget
    auto chart = new QTradingView::Chart(&window);
    window.setCentralWidget(chart);

    // Generate more realistic sample data (price-like movement with trend and noise)
    QList<QTradingView::DataPoint> data;
    QDateTime startTime = QDateTime::currentDateTime().addDays(-500);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> noise(0, 1.5);
    std::normal_distribution<> trendNoise(0, 0.3);

    double basePrice = 100.0;
    double price = basePrice;
    double momentum = 0.0;

    for (int i = 0; i < 500; ++i) {
        QDateTime time = startTime.addSecs(i * 86400); // 1 day intervals

        // Add momentum-based trend (creates realistic trending behavior)
        momentum += trendNoise(gen);
        momentum *= 0.95; // Decay momentum slightly

        // Apply momentum and random walk to price
        price += momentum + noise(gen);

        // Ensure price doesn't go negative
        if (price < 10.0) {
            price = 10.0 + std::abs(noise(gen));
            momentum = std::abs(momentum); // Reverse momentum
        }

        QTradingView::TimePoint timePoint(time, i);
        data.append(QTradingView::DataPoint(timePoint, price));
    }

    // Create data provider
    auto dataProvider = std::make_shared<QTradingView::DataProvider>(data);

    // Get chart and setup
    chart->setDataProvider(dataProvider);

    // Create main pane
    auto mainPane = chart->addPane(1.0);

    // Create and configure scale
    auto scale = std::make_shared<QTradingView::LinearScale>();
    mainPane->setScale(scale);

    // Create line series with enhanced styling
    auto lineSeries = std::make_shared<QTradingView::LineSeries>(dataProvider);

    // Customize line style for a professional TradingView appearance
    QTradingView::LineStyle lineStyle;
    lineStyle.color = QColor(42, 162, 255);  // Bright TradingView blue
    lineStyle.width = 2.5;
    lineStyle.style = Qt::SolidLine;
    lineSeries->setStyle(lineStyle);

    // Add series to pane
    mainPane->addSeries(lineSeries);

    // Use the dark theme for a professional look
    chart->setTheme(QTradingView::ChartTheme::tradingViewDark());

    // Show window
    window.show();

    return app.exec();
}
