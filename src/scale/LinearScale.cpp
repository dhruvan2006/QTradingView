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

    void LinearScale::applyPadding(double &minValue, double &maxValue, double paddingRatio) const {
        double padding = (maxValue - minValue) * paddingRatio;
        minValue -= padding;
        maxValue += padding;
    }

    void LinearScale::zoomDomain(double &minValue, double &maxValue, double zoomFactor, double anchorValue) const {
        double range = maxValue - minValue;
        double newRange = range * zoomFactor;
        double anchorRatio = (anchorValue - minValue) / range;

        minValue = anchorValue - anchorRatio * newRange;
        maxValue = minValue + newRange;
    }
} // namespace QTradingView
