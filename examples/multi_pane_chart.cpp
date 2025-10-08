#include <QApplication>
#include <QMainWindow>
#include <QDateTime>
#include "QTradingView/QTradingView.h"
#include "QTradingView/Chart.h"
#include "QTradingView/data/CandleStickProvider.h"
#include "QTradingView/data/CandleStick.h"
#include "QTradingView/data/BarProvider.h"
#include "QTradingView/data/Bar.h"
#include "QTradingView/data/DataProvider.h"
#include "QTradingView/data/DataPoint.h"
#include "QTradingView/series/CandleStickSeries.h"
#include "QTradingView/series/BarSeries.h"
#include "QTradingView/series/LineSeries.h"
#include "QTradingView/scale/LinearScale.h"
#include <random>
#include <algorithm>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create main window
    QMainWindow window;
    window.setWindowTitle("QTradingView - Multi-Pane Complex Example");
    window.resize(1600, 900);

    // Create chart widget
    auto chartView = new QTradingView::QTradingView(&window);
    window.setCentralWidget(chartView);

    // Generate realistic market data
    QList<QTradingView::CandleStick> candles;
    QList<QTradingView::Bar> volumeBars;
    QList<QTradingView::DataPoint> macdLine;
    QList<QTradingView::DataPoint> signalLine;
    QList<QTradingView::Bar> histogramBars;

    QDateTime startTime = QDateTime::currentDateTime().addDays(-300);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> priceMove(0, 2.5);
    std::uniform_real_distribution<> wickRatio(0.2, 0.8);
    std::uniform_real_distribution<> volumeBase(1000000, 5000000);
    std::normal_distribution<> volumeNoise(0, 500000);

    double basePrice = 200.0;
    double lastClose = basePrice;

    // For MACD calculation (simplified)
    std::vector<double> prices;
    std::vector<QTradingView::TimePoint> timePoints;  // Store all timepoints
    const int ema12Period = 12;
    const int ema26Period = 26;
    const int signalPeriod = 9;

    for (int i = 0; i < 300; ++i) {
        QDateTime time = startTime.addSecs(i * 86400); // 1 day intervals

        // === Generate Candlestick Data ===
        double open = lastClose;
        double priceChange = priceMove(gen);

        // Add market trend periods
        if (i >= 50 && i < 120) {
            priceChange += 0.5; // Bull trend
        } else if (i >= 180 && i < 230) {
            priceChange -= 0.4; // Bear trend
        }

        double close = open + priceChange;

        // Ensure price stays reasonable
        if (close < 100.0) {
            close = 100.0 + std::abs(priceMove(gen));
        } else if (close > 400.0) {
            close = 400.0 - std::abs(priceMove(gen));
        }

        // Calculate high and low
        double rangeSize = std::abs(close - open) * 1.8 + std::abs(priceMove(gen));
        double high = std::max(open, close) + rangeSize * wickRatio(gen);
        double low = std::min(open, close) - rangeSize * (1.0 - wickRatio(gen));

        if (low < 80.0) low = 80.0;

        QTradingView::TimePoint timePoint(time, i);
        timePoints.push_back(timePoint);  // Store for later use
        candles.append(QTradingView::CandleStick(timePoint, open, high, low, close));

        prices.push_back(close);
        lastClose = close;

        // === Generate Volume Data ===
        // Volume typically higher on big price movements
        double priceChangePercent = std::abs(priceChange) / open;
        double volumeMultiplier = 1.0 + (priceChangePercent * 3.0);
        double volume = volumeBase(gen) * volumeMultiplier + volumeNoise(gen);

        if (volume < 0) volume = volumeBase(gen);

        volumeBars.append(QTradingView::Bar(timePoint, volume));
    }

    // === Generate MACD Indicator Data (Simplified) ===
    // Pre-fill with NaN values for indices before MACD can be calculated
    for (int i = 0; i < ema26Period; ++i) {
        macdLine.append(QTradingView::DataPoint(timePoints[i], std::nan("")));
    }

    for (int i = ema26Period; i < 300; ++i) {
        // Simplified EMA calculation
        double sum12 = 0, sum26 = 0;
        for (int j = 0; j < ema12Period; ++j) {
            sum12 += prices[i - j];
        }
        for (int j = 0; j < ema26Period; ++j) {
            sum26 += prices[i - j];
        }
        double ema12 = sum12 / ema12Period;
        double ema26 = sum26 / ema26Period;
        double macdValue = ema12 - ema26;

        macdLine.append(QTradingView::DataPoint(timePoints[i], macdValue));
    }

    // Calculate signal line (EMA of MACD)
    // Pre-fill with NaN values for indices before signal can be calculated
    int signalStartIndex = ema26Period + signalPeriod - 1;
    for (int i = 0; i < signalStartIndex; ++i) {
        signalLine.append(QTradingView::DataPoint(timePoints[i], std::nan("")));
        histogramBars.append(QTradingView::Bar(timePoints[i], 0.0));  // Zero histogram
    }

    for (int i = signalStartIndex; i < 300; ++i) {
        double signalSum = 0;
        for (int j = 0; j < signalPeriod; ++j) {
            signalSum += macdLine[i - j].value();
        }
        double signalValue = signalSum / signalPeriod;
        signalLine.append(QTradingView::DataPoint(timePoints[i], signalValue));

        // Histogram is MACD - Signal
        double histValue = macdLine[i].value() - signalValue;
        histogramBars.append(QTradingView::Bar(timePoints[i], histValue));
    }

    // === Setup Chart ===
    auto chart = chartView->chart();

    // Create data providers
    auto candleProvider = std::make_shared<QTradingView::CandleStickProvider>(candles);
    auto volumeProvider = std::make_shared<QTradingView::BarProvider>(volumeBars);
    auto macdProvider = std::make_shared<QTradingView::DataProvider>(macdLine);
    auto signalProvider = std::make_shared<QTradingView::DataProvider>(signalLine);
    auto histogramProvider = std::make_shared<QTradingView::BarProvider>(histogramBars);

    // Set main data provider (for time synchronization)
    chart->setDataProvider(candleProvider);

    // === Pane 1: Main Price Chart (60% height) ===
    auto pricePane = chart->addPane(2.0); // Larger ratio = more height
    auto priceScale = std::make_shared<QTradingView::LinearScale>();
    pricePane->setScale(priceScale);

    // Add candlestick series
    auto candleSeries = std::make_shared<QTradingView::CandleStickSeries>(candleProvider);
    pricePane->addSeries(candleSeries);

    // Add a moving average line on the price chart
    QList<QTradingView::DataPoint> smaData;
    const int smaPeriod = 20;

    // Pre-fill with NaN values for indices before SMA can be calculated
    for (int i = 0; i < smaPeriod - 1; ++i) {
        smaData.append(QTradingView::DataPoint(candles[i].time(), std::nan("")));
    }

    // Calculate SMA for remaining indices
    for (int i = smaPeriod - 1; i < candles.size(); ++i) {
        double sum = 0;
        for (int j = 0; j < smaPeriod; ++j) {
            sum += candles[i - j].close();
        }
        double sma = sum / smaPeriod;
        smaData.append(QTradingView::DataPoint(candles[i].time(), sma));
    }
    auto smaProvider = std::make_shared<QTradingView::DataProvider>(smaData);
    auto smaSeries = std::make_shared<QTradingView::LineSeries>(smaProvider);

    QTradingView::LineStyle smaStyle;
    smaStyle.color = QColor(255, 152, 0); // Orange
    smaStyle.width = 2.0;
    smaStyle.style = Qt::SolidLine;
    smaSeries->setStyle(smaStyle);
    pricePane->addSeries(smaSeries);

    // === Pane 2: Volume Chart (25% height) ===
    auto volumePane = chart->addPane(0.8);
    auto volumeScale = std::make_shared<QTradingView::LinearScale>();
    volumePane->setScale(volumeScale);

    auto volumeSeries = std::make_shared<QTradingView::BarSeries>(volumeProvider);
    volumePane->addSeries(volumeSeries);

    // === Pane 3: MACD Indicator (25% height) ===
    auto macdPane = chart->addPane(0.8);
    auto macdScale = std::make_shared<QTradingView::LinearScale>();
    macdPane->setScale(macdScale);

    // Add MACD line
    auto macdLineSeries = std::make_shared<QTradingView::LineSeries>(macdProvider);
    QTradingView::LineStyle macdStyle;
    macdStyle.color = QColor(33, 150, 243); // Blue
    macdStyle.width = 2.0;
    macdStyle.style = Qt::SolidLine;
    macdLineSeries->setStyle(macdStyle);
    macdPane->addSeries(macdLineSeries);

    // Add Signal line
    auto signalLineSeries = std::make_shared<QTradingView::LineSeries>(signalProvider);
    QTradingView::LineStyle signalStyle;
    signalStyle.color = QColor(255, 87, 34); // Red-Orange
    signalStyle.width = 2.0;
    signalStyle.style = Qt::SolidLine;
    signalLineSeries->setStyle(signalStyle);
    macdPane->addSeries(signalLineSeries);

    // Add Histogram
    auto histogramSeries = std::make_shared<QTradingView::BarSeries>(histogramProvider);
    macdPane->addSeries(histogramSeries);

    // Use the dark theme for a professional look
    chart->setTheme(QTradingView::ChartTheme::tradingViewDark());

    // Show window
    window.showMaximized();

    return app.exec();
}
