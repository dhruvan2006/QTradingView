#ifndef QTRADINGVIEW_LINEARSCALE_H
#define QTRADINGVIEW_LINEARSCALE_H

#include <vector>
#include "Iscale.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT LinearScale : public IScale
{
public:
    LinearScale();
    LinearScale(double minValue, double maxValue, double minPixel, double maxPixel);
    ~LinearScale() override = default;

    double dataToPixel(double value) const override;
    double pixelToData(double pixel) const override;
    void setDomain(double minValue, double maxValue) override;
    void setRange(double minPixel, double maxPixel) override;
    std::vector<double> getTicks(int approxCount) const override;

private:
    double m_minValue;
    double m_maxValue;
    double m_minPixel;
    double m_maxPixel;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_LINEARSCALE_H