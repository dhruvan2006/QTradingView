#ifndef QTRADINGVIEW_CANDLESTICKSERIES_H
#define QTRADINGVIEW_CANDLESTICKSERIES_H

#include "QTradingView/series/ISeries.h"
#include "QTradingView/style/CandleStickStyle.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT CandleStickSeries : public ISeries
{
public:
    explicit CandleStickSeries(std::shared_ptr<IDataProvider> data);
    ~CandleStickSeries() override;

    [[nodiscard]] QString type() const override;
    [[nodiscard]] std::shared_ptr<IDataProvider> dataProvider() const override;

    void setStyle(const SeriesStyle& style) override;
    void render(QPainter* painter, const ViewPort& viewport, IScale* scale) override;
    bool hitTest(const QPointF& point, int& outIndex) const override;
    void calculateRange(int startIndex, int endIndex, double& outMin, double& outMax) const override;

private:
    std::shared_ptr<IDataProvider> m_data;
    CandleStickStyle m_style;

    static double mapOr(const QVariantMap& m, const char* key, double def = 0.0) {
        auto it = m.find(QLatin1String(key));
        return it == m.end() ? def : it->toDouble();
    }
};

} // namespace QTradingView

#endif // QTRADINGVIEW_CANDLESTICKSERIES_H
