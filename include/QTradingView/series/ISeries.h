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

#ifndef QTRADINGVIEW_ISERIES_H
#define QTRADINGVIEW_ISERIES_H

#include "../data/IDataProvider.h"
#include "../scale/IScale.h"
#include <memory>
#include <QString>
#include <QPainter>
#include <QPointF>
#include "../ViewPort.h"
#include "../style/SeriesStyle.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT ISeries
{
public:
    virtual ~ISeries() = default;

    [[nodiscard]] virtual QString type() const = 0;
    [[nodiscard]] virtual std::shared_ptr<IDataProvider> dataProvider() const = 0;

    virtual void setStyle(const SeriesStyle& style) = 0;

    virtual void render(QPainter* painter, const ViewPort& viewport, IScale* scale) = 0;

    [[nodiscard]] virtual bool hitTest(const QPointF& point, int& outIndex) const = 0;

    virtual void calculateRange(int startIndex, int endIndex, double& outMin, double& outMax) const = 0;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_ISERIES_H