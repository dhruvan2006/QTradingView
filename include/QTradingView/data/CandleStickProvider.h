#ifndef QTRADINGVIEW_CANDLESTICKPROVIDER_H
#define QTRADINGVIEW_CANDLESTICKPROVIDER_H

#include <QList>
#include <QVariantMap>

#include "IDataProvider.h"
#include "CandleStick.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT CandleStickProvider : public IDataProvider
{
public:
    explicit CandleStickProvider(const QList<CandleStick>& data = QList<CandleStick>());
    ~CandleStickProvider() override = default;

    [[nodiscard]] int count() const override;
    [[nodiscard]] TimePoint timeAt(int index) const override;
    [[nodiscard]] QVariant valueAt(int index) const override;
    void getRange(int start, int end, QVariantList& out) const override;

private:
    QList<CandleStick> m_data;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_CANDLESTICKPROVIDER_H