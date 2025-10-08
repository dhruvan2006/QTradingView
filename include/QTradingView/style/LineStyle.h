#ifndef QTRADINGVIEW_LINESTYLE_H
#define QTRADINGVIEW_LINESTYLE_H

#include "SeriesStyle.h"
#include <Qt>
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT LineStyle : public SeriesStyle
{
public:
    LineStyle() = default;
    ~LineStyle() override = default;

    double width = 1.5;
    Qt::PenStyle style = Qt::SolidLine;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_LINESTYLE_H
