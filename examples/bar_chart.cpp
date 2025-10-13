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
#include "QTradingView/data/BarProvider.h"
#include "QTradingView/data/Bar.h"
#include "QTradingView/series/BarSeries.h"
#include "QTradingView/scale/LinearScale.h"
#include "QTradingView/style/BarStyle.h"
#include <random>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create main window
    QMainWindow window;
    window.setWindowTitle("QTradingView - Bar Chart (Positive/Negative Values) Example");
    window.resize(1400, 700);

    // Create chart widget
    auto chart = new QTradingView::Chart(&window);
    window.setCentralWidget(chart);

    // Generate data with both positive and negative values (e.g., profit/loss, price changes)
    QList<QTradingView::Bar> bars;
    QDateTime startTime = QDateTime::currentDateTime().addDays(-200);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> valueDist(0.0, 5.0); // Centered at 0, ranges positive and negative

    for (int i = 0; i < 200; ++i) {
        QDateTime time = startTime.addSecs(i * 86400); // 1 day intervals

        // Generate values that can be positive or negative
        double value = valueDist(gen);

        // Add some trends and spikes
        if (i >= 50 && i < 100) {
            value += 3.0; // Positive trend period
        } else if (i >= 120 && i < 150) {
            value -= 4.0; // Negative trend period
        }

        // Add occasional large spikes
        if (i % 25 == 0) {
            value *= 2.5;
        }

        QTradingView::TimePoint timePoint(time, i);
        bars.append(QTradingView::Bar(timePoint, value));
    }

    // Create bar data provider
    auto dataProvider = std::make_shared<QTradingView::BarProvider>(bars);

    // Get chart and setup
    chart->setDataProvider(dataProvider);

    // Create main pane
    auto mainPane = chart->addPane(1.0);

    // Create and configure scale
    auto scale = std::make_shared<QTradingView::LinearScale>();
    mainPane->setScale(scale);

    // Create bar series
    auto barSeries = std::make_shared<QTradingView::BarSeries>(dataProvider);

    // Add series to pane
    mainPane->addSeries(barSeries);

    // Use the dark theme for a professional look
    chart->setTheme(QTradingView::ChartTheme::tradingViewDark());

    // Show window
    window.show();

    return app.exec();
}
