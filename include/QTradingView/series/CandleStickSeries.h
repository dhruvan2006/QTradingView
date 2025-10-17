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

/**
 * @brief Represents a candlestick chart series in QTradingView.
 *
 * CandleStickSeries manages a collection of candlestick data and renders them as a candlestick chart. Supports data updates and style customization.
 */
class QTRADINGVIEW_EXPORT CandleStickSeries : public Series
{
public:
    /**
     * @brief Constructs a CandleStickSeries with initial data.
     * @param data List of CandleStick objects.
     */
    explicit CandleStickSeries(const QList<CandleStick>& data);
    /**
     * @brief Destroys the CandleStickSeries object.
     */
    ~CandleStickSeries() override;

    /**
     * @brief Sets the candlestick data for the series.
     * @param data List of CandleStick objects.
     */
    void setData(const QList<CandleStick>& data = {});
    /**
     * @brief Returns the current candlestick data.
     * @return List of CandleStick objects.
     */
    const QList<CandleStick>& data() const;
    /**
     * @brief Returns the timestamp for the candlestick at the given index.
     * @param index Candlestick index.
     * @return Timestamp in milliseconds.
     */
    qint64 timestampAt(int index) const override;
    /**
     * @brief Returns the number of candlesticks in the series.
     * @return Candlestick count.
     */
    int dataCount() const override;

    /**
     * @brief Sets the color for bullish candles.
     * @param color Bullish candle color.
     */
    void setBullColor(const QColor& color);
    /**
     * @brief Sets the color for bearish candles.
     * @param color Bearish candle color.
     */
    void setBearColor(const QColor& color);
    /**
     * @brief Sets the border color for candles.
     * @param color Border color.
     */
    void setBorderColor(const QColor& color);
    /**
     * @brief Sets the border width for candles.
     * @param width Border width in pixels.
     */
    void setBorderWidth(double width);
    /**
     * @brief Sets the body width ratio for candles.
     * @param ratio Ratio of body width to total width.
     */
    void setBodyWithRatio(double ratio);
    /**
     * @brief Sets the maximum body width for candles.
     * @param maxWidth Maximum body width in pixels.
     */
    void setMaxBodyWidth(double maxWidth);
    /**
     * @brief Enables or disables antialiasing for the series.
     * @param enabled Antialiasing flag.
     */
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
