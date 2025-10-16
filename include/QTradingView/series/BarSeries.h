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

#ifndef QTRADINGVIEW_BARSERIES_H
#define QTRADINGVIEW_BARSERIES_H

#include <QColor>
#include <QList>

#include "Series.h"
#include "QTradingView/Data.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

/**
 * @brief Represents a bar chart series in QTradingView.
 *
 * BarSeries manages a collection of bar data and renders them as a bar chart. Supports data updates and style customization.
 */
class QTRADINGVIEW_EXPORT BarSeries : public Series
{
public:
    /**
     * @brief Constructs a BarSeries with optional initial data.
     * @param data List of DataPoint objects.
     */
    explicit BarSeries(const QList<DataPoint>& data = {});
    /**
     * @brief Destroys the BarSeries object.
     */
    ~BarSeries() override;

    /**
     * @brief Sets the bar data for the series.
     * @param data List of DataPoint objects.
     */
    void setData(const QList<DataPoint>& data);
    /**
     * @brief Returns the current bar data.
     * @return List of DataPoint objects.
     */
    const QList<DataPoint>& data() const;
    /**
     * @brief Returns the timestamp for the bar at the given index.
     * @param index Bar index.
     * @return Timestamp in milliseconds.
     */
    qint64 timestampAt(int index) const override;
    /**
     * @brief Returns the number of bars in the series.
     * @return Bar count.
     */
    int dataCount() const override;

    /**
     * @brief Sets the color for upward bars.
     * @param color Upward bar color.
     */
    void setUpColor(const QColor& color);
    /**
     * @brief Sets the color for downward bars.
     * @param color Downward bar color.
     */
    void setDownColor(const QColor& color);
    /**
     * @brief Sets the width ratio for bars.
     * @param ratio Ratio of bar width to total width.
     */
    void setBarWidthRatio(double ratio);
    /**
     * @brief Sets the line width for the bars.
     * @param width Line width in pixels.
     */
    void setLineWidth(double width);
    /**
     * @brief Enables or disables antialiasing for bar rendering.
     * @param enabled Antialiasing flag.
     */
    void setAntialiasing(bool enabled);

    // Rendering
    void render(QPainter* painter, const ViewPort& viewport, IScale* scale) override;
    bool hitTest(const QPointF& point, int& outIndex) const override;
    void calculateRange(int startIndex, int endIndex, double& outMin, double& outMax) const override;

private:
    QList<DataPoint> m_data;

    QColor m_upColor;
    QColor m_downColor;
    double m_barWidthRatio;
    double m_lineWidth;
    bool m_antialiasing;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_BARSERIES_H

