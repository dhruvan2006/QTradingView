#ifndef QTRADINGVIEW_BARSERIES_H
#define QTRADINGVIEW_BARSERIES_H

#include <memory>
#include "ISeries.h"
#include "../data/IDataProvider.h"
#include "../style/SeriesStyle.h"
#include "../style/BarStyle.h"
#include "../ViewPort.h"
#include "QTradingView/scale/IScale.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT BarSeries : public ISeries
{
public:
    explicit BarSeries(std::shared_ptr<IDataProvider> data);
    ~BarSeries() override;

    [[nodiscard]] QString type() const override;
    [[nodiscard]] std::shared_ptr<IDataProvider> dataProvider() const override;

    void setStyle(const SeriesStyle& style) override;
    void render(QPainter* painter, const ViewPort& viewport, IScale* scale) override;
    bool hitTest(const QPointF& point, int& outIndex) const override;
    void calculateRange(int startIndex, int endIndex, double& outMin, double& outMax) const override;

private:
    std::shared_ptr<IDataProvider> m_data;
    BarStyle m_style;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_BARSERIES_H

