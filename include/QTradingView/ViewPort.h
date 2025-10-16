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

#ifndef QTRADINGVIEW_VIEWPORT_H
#define QTRADINGVIEW_VIEWPORT_H

#include <QRectF>
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

constexpr int MAX_VISIBLE_BARS = 1500; ///< Maximum number of bars visible in the viewport

/**
 * @brief Manages the visible range and pixel mapping for chart bars.
 *
 * ViewPort controls which data indices are visible, their pixel positions, and the bar width for rendering. It provides conversion between data indices and pixel coordinates, and supports dynamic resizing and zooming.
 */
class QTRADINGVIEW_EXPORT ViewPort
{
public:
    /**
     * @brief Constructs an empty ViewPort.
     */
    ViewPort();

    /**
     * @brief Constructs a ViewPort with specified range, pixel rectangle, and bar width.
     * @param startIndex Index of the first visible bar.
     * @param endIndex Index of the last visible bar.
     * @param pixelRect Rectangle area in pixels for the viewport.
     * @param barWidth Width of each bar in pixels (default 8.0).
     */
    ViewPort(int startIndex, int endIndex, const QRectF &pixelRect, double barWidth = 8.0);

    /**
     * @brief Sets the visible range of bar indices.
     * @param start Index of the first visible bar.
     * @param end Index of the last visible bar.
     */
    void setVisibleRange(int start, int end);

    /**
     * @brief Returns the index of the first visible bar.
     */
    int startIndex() const;

    /**
     * @brief Returns the index of the last visible bar.
     */
    int endIndex() const;

    /**
     * @brief Returns the number of visible bars in the viewport.
     */
    int visibleCount() const;

    /**
     * @brief Sets the pixel rectangle for the viewport area.
     * @param rect Rectangle area in pixels.
     */
    void setPixelRect(const QRectF &rect);

    /**
     * @brief Returns the pixel rectangle of the viewport.
     */
    QRectF pixelRect() const;

    /**
     * @brief Converts a bar index to its pixel position within the viewport.
     * @param index Bar index to convert.
     * @return Pixel position corresponding to the index.
     */
    double indexToPixel(int index) const;

    /**
     * @brief Converts a pixel position to the corresponding bar index.
     * @param pixel Pixel position to convert.
     * @return Bar index corresponding to the pixel position.
     */
    int pixelToIndex(double pixel) const;

    /**
     * @brief Sets the width of each bar in pixels.
     * @param width Bar width in pixels.
     */
    void setBarWidth(double width);

    /**
     * @brief Returns the width of each bar in pixels.
     */
    double barWidth() const;

private:
    int m_startIndex;
    int m_endIndex;
    QRectF m_pixelRect;
    double m_barWidth;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_VIEWPORT_H
