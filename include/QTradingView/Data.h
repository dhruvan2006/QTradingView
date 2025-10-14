#ifndef QTRADINGVIEW_DATA_H
#define QTRADINGVIEW_DATA_H

#include <QDateTime>

namespace QTradingView {

struct DataPoint {
    QDateTime time;
    double value;
};

struct CandleStick {
    QDateTime time;
    double open;
    double high;
    double low;
    double close;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_DATA_H