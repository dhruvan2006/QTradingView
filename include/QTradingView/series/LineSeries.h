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

#ifndef QTRADINGVIEW_LINESERIES_H
#define QTRADINGVIEW_LINESERIES_H

#include <QColor>
#include <memory>
#include "Series.h"
#include "QTradingView/qtradingview_global.h"

class QPainter;

namespace QTradingView {

struct DataPoint;
class Viewport;
class IScale;

/**
 * @brief Represents a line chart series in QTradingView.
 *
 * LineSeries manages a collection of data points and renders them as a line chart. Supports data updates and style customization.
 */
class QTRADINGVIEW_EXPORT LineSeries : public Series
{
public:
    /**
     * @brief Constructs a LineSeries with optional initial data.
     * @param data List of DataPoint objects.
     */
    explicit LineSeries(const QList<DataPoint>& data = {});
    /**
     * @brief Destroys the LineSeries object.
     */
    ~LineSeries() override;

    /**
     * @brief Sets the data points for the series.
     * @param data List of DataPoint objects.
     */
    void setData(const QList<DataPoint>& data);
    /**
     * @brief Returns the current data points.
     * @return List of DataPoint objects.
     */
    const QList<DataPoint>& data() const;
    /**
     * @brief Returns the timestamp for the data point at the given index.
     * @param index Data point index.
     * @return Timestamp in milliseconds.
     */
    qint64 timestampAt(int index) const override;
    /**
     * @brief Returns the number of data points in the series.
     * @return Data point count.
     */
    int dataCount() const override;

    // Style
    void setColor(const QColor& color);
    void setLineWidth(double width);
    void setLineStyle(Qt::PenStyle style);
    void setAntialiasing(bool enabled);

    // Rendering
    void render(QPainter* painter, const ViewPort& viewport, IScale* scale) override;
    bool hitTest(const QPointF& point, int& outIndex) const override;
    void calculateRange(int startIndex, int endIndex, double& outMin, double& outMax) const override;

private:
    QList<DataPoint> m_data;

    QColor m_color;
    double m_width;
    Qt::PenStyle m_lineStyle;
    bool m_antialiasing;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_LINESERIES_H
