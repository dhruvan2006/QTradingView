#ifndef QTRADINGVIEW_ISCALE_H
#define QTRADINGVIEW_ISCALE_H

#include <vector>
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT IScale
{
public:
    virtual ~IScale() = default;

    [[nodiscard]] virtual double dataToPixel(double value) const = 0;
    [[nodiscard]] virtual double pixelToData(double pixel) const = 0;

    virtual void setDomain(double minValue, double maxValue) = 0;
    virtual void setRange(double minPixel, double maxPixel) = 0;

    [[nodiscard]] virtual std::vector<double> getTicks(int approxCount) const = 0;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_ISCALE_H
