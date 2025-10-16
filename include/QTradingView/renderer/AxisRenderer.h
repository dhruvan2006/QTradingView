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
#include <QDateTime>

#include "QTradingView/ViewPort.h"
#include "QTradingView/Pane.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

/**
 * @brief Represents a label for a specific time on the X-axis.
 *
 * Contains the timestamp, label string, and associated data index for rendering time labels on the chart axis.
 */
struct TimeLabel {
    /**
     * @brief Milliseconds since epoch for the label position.
     */
    qint64 msSecsSinceEpoch;
    /**
     * @brief The text to display for the label.
     */
    QString label;
    /**
     * @brief The index in the data series corresponding to this label.
     */
    int dataIndex;
};

/**
 * @brief Renders chart axes, ticks, and labels for QTradingView.
 *
 * AxisRenderer provides customization for axis appearance and handles drawing of axis lines, ticks, and time labels.
 */
class QTRADINGVIEW_EXPORT AxisRenderer
{
public:
    /**
     * @brief Constructs an AxisRenderer with default settings.
     */
    AxisRenderer();

    /**
     * @brief Sets the color of axis text labels.
     * @param color The text color.
     */
    void setTextColor(const QColor& color);

    /**
     * @brief Sets the color of axis ticks.
     * @param color The tick color.
     */
    void setTickColor(const QColor& color);

    /**
     * @brief Sets the color of the axis border.
     * @param color The border color.
     */
    void setBorderColor(const QColor& color);

    /**
     * @brief Sets the background color of the axis.
     * @param color The background color.
     */
    void setBackgroundColor(const QColor& color);

    /**
     * @brief Sets the width of the axis.
     * @param width The axis width in pixels.
     */
    void setAxisWidth(int width);

    /**
     * @brief Draws the X-axis.
     * @param painter The QPainter instance used for drawing.
     * @param axisRect The rectangle area for the axis.
     * @param viewport The current viewport settings.
     * @param series The data series being displayed.
     */
    void drawXAxis(QPainter* painter, const QRectF& axisRect,
                   const ViewPort& viewport, const Series* series);

    /**
     * @brief Draws the Y-axis.
     * @param painter The QPainter instance used for drawing.
     * @param leftAxisRect The rectangle area for the left Y-axis.
     * @param rightAxisRect The rectangle area for the right Y-axis.
     * @param pane The pane containing the series.
     */
    void drawYAxis(QPainter* painter, const QRectF& leftAxisRect,
                   const QRectF& rightAxisRect, const Pane* pane);

    /**
     * @brief Calculates the labels for the X-axis.
     * @param viewport The current viewport settings.
     * @param series The data series being displayed.
     * @return A vector of TimeLabel structures containing label information.
     */
    std::vector<TimeLabel> calculateXAxisLabels(const ViewPort& viewport, const Series* series) const;

private:
    QColor m_textColor;
    QColor m_tickColor;
    QColor m_borderColor;
    QColor m_backgroundColor;
    int m_axisWidth;

    QString formatYAxisLabel(double value, double minValue, double maxValue, int availableWidth, const QFont& font) const;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_AXISRENDERER_H
