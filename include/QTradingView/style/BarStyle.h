#ifndef QTRADINGVIEW_BARSTYLE_H
#define QTRADINGVIEW_BARSTYLE_H

#include <QColor>
#include "QTradingView/style/SeriesStyle.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT BarStyle : public SeriesStyle
{
public:
    // Bar appearance
    QColor color = QColor(100, 150, 200);
    QColor borderColor = QColor(70, 120, 170);

    // Bar sizing
    double barWidthRatio = 0.8;
    double maxBarWidthPx = 50.0;
    double minBarWidthPx = 1.0;

    // Bar rendering
    bool drawBorder = false;
    double borderWidth = 1.0;

    // Coloring based on value
    bool usePositiveNegativeColors = true;
    QColor positiveColor = QColor(8, 153, 129);
    QColor negativeColor = QColor(242, 54, 69);
};

} // namespace QTradingView

#endif // QTRADINGVIEW_BARSTYLE_H
