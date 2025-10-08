#ifndef QTRADINGVIEW_BARPROVIDER_H
#define QTRADINGVIEW_BARPROVIDER_H

#include <QList>
#include "IDataProvider.h"
#include "Bar.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT BarProvider : public IDataProvider
{
public:
    explicit BarProvider(const QList<Bar> &data = QList<Bar>());
    ~BarProvider() override = default;

    [[nodiscard]] int count() const override;
    [[nodiscard]] TimePoint timeAt(int index) const override;
    [[nodiscard]] QVariant valueAt(int index) const override;
    void getRange(int start, int end, QVariantList &out) const override;

    void setData(const QList<Bar> &data);
    const QList<Bar>& data() const;

private:
    QList<Bar> m_data;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_BARPROVIDER_H

