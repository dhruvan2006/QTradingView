#include "QTradingView/data/DataPoint.h"

namespace QTradingView {

    DataPoint::DataPoint()
        : m_time(TimePoint()), m_value(0.0) {
    }

    DataPoint::DataPoint(const TimePoint &time, double value)
        : m_time(time), m_value(value) {
    }

    TimePoint DataPoint::time() const {
        return m_time;
    }

    void DataPoint::setTime(const TimePoint &time) {
        m_time = time;
    }

    double DataPoint::value() const {
        return m_value;
    }

    void DataPoint::setValue(double value) {
        m_value = value;
    }

} // namespace QTradingView
