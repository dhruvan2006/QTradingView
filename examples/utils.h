#ifndef UTILS_H
#define UTILS_H

#include <QList>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "QTradingView/Data.h"

namespace Utils {

    // Load closing prices for line series
    inline QList<QTradingView::DataPoint> loadClosePrices(const QString &csvPath) {
        QList<QTradingView::DataPoint> data;
        QFile file(csvPath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return data;
        QTextStream in(&file);
        in.readLine(); // skip header
        while (!in.atEnd()) {
            QString line = in.readLine();
            auto fields = line.split(',');
            if (fields.size() < 5) continue;
            QDateTime time = QDateTime::fromString(fields[0], "yyyy-MM-dd HH:mm:ss+00:00");
            data.push_back({time.toMSecsSinceEpoch(), fields[4].toDouble()});
        }
        return data;
    }

    // Load candlestick data
    inline QList<QTradingView::CandleStick> loadCandles(const QString &csvPath) {
        QList<QTradingView::CandleStick> candles;
        QFile file(csvPath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return candles;
        QTextStream in(&file);
        in.readLine(); // skip header
        while (!in.atEnd()) {
            QString line = in.readLine();
            auto f = line.split(',');
            if (f.size() < 5) continue;
            QDateTime t = QDateTime::fromString(f[0], "yyyy-MM-dd HH:mm:ss+00:00");
            candles.append({t.toMSecsSinceEpoch(),
                            f[1].toDouble(), f[2].toDouble(),
                            f[3].toDouble(), f[4].toDouble()});
        }
        return candles;
    }

    inline QList<QTradingView::DataPoint> computeSMA(const QList<QTradingView::CandleStick>& candles, int period) {
        QList<QTradingView::DataPoint> smaData;
        for (int i = 0; i < period - 1; ++i) {
            smaData.append({candles[i].timeMsecs, std::nan("")});
        }
        for (int i = period - 1; i < candles.size(); ++i) {
            double sum = 0;
            for (int j = 0; j < period; ++j) sum += candles[i - j].close;
            smaData.append({candles[i].timeMsecs, sum / period});
        }
        return smaData;
    }

    struct MACDResult {
        QList<QTradingView::DataPoint> macdLine;
        QList<QTradingView::DataPoint> signalLine;
        QList<QTradingView::DataPoint> histogram;
    };

    inline MACDResult computeMACD(const QList<QTradingView::CandleStick>& candles, int ema12=12, int ema26=26, int signalPeriod=9) {
        QList<QTradingView::DataPoint> macdLine, signalLine, histogram;
        std::vector<double> prices;
        std::vector<qint64> times;

        for (auto &c : candles) {
            prices.push_back(c.close);
            times.push_back(c.timeMsecs);
        }

        // Compute MACD line (simple EMA approximation)
        for (int i = 0; i < ema26; ++i) macdLine.append({times[i], std::nan("")});
        for (int i = ema26; i < prices.size(); ++i) {
            double ema12Val = 0, ema26Val = 0;
            for (int j = 0; j < ema12; ++j) ema12Val += prices[i - j];
            for (int j = 0; j < ema26; ++j) ema26Val += prices[i - j];
            ema12Val /= ema12;
            ema26Val /= ema26;
            macdLine.append({times[i], ema12Val - ema26Val});
        }

        int signalStart = ema26 + signalPeriod - 1;
        for (int i = 0; i < signalStart; ++i) {
            signalLine.append({times[i], std::nan("")});
            histogram.append({times[i], 0.0});
        }

        for (int i = signalStart; i < prices.size(); ++i) {
            double sum = 0;
            for (int j = 0; j < signalPeriod; ++j) sum += macdLine[i - j].value;
            double sig = sum / signalPeriod;
            signalLine.append({times[i], sig});
            histogram.append({times[i], macdLine[i].value - sig});
        }

        return {macdLine, signalLine, histogram};
    }

} // namespace CsvLoader

#endif // UTILS_H