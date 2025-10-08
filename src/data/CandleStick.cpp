#include "QTradingView/data/CandleStick.h"

namespace QTradingView {
    CandleStick::CandleStick()
        : m_open(0.0), m_high(0.0), m_low(0.0), m_close(0.0) {
    }

    CandleStick::CandleStick(const TimePoint &time, double open, double high, double low, double close)
        : m_time(time), m_open(open), m_high(high), m_low(low), m_close(close) {
    }

    TimePoint CandleStick::time() const {
        return m_time;
    }

    void CandleStick::setTime(const TimePoint &time) {
        m_time = time;
    }

    double CandleStick::open() const {
        return m_open;
    }

    void CandleStick::setOpen(double open) {
        m_open = open;
    }

    double CandleStick::high() const {
        return m_high;
    }

    void CandleStick::setHigh(double high) {
        m_high = high;
    }

    double CandleStick::low() const {
        return m_low;
    }

    void CandleStick::setLow(double low) {
        m_low = low;
    }

    double CandleStick::close() const {
        return m_close;
    }

    void CandleStick::setClose(double close) {
        m_close = close;
    }

    bool CandleStick::isBullish() {
        return m_close > m_open;
    }

    bool CandleStick::isBearish() {
        return m_close < m_open;
    }

    double CandleStick::bodyTop() {
        return std::max(m_open, m_close);
    }

    double CandleStick::bodyBottom() {
        return std::min(m_open, m_close);
    }

    double CandleStick::range() {
        return m_high - m_low;
    }
} // namespace QTradingView
