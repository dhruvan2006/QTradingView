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

#ifndef QTRADINGVIEW_AXISRENDERER_H
#define QTRADINGVIEW_AXISRENDERER_H

#include <QPainter>
#include <QRectF>
#include <QColor>
#include <memory>
#include "QTradingView/ViewPort.h"
#include "QTradingView/Pane.h"
#include "QTradingView/data/IDataProvider.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

struct TimeLabel {
    QDateTime dateTime;
    QString label;
    int dataIndex;
};

class QTRADINGVIEW_EXPORT AxisRenderer
{
public:
    AxisRenderer();

    void setTextColor(const QColor& color);
    void setGridColor(const QColor& color);
    void setBorderColor(const QColor& color);
    void setBackgroundColor(const QColor& color);
    void setAxisWidth(int width);

    void drawXAxis(QPainter* painter, const QRectF& axisRect,
                   const ViewPort& viewport, IDataProvider* dataProvider);

    void drawYAxis(QPainter* painter, const QRectF& leftAxisRect,
                   const QRectF& rightAxisRect, const Pane* pane);

    std::vector<double> calculateYAxisTicks(double minValue, double maxValue, int approxTickCount) const;
    std::vector<TimeLabel> calculateXAxisLabels(const ViewPort& viewport, IDataProvider* dataProvider) const;

private:
    QColor m_textColor;
    QColor m_gridColor;
    QColor m_borderColor;
    QColor m_backgroundColor;
    int m_axisWidth;

    QString formatYAxisLabel(double value, double minValue, double maxValue, int availableWidth, const QFont& font) const;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_AXISRENDERER_H
