#ifndef QTRADINGVIEW_BAR_H
#define QTRADINGVIEW_BAR_H

#include "TimePoint.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT Bar
{
public:
    Bar();
    Bar(const TimePoint &time, double value);

    TimePoint time() const;
    void setTime(const TimePoint &time);

    double value() const;
    void setValue(double value);

private:
    TimePoint m_time;
    double m_value;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_BAR_H

