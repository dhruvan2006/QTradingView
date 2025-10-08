#ifndef QTRADINGVIEW_PANE_H
#define QTRADINGVIEW_PANE_H

#include <QRectF>
#include <vector>
#include <memory>
#include "series/ISeries.h"
#include "scale/IScale.h"
#include "Viewport.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT Pane
{
public:
    Pane();

    void addSeries(std::shared_ptr<ISeries> series);
    void removeSeries(std::shared_ptr<ISeries> series);
    const std::vector<std::shared_ptr<ISeries>>& series() const;

    void setScale(std::shared_ptr<IScale> scale);
    IScale* scale() const;

    void setHeightRatio(double ratio);
    double heightRatio() const;

    void setRect(const QRectF &rect);
    QRectF rect() const;

    void calculateRange(int start, int end);
    double minValue() const;
    double maxValue() const;

    void setManualRange(double minValue, double maxValue);
    void zoomYAxis(double zoomFactor, double anchorValue);
    void resetAutoRange();
    bool isAutoRange() const;

    void render(QPainter *painter, const ViewPort &viewport);

private:
    std::vector<std::shared_ptr<ISeries>> m_series;
    std::shared_ptr<IScale> m_scale;
    double m_heightRatio;
    QRectF m_rect;
    double m_minValue;
    double m_maxValue;
    bool m_autoRange;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_PANE_H
