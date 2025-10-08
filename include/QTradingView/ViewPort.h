#ifndef QTRADINGVIEW_VIEWPORT_H
#define QTRADINGVIEW_VIEWPORT_H

#include <QRectF>
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

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
