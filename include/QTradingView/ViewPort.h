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

constexpr int MAX_VISIBLE_BARS = 1500;

class QTRADINGVIEW_EXPORT ViewPort
{
public:
    ViewPort();
    ViewPort(int startIndex, int endIndex, const QRectF &pixelRect, double barWidth = 8.0);

    void setVisibleRange(int start, int end);
    int startIndex() const;
    int endIndex() const;
    int visibleCount() const;

    void setPixelRect(const QRectF &rect);
    QRectF pixelRect() const;

    double indexToPixel(int index) const;
    int pixelToIndex(double pixel) const;

    void setBarWidth(double width);
    double barWidth() const;

private:
    int m_startIndex;
    int m_endIndex;
    QRectF m_pixelRect;
    double m_barWidth;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_VIEWPORT_H
