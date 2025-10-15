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

#ifndef QTRADINGVIEW_ISERIES_H
#define QTRADINGVIEW_ISERIES_H

#include "QTradingView/qtradingview_global.h"

class QPainter;
class QPointF;
class QDateTime;

namespace QTradingView {

class IScale;
class ViewPort;

enum class SeriesType {
    Line,
    Bar,
    CandleStick
};

class QTRADINGVIEW_EXPORT Series
{
public:
    explicit Series(SeriesType type) : m_type(type) {}
    virtual ~Series() = default;

    SeriesType type() const { return m_type; }

    virtual qint64 timestampAt(int index) const = 0;
    virtual int dataCount() const = 0;

    virtual void render(QPainter* painter, const ViewPort& viewport, IScale* scale) = 0;
    virtual bool hitTest(const QPointF& point, int& outIndex) const = 0;
    virtual void calculateRange(int startIndex, int endIndex, double& outMin, double& outMax) const = 0;

protected:
    SeriesType m_type;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_ISERIES_H