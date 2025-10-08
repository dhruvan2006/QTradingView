#include "QTradingView/data/Bar.h"

namespace QTradingView {

    Bar::Bar()
        : m_time(TimePoint()), m_value(0.0) {
    }

    Bar::Bar(const TimePoint &time, double value)
        : m_time(time), m_value(value) {
    }

    TimePoint Bar::time() const {
        return m_time;
    }

    void Bar::setTime(const TimePoint &time) {
        m_time = time;
    }

    double Bar::value() const {
        return m_value;
    }

    void Bar::setValue(double value) {
        m_value = value;
    }

} // namespace QTradingView

