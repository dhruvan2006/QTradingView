// Copyright (c) 2025 Dhruvan Gnanadhandayuthapani
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "QTradingView/data/CandleStickProvider.h"

namespace QTradingView {
    CandleStickProvider::CandleStickProvider(const QList<CandleStick> &data)
        : m_data(data) {
    }

    int CandleStickProvider::count() const {
        return m_data.size();
    }

    TimePoint CandleStickProvider::timeAt(int index) const {
        if (index < 0 || index >= m_data.size()) return TimePoint();

        return m_data[index].time();
    }

    QVariant CandleStickProvider::valueAt(int index) const {
        if (index < 0 || index >= m_data.size()) return QVariant();

        const CandleStick &candle = m_data[index];
        QVariantMap map;
        map.insert(QStringLiteral("open"), candle.open());
        map.insert(QStringLiteral("high"), candle.high());
        map.insert(QStringLiteral("low"), candle.low());
        map.insert(QStringLiteral("close"), candle.close());
        return map;
    }

    void CandleStickProvider::getRange(int start, int end, QVariantList &out) const {
        out.clear();

        if (m_data.empty()) return;

        int s = std::max(0, start);
        int e = std::min(end, static_cast<int>(m_data.size()) - 1);
        if (s > e) return;

        out.reserve(e - s + 1);
        for (int i = s; i <= e; ++i) {
            out.append(valueAt(i));
        }
    }
} // namespace QTradingView