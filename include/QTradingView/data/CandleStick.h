#ifndef QTRADINGVIEW_CANDLESTICK_H
#define QTRADINGVIEW_CANDLESTICK_H

#include "TimePoint.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT CandleStick
{
public:
    CandleStick();
    CandleStick(const TimePoint &time, double open, double high, double low, double close);

    TimePoint time() const;
    void setTime(const TimePoint &time);

    double open() const;
    void setOpen(double open);

    double high() const;
    void setHigh(double high);

    double low() const;
    void setLow(double low);

    double close() const;
    void setClose(double close);

    bool isBullish();
    bool isBearish();
    double bodyTop();
    double bodyBottom();
    double range();

private:
    TimePoint m_time;
    double m_open;
    double m_high;
    double m_low;
    double m_close;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_CANDLESTICK_H
