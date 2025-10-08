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
