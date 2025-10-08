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