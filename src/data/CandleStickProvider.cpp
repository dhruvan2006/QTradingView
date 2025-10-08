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