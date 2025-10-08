#ifndef QTRADINGVIEW_SERIESSTYLE_H
#define QTRADINGVIEW_SERIESSTYLE_H

#include <QColor>
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT SeriesStyle
{
public:
    virtual ~SeriesStyle() = default;

    QColor color = Qt::white;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_SERIESSTYLE_H
