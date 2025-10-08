#ifndef QTRADINGVIEW_GRIDRENDERER_H
#define QTRADINGVIEW_GRIDRENDERER_H

#include <QColor>
#include "QTradingView/qtradingview_global.h"

class QPainter;
class QRectF;

namespace QTradingView {

class Pane;
class ViewPort;
class IDataProvider;
class AxisRenderer;

class QTRADINGVIEW_EXPORT GridRenderer
{
public:
    GridRenderer();

    void setGridColor(const QColor& color);
    void setVerticalGridEnabled(bool enabled);
    void setHorizontalGridEnabled(bool enabled);

    void render(QPainter* painter, const Pane* pane, const ViewPort& viewport,
                IDataProvider* dataProvider, const AxisRenderer* axisRenderer);

private:
    QColor m_gridColor;
    bool m_verticalGridEnabled;
    bool m_horizontalGridEnabled;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_GRIDRENDERER_H
