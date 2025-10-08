#include <QApplication>
#include <QMainWindow>
#include <QDateTime>
#include "QTradingView/QTradingView.h"
#include "QTradingView/Chart.h"
#include "QTradingView/data/CandleStickProvider.h"
#include "QTradingView/data/CandleStick.h"
#include "QTradingView/series/CandleStickSeries.h"
#include "QTradingView/scale/LinearScale.h"
#include <random>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create main window
    QMainWindow window;
    window.setWindowTitle("QTradingView - Candlestick Chart Example");
    window.resize(1400, 700);

    // Create chart widget
    auto chartView = new QTradingView::QTradingView(&window);
    window.setCentralWidget(chartView);

    // Generate realistic candlestick data
    QList<QTradingView::CandleStick> candles;
    QDateTime startTime = QDateTime::currentDateTime().addDays(-200);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> priceMove(0, 2.0);
    std::uniform_real_distribution<> wickRatio(0.3, 0.7);

    double basePrice = 150.0;
    double lastClose = basePrice;

    for (int i = 0; i < 200; ++i) {
        QDateTime time = startTime.addSecs(i * 86400); // 1 day intervals

        // Generate OHLC values
        double open = lastClose;
        double priceChange = priceMove(gen);
        double close = open + priceChange;

        // Ensure close doesn't go too low
        if (close < 50.0) {
            close = 50.0 + std::abs(priceMove(gen));
        }

        // Calculate high and low based on open/close
        double rangeSize = std::abs(close - open) * 1.5 + std::abs(priceMove(gen));
        double high = std::max(open, close) + rangeSize * wickRatio(gen);
        double low = std::min(open, close) - rangeSize * (1.0 - wickRatio(gen));

        // Ensure low doesn't go negative
        if (low < 40.0) low = 40.0;

        QTradingView::TimePoint timePoint(time, i);
        candles.append(QTradingView::CandleStick(timePoint, open, high, low, close));

        lastClose = close;
    }

    // Create candlestick data provider
    auto dataProvider = std::make_shared<QTradingView::CandleStickProvider>(candles);

    // Get chart and setup
    auto chart = chartView->chart();
    chart->setDataProvider(dataProvider);

    // Create main pane
    auto mainPane = chart->addPane(1.0);

    // Create and configure scale
    auto scale = std::make_shared<QTradingView::LinearScale>();
    mainPane->setScale(scale);

    // Create candlestick series
    auto candleSeries = std::make_shared<QTradingView::CandleStickSeries>(dataProvider);

    // Add series to pane
    mainPane->addSeries(candleSeries);

    // Use the dark theme for a professional look
    chart->setTheme(QTradingView::ChartTheme::tradingViewDark());

    // Show window
    window.show();

    return app.exec();
}
