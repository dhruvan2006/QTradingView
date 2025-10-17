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

/**
 * @class CrosshairRenderer
 * @brief Renders crosshair lines and labels on the chart.
 *
 * @details CrosshairRenderer draws vertical and horizontal crosshair lines at the mouse position,
 * along with price and time labels. It supports customization of colors and fonts for the crosshair
 * and labels.
 */
class QTRADINGVIEW_EXPORT CrosshairRenderer
{
public:
    /**
     * @brief Constructs a CrosshairRenderer with default settings.
     */
    CrosshairRenderer();

    /**
     * @brief Sets the color of the crosshair lines.
     * @param color The crosshair color.
     */
    void setCrosshairColor(const QColor& color);
    /**
     * @brief Sets the background color of the crosshair label.
     * @param color The label background color.
     */
    void setLabelBackgroundColor(const QColor& color);
    /**
     * @brief Sets the text color of the crosshair label.
     * @param color The label text color.
     */
    void setLabelTextColor(const QColor& color);
    /**
     * @brief Sets the font used for crosshair labels.
     * @param font The label font.
     */
    void setFont(const QFont& font);

    /**
     * @brief Renders the crosshair and its labels on the chart.
     * @param painter The QPainter instance used for drawing.
     * @param position The position of the crosshair.
     * @param viewport The current viewport settings.
     * @param pane The pane being rendered.
     * @param series The data series being displayed.
     * @param xAxisY Optional Y position for the X-axis (default -1.0).
     */
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
