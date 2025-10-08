/*
 * Copyright (c) 2025 Dhruvan Gnanadhandayuthapani
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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