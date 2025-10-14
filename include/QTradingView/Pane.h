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

#ifndef QTRADINGVIEW_PANE_H
#define QTRADINGVIEW_PANE_H

#include <QRectF>
#include <vector>
#include <memory>
#include "series/Series.h"
#include "scale/IScale.h"
#include "Viewport.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

enum class ScaleType;

class QTRADINGVIEW_EXPORT Pane
{
public:
    Pane();

    void addSeries(std::shared_ptr<Series> series);
    void removeSeries(std::shared_ptr<Series> series);
    const std::vector<std::shared_ptr<Series>>& series() const;

    void setScale(ScaleType type);
    IScale* scale() const;

    void setHeightRatio(double ratio);
    double heightRatio() const;

    void setRect(const QRectF &rect);
    QRectF rect() const;

    void calculateRange(int start, int end);
    double minValue() const;
    double maxValue() const;

    void setManualRange(double minValue, double maxValue);
    void zoomYAxis(double zoomFactor, double anchorValue);
    void resetAutoRange();
    bool isAutoRange() const;

    void render(QPainter *painter, const ViewPort &viewport);

private:
    std::vector<std::shared_ptr<Series>> m_series;
    std::shared_ptr<IScale> m_scale;
    double m_heightRatio;
    QRectF m_rect;
    double m_minValue;
    double m_maxValue;
    bool m_autoRange;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_PANE_H
