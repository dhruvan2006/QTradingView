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
#include "QTradingView/ViewPort.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

enum class ScaleType;

/**
 * @brief Represents a chart pane that contains one or more data series.
 *
 * Pane manages the layout, scaling, and rendering of its series. It supports automatic and manual Y-axis scaling, zooming, and custom height ratios for multi-pane charts.
 */
class QTRADINGVIEW_EXPORT Pane
{
public:
    /**
     * @brief Constructs an empty Pane.
     */
    Pane();

    /**
     * @brief Adds a data series to the pane.
     * @param series Shared pointer to the Series to add.
     */
    void addSeries(std::shared_ptr<Series> series);

    /**
     * @brief Removes a data series from the pane.
     * @param series Shared pointer to the Series to remove.
     */
    void removeSeries(std::shared_ptr<Series> series);

    /**
     * @brief Returns the list of data series in the pane.
     */
    const std::vector<std::shared_ptr<Series>>& series() const;

    /**
     * @brief Sets the Y-axis scale type for the pane.
     * @param type The scale type (e.g., linear, logarithmic).
     */
    void setScale(ScaleType type);

    /**
     * @brief Returns the current Y-axis scale object.
     */
    IScale* scale() const;

    /**
     * @brief Sets the height ratio of the pane relative to other panes.
     * @param ratio The height ratio (e.g., 1.0 for equal height).
     */
    void setHeightRatio(double ratio);

    /**
     * @brief Returns the height ratio of the pane.
     */
    double heightRatio() const;

    /**
     * @brief Sets the rectangle area for the pane in the chart.
     * @param rect The rectangle area.
     */
    void setRect(const QRectF &rect);

    /**
     * @brief Returns the rectangle area of the pane.
     */
    QRectF rect() const;

    /**
     * @brief Calculates the visible value range for the given data indices.
     * @param start Start index of the data range.
     * @param end End index of the data range.
     */
    void calculateRange(int start, int end);

    /**
     * @brief Returns the minimum value in the current range.
     */
    double minValue() const;

    /**
     * @brief Returns the maximum value in the current range.
     */
    double maxValue() const;

    /**
     * @brief Sets a manual value range for the Y-axis.
     * @param minValue Minimum value.
     * @param maxValue Maximum value.
     */
    void setManualRange(double minValue, double maxValue);

    /**
     * @brief Zooms the Y-axis by a factor, anchored at a specific value.
     * @param zoomFactor The zoom factor (>1 to zoom in, <1 to zoom out).
     * @param anchorValue The value to anchor the zoom operation.
     */
    void zoomYAxis(double zoomFactor, double anchorValue);

    /**
     * @brief Resets the Y-axis to automatic range calculation.
     */
    void resetAutoRange();

    /**
     * @brief Returns true if the Y-axis is in auto-range mode.
     */
    bool isAutoRange() const;

    /**
     * @brief Renders the pane and its series using the given painter and viewport.
     * @param painter QPainter object for drawing.
     * @param viewport The current viewport.
     */
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
