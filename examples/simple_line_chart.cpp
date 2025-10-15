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
#include "QTradingView/Data.h"
#include "QTradingView/series/LineSeries.h"
#include "QTradingView/scale/LinearScale.h"
#include <cmath>
#include <random>
#include <QFile>
#include <QTextStream>
#include "QTradingView/scale/ScaleType.h"

QList<QTradingView::DataPoint> loadClosePrices(const QString& csvPath) {
    QList<QTradingView::DataPoint> data;
    QFile file(csvPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return data;
    QTextStream in(&file);
    in.readLine(); // Skip header
    while (!in.atEnd()) {
        QString line = in.readLine();
        auto fields = line.split(',');
        if (fields.size() < 5) continue;
        QDateTime time = QDateTime::fromString(fields[0], "yyyy-MM-dd HH:mm:ss+00:00");
        double close = fields[4].toDouble();
        data.push_back({time.toMSecsSinceEpoch(), close});
    }
    return data;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create main window
    QMainWindow window;
    window.setWindowTitle("QTradingView Line Chart Example");
    window.resize(800, 499);

    // Chart widget
    auto chart = new QTradingView::Chart(&window);
    window.setCentralWidget(chart);

    // Data
    QString csvPath = QCoreApplication::applicationDirPath() + "/btc-usd.csv";
    QList<QTradingView::DataPoint> data = loadClosePrices(csvPath);

    // Create main pane
    auto mainPane = chart->addPane(1.0);
    mainPane->setScale(QTradingView::ScaleType::Logarithmic);

    // Create line series and add to pane
    auto lineSeries = std::make_shared<QTradingView::LineSeries>(data);
    lineSeries->setColor(0xff6900);
    mainPane->addSeries(lineSeries);

    // Optional: Set theme
    chart->setTheme(QTradingView::ChartTheme::tradingViewDark());
    chart->fitToData();

    window.show();
    return app.exec();
}
