#ifndef QTRADINGVIEW_AXISRENDERER_H
#define QTRADINGVIEW_AXISRENDERER_H

#include <QPainter>
#include <QRectF>
#include <QColor>
#include <memory>
#include "QTradingView/ViewPort.h"
#include "QTradingView/Pane.h"
#include "QTradingView/data/IDataProvider.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

struct TimeLabel {
    QDateTime dateTime;
    QString label;
    int dataIndex;
};

class QTRADINGVIEW_EXPORT AxisRenderer
{
public:
    AxisRenderer();

    void setTextColor(const QColor& color);
    void setGridColor(const QColor& color);
    void setBorderColor(const QColor& color);
    void setBackgroundColor(const QColor& color);
    void setAxisWidth(int width);

    void drawXAxis(QPainter* painter, const QRectF& axisRect,
                   const ViewPort& viewport, IDataProvider* dataProvider);

    void drawYAxis(QPainter* painter, const QRectF& leftAxisRect,
                   const QRectF& rightAxisRect, const Pane* pane);

    std::vector<double> calculateYAxisTicks(double minValue, double maxValue, int approxTickCount) const;
    std::vector<TimeLabel> calculateXAxisLabels(const ViewPort& viewport, IDataProvider* dataProvider) const;

private:
    QColor m_textColor;
    QColor m_gridColor;
    QColor m_borderColor;
    QColor m_backgroundColor;
    int m_axisWidth;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_AXISRENDERER_H
