#ifndef QTRADINGVIEW_ISERIES_H
#define QTRADINGVIEW_ISERIES_H

#include "../data/IDataProvider.h"
#include "../scale/IScale.h"
#include <memory>
#include <QString>
#include <QPainter>
#include <QPointF>
#include "../ViewPort.h"
#include "../style/SeriesStyle.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT ISeries
{
public:
    virtual ~ISeries() = default;

    [[nodiscard]] virtual QString type() const = 0;
    [[nodiscard]] virtual std::shared_ptr<IDataProvider> dataProvider() const = 0;

    virtual void setStyle(const SeriesStyle& style) = 0;

    virtual void render(QPainter* painter, const ViewPort& viewport, IScale* scale) = 0;

    [[nodiscard]] virtual bool hitTest(const QPointF& point, int& outIndex) const = 0;

    virtual void calculateRange(int startIndex, int endIndex, double& outMin, double& outMax) const = 0;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_ISERIES_H