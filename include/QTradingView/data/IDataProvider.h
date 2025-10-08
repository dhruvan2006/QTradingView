#ifndef QTRADINGVIEW_IDATAPROVIDER_H
#define QTRADINGVIEW_IDATAPROVIDER_H

#include <QVariant>
#include <QVariantList>
#include "QTradingView/qtradingview_global.h"

#include "TimePoint.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT IDataProvider
{
public:
    virtual ~IDataProvider() = default;

    [[nodiscard]] virtual int count() const = 0;
    [[nodiscard]] virtual TimePoint timeAt(int index) const = 0;
    [[nodiscard]] virtual QVariant valueAt(int index) const = 0;
    virtual void getRange(int start, int end, QVariantList &out) const = 0;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_IDATAPROVIDER_H
