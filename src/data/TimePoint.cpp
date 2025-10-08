#include "QTradingView/data/TimePoint.h"

namespace QTradingView {

    TimePoint::TimePoint()
        : m_timestamp(QDateTime()), m_index(-1) {
    }

    TimePoint::TimePoint(const QDateTime &timestamp, int index)
        : m_timestamp(timestamp), m_index(index) {
    }

    QDateTime TimePoint::timestamp() const {
        return m_timestamp;
    }

    void TimePoint::setTimestamp(const QDateTime &timestamp) {
        m_timestamp = timestamp;
    }

    int TimePoint::index() const {
        return m_index;
    }

    void TimePoint::setIndex(int index) {
        m_index = index;
    }

} // namespace QTradingView
