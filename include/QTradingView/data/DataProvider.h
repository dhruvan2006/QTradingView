#ifndef QTRADINGVIEW_DATAPROVIDER_H
#define QTRADINGVIEW_DATAPROVIDER_H

#include <QList>
#include "IDataProvider.h"
#include "DataPoint.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT DataProvider : public IDataProvider
{
public:
    explicit DataProvider(const QList<DataPoint> &data = QList<DataPoint>());
    ~DataProvider() override = default;

    [[nodiscard]] int count() const override;
    [[nodiscard]] TimePoint timeAt(int index) const override;
    [[nodiscard]] QVariant valueAt(int index) const override;
    void getRange(int start, int end, QVariantList &out) const override;

    void setData(const QList<DataPoint> &data);
    const QList<DataPoint>& data() const;

private:
    QList<DataPoint> m_data;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_DATAPROVIDER_H