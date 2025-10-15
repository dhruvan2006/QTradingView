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

#ifndef QTRADINGVIEW_CANDLESTICKSERIES_H
#define QTRADINGVIEW_CANDLESTICKSERIES_H

#include <QColor>
#include "QTradingView/Data.h"
#include "QTradingView/series/Series.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT CandleStickSeries : public Series
{
public:
    explicit CandleStickSeries(const QList<CandleStick>& data);
    ~CandleStickSeries() override;

    // Data
    void setData(const QList<CandleStick>& data = {});
    const QList<CandleStick>& data() const;
    qint64 timestampAt(int index) const override;
    int dataCount() const override;

    // Style
    void setBullColor(const QColor& color);
    void setBearColor(const QColor& color);
    void setBorderColor(const QColor& color);
    void setBorderWidth(double width);
    void setBodyWithRatio(double ratio);
    void setMaxBodyWidth(double maxWidth);
    void setAntialiasing(bool enabled);

    // Rendering
    void render(QPainter* painter, const ViewPort& viewport, IScale* scale) override;
    bool hitTest(const QPointF& point, int& outIndex) const override;
    void calculateRange(int startIndex, int endIndex, double& outMin, double& outMax) const override;

private:
    QList<CandleStick> m_data;

    QColor m_bullColor;
    QColor m_bearColor;
    QColor m_borderColor;
    double m_borderWidth;
    double m_bodyWidthRatio;
    double m_maxBodyWidthPx;
    bool m_antialiasing;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_CANDLESTICKSERIES_H
