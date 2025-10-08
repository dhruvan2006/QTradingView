#ifndef QTRADINGVIEW_CANDLESTICKSTYLE_H
#define QTRADINGVIEW_CANDLESTICKSTYLE_H

#include <QColor>
#include "QTradingView/style/SeriesStyle.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class CandleStickStyle : public SeriesStyle
{
public:
    // Body
    QColor bullishColor = QColor(8, 153, 129);
    QColor bearishColor = QColor(242, 54, 69);
    QColor borderColor = QColor(30, 30, 30);

    // Wick
    int wickWidth = 1;

    // Body sizing
    double bodyWidthRatio = 0.6;
    double maxBodyWidthPx = 20.0;
    double minBodyHeight = 1.0;

    // Body rendering
    bool drawBody = true;
    bool hollowCandles = false;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_CANDLESTICKSTYLE_H