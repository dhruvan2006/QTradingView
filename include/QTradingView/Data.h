#ifndef QTRADINGVIEW_DATA_H
#define QTRADINGVIEW_DATA_H

#include <QtGlobal>

namespace QTradingView {

/**
 * @struct DataPoint
 * @brief Represents a single time-value data point for line/bar series.
 */
struct DataPoint {
    /**
     * @brief Timestamp in milliseconds since epoch.
     */
    qint64 timeMsecs;

    /**
     * @brief Value at the given timestamp.
     */
    double value;
};

/**
 * @struct CandleStick
 * @brief Represents a single candlestick data point for candlestick series.
 */
struct CandleStick {
    /**
     * @brief Timestamp in milliseconds since epoch.
     */
    qint64 timeMsecs;

    /**
     * @brief Open
     */
    double open;

    /**
     * @brief High
     */
    double high;

    /**
     * @brief Low
     */
    double low;

    /**
     * @brief Close
     */
    double close;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_DATA_H