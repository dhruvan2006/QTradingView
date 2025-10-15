#ifndef QTRADINGVIEW_DATA_H
#define QTRADINGVIEW_DATA_H

#include <QtGlobal>

namespace QTradingView {

struct DataPoint {
    qint64 timeMsecs;
    double value;
};

struct CandleStick {
    qint64 timeMsecs;
    double open;
    double high;
    double low;
    double close;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_DATA_H