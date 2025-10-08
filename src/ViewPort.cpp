// Copyright (c) 2025 Dhruvan Gnanadhandayuthapani
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "QTradingView/ViewPort.h"
#include <cmath>

namespace QTradingView {

ViewPort::ViewPort()
    : m_startIndex(0), m_endIndex(0), m_barWidth(8.0) {
}

ViewPort::ViewPort(int startIndex, int endIndex, const QRectF &pixelRect, double barWidth)
    : m_startIndex(startIndex), m_endIndex(endIndex), m_pixelRect(pixelRect), m_barWidth(barWidth) {
}

void ViewPort::setVisibleRange(int start, int end) {
    m_startIndex = start;
    m_endIndex = end;
}

int ViewPort::startIndex() const {
    return m_startIndex;
}

int ViewPort::endIndex() const {
    return m_endIndex;
}

int ViewPort::visibleCount() const {
    return m_endIndex - m_startIndex + 1;
}

void ViewPort::setPixelRect(const QRectF &rect) {
    m_pixelRect = rect;
}

QRectF ViewPort::pixelRect() const {
    return m_pixelRect;
}

double ViewPort::indexToPixel(int index) const {
    if (m_startIndex == m_endIndex) {
        return m_pixelRect.left();
    }

    double ratio = static_cast<double>(index - m_startIndex) /
                   static_cast<double>(m_endIndex - m_startIndex);
    return m_pixelRect.left() + ratio * m_pixelRect.width();
}

int ViewPort::pixelToIndex(double pixel) const {
    if (m_pixelRect.width() == 0.0) {
        return m_startIndex;
    }

    double ratio = (pixel - m_pixelRect.left()) / m_pixelRect.width();
    int index = m_startIndex + static_cast<int>(std::round(ratio * (m_endIndex - m_startIndex)));
    return index;
}

void ViewPort::setBarWidth(double width) {
    m_barWidth = width;
}

double ViewPort::barWidth() const {
    return m_barWidth;
}

} // namespace QTradingView
