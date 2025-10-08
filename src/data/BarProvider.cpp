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

#include "QTradingView/data/BarProvider.h"

namespace QTradingView {

    BarProvider::BarProvider(const QList<Bar> &data)
        : m_data(data) {
    }

    int BarProvider::count() const {
        return m_data.size();
    }

    TimePoint BarProvider::timeAt(int index) const {
        if (index < 0 || index >= m_data.size()) return TimePoint();
        return m_data[index].time();
    }

    QVariant BarProvider::valueAt(int index) const {
        if (index < 0 || index >= m_data.size()) return QVariant();
        return m_data[index].value();
    }

    void BarProvider::getRange(int start, int end, QVariantList &out) const {
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

    void BarProvider::setData(const QList<Bar> &data) {
        m_data = data;
    }

    const QList<Bar>& BarProvider::data() const {
        return m_data;
    }

} // namespace QTradingView

