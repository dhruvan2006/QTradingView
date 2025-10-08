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

#include "QTradingView/data/DataProvider.h"

namespace QTradingView {
    DataProvider::DataProvider(const QList<DataPoint> &data)
        : m_data(data) {
    }

    int DataProvider::count() const {
        return m_data.size();
    }

    TimePoint DataProvider::timeAt(int index) const {
        if (index < 0 || index >= m_data.size()) return TimePoint();
        return m_data[index].time();
    }

    QVariant DataProvider::valueAt(int index) const {
        if (index < 0 || index >= m_data.size()) return QVariant();
        return m_data[index].value();
    }

    void DataProvider::getRange(int start, int end, QVariantList &out) const {
        out.clear();

        if (start < 0 || end >= m_data.size() || start > end) return;

        out.reserve(end - start + 1);
        for (int i = start; i <= end; ++i) {
            out.append(QVariant(m_data[i].value()));
        }
    }

    void DataProvider::setData(const QList<DataPoint> &data) {
        m_data = data;
    }

    const QList<DataPoint>& DataProvider::data() const {
        return m_data;
    }
} // namespace QTradingView