#include "QTradingView/scale/LinearScale.h"

#include <stdexcept>

namespace QTradingView {

    LinearScale::LinearScale()
        : m_minValue(0.0), m_maxValue(0.0), m_minPixel(0.0), m_maxPixel(0.0) {
    }

    LinearScale::LinearScale(double minValue, double maxValue, double minPixel, double maxPixel)
        : m_minValue(minValue), m_maxValue(maxValue), m_minPixel(minPixel), m_maxPixel(maxPixel) {
    }

    double LinearScale::dataToPixel(double value) const {
        if (m_minValue == m_maxValue) return m_minPixel;
        double ratio = (value - m_minValue) / (m_maxValue - m_minValue);
        return m_minPixel + ratio * (m_maxPixel - m_minPixel);
    }

    double LinearScale::pixelToData(double pixel) const {
        if (m_minPixel == m_maxPixel) return m_minValue;
        double ratio = (pixel - m_minPixel) / (m_maxPixel - m_minPixel);
        return m_minValue + ratio * (m_maxValue - m_minValue);
    }

    void LinearScale::setDomain(double minValue, double maxValue) {
        if (maxValue == minValue) throw std::invalid_argument("minValue and maxValue cannot be equal");
        m_minValue = minValue;
        m_maxValue = maxValue;
    }

    void LinearScale::setRange(double minPixel, double maxPixel) {
        if (maxPixel == minPixel) throw std::invalid_argument("minPixel and maxPixel cannot be equal");
        m_minPixel = minPixel;
        m_maxPixel = maxPixel;
    }

    std::vector<double> LinearScale::getTicks(int approxCount) const {
        std::vector<double> ticks;
        if (approxCount <= 0) return ticks;

        double range = m_maxValue - m_minValue;
        double step = range / static_cast<double>(approxCount);
        for (int i = 0; i <= approxCount; i++) {
            ticks.push_back(m_minValue + i * step);
        }
        return ticks;
    }

} // namespace QTradingView
