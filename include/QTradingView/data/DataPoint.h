#ifndef QTRADINGVIEW_DATAPOINT_H
#define QTRADINGVIEW_DATAPOINT_H

#include "TimePoint.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT DataPoint
{
public:
    DataPoint();
    DataPoint(const TimePoint &time, double value);

    TimePoint time() const;
    void setTime(const TimePoint &time);

    double value() const;
    void setValue(double value);

private:
    TimePoint m_time;
    double m_value;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_DATAPOINT_H