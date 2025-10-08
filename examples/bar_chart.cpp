#include <QApplication>
#include <QMainWindow>
#include <QDateTime>
#include "QTradingView/QTradingView.h"
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
    auto chartView = new QTradingView::QTradingView(&window);
    window.setCentralWidget(chartView);

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
    auto chart = chartView->chart();
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
