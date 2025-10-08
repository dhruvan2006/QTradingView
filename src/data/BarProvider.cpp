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

