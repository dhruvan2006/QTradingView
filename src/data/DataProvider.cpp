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