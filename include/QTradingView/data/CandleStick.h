/*
 * Copyright (c) 2025 Dhruvan Gnanadhandayuthapani
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
