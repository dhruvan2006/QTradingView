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

#ifndef QTRADINGVIEW_CROSSHAIRRENDERER_H
#define QTRADINGVIEW_CROSSHAIRRENDERER_H

#include <QColor>
#include <QPointF>
#include <QFont>
#include "QTradingView/qtradingview_global.h"

class QPainter;
class QRectF;

namespace QTradingView {

class ViewPort;
class Pane;
class Series;

class QTRADINGVIEW_EXPORT CrosshairRenderer
{
public:
    CrosshairRenderer();

    void setCrosshairColor(const QColor& color);
    void setLabelBackgroundColor(const QColor& color);
    void setLabelTextColor(const QColor& color);
    void setFont(const QFont& font);

    void render(QPainter* painter, const QPointF& position, const ViewPort& viewport,
                const Pane* pane, const Series* series, double xAxisY = -1.0);

private:
    QColor m_crosshairColor;
    QColor m_labelBackgroundColor;
    QColor m_labelTextColor;
    QFont m_font;

    void drawPriceLabel(QPainter* painter, double y, double price, const QRectF& chartArea, bool onRight);
    void drawTimeLabel(QPainter* painter, double x, const QString& timeStr, const QRectF& chartArea, double bottomY);
    void drawCrosshairMarker(QPainter* painter, const QPointF& position);
};

} // namespace QTradingView

#endif // QTRADINGVIEW_CROSSHAIRRENDERER_H
