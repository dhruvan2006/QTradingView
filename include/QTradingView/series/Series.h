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

/**
 * @brief Abstract base class for data series in QTradingView charts.
 *
 * Series provides the interface for chart series (line, bar, candlestick), including type, rendering, and data access.
 */
class QTRADINGVIEW_EXPORT Series
{
public:
    /**
     * @brief Constructs a Series of the given type.
     * @param type Series type (Line, Bar, CandleStick).
     */
    explicit Series(SeriesType type);

    /**
     * @brief Virtual destructor for Series.
     */
    virtual ~Series() = default;

    /**
     * @brief Returns the type of the series.
     * @return SeriesType enum value.
     */
    SeriesType type() const;

    /**
     * @brief Returns the timestamp for the data point at the given index.
     * @param index Data point index.
     * @return Timestamp in milliseconds.
     */
    virtual qint64 timestampAt(int index) const = 0;

    /**
     * @brief Returns the number of data points in the series.
     * @return Data point count.
     */
    virtual int dataCount() const = 0;

    /**
     * @brief Renders the series within the given viewport and scale.
     * @param painter QPainter instance for drawing.
     * @param viewport Viewport defining the area to render.
     * @param scale Scale used for rendering.
     */
    virtual void render(QPainter* painter, const ViewPort& viewport, IScale* scale) = 0;

    /**
     * @brief Performs hit testing to find the nearest data point to the given position.
     * @param point Position to test.
     * @param outIndex Output parameter for the nearest data point index.
     * @return True if a data point was hit, false otherwise.
     */
    virtual bool hitTest(const QPointF& point, int& outIndex) const = 0;

    /**
     * @brief Calculates the data range (min/max values) within the given index range.
     * @param startIndex Start index of the range.
     * @param endIndex End index of the range.
     * @param outMin Output parameter for the minimum value.
     * @param outMax Output parameter for the maximum value.
     */
    virtual void calculateRange(int startIndex, int endIndex, double& outMin, double& outMax) const = 0;

protected:
    SeriesType m_type;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_ISERIES_H