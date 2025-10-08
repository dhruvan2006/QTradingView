#ifndef QTRADINGVIEW_CROSSHAIRRENDERER_H
#define QTRADINGVIEW_CROSSHAIRRENDERER_H

#include <QColor>
#include <QPointF>
#include <QFont>
#include "QTradingView/qtradingview_global.h"

class QPainter;
class QRectF;

namespace QTradingView {

class ViewPort;
class Pane;
class IDataProvider;

class QTRADINGVIEW_EXPORT CrosshairRenderer
{
public:
    CrosshairRenderer();

    void setCrosshairColor(const QColor& color);
    void setLabelBackgroundColor(const QColor& color);
    void setLabelTextColor(const QColor& color);
    void setFont(const QFont& font);

    void render(QPainter* painter, const QPointF& position, const ViewPort& viewport,
                const Pane* pane, IDataProvider* dataProvider, double xAxisY = -1.0);

private:
    QColor m_crosshairColor;
    QColor m_labelBackgroundColor;
    QColor m_labelTextColor;
    QFont m_font;

    void drawPriceLabel(QPainter* painter, double y, double price, const QRectF& chartArea, bool onRight);
    void drawTimeLabel(QPainter* painter, double x, const QString& timeStr, const QRectF& chartArea, double bottomY);
    void drawCrosshairMarker(QPainter* painter, const QPointF& position);
};

} // namespace QTradingView

#endif // QTRADINGVIEW_CROSSHAIRRENDERER_H
