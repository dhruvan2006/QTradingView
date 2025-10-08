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

