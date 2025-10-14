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

#include "QTradingView/scale/LogScale.h"

#include <stdexcept>
#include <cmath>

namespace QTradingView {
    LogScale::LogScale()
        : m_minValue(1.0), m_maxValue(10.0), m_minPixel(0.0), m_maxPixel(100.0), m_linearThreshold(1.0) {
    }

    LogScale::LogScale(double minValue, double maxValue, double minPixel, double maxPixel)
        : m_minValue(minValue), m_maxValue(maxValue), m_minPixel(minPixel), m_maxPixel(maxPixel), m_linearThreshold(1.0) {
    }

    double LogScale::symlog(double value) const {
        // Symmetric log transformation
        // For |value| < threshold: linear
        // For |value| >= threshold: logarithmic with smooth transition
        if (std::abs(value) <= m_linearThreshold) {
            return value;
        } else if (value > 0) {
            return m_linearThreshold * (std::log10(value / m_linearThreshold) + 1.0);
        } else {
            return -m_linearThreshold * (std::log10(-value / m_linearThreshold) + 1.0);
        }
    }

    double LogScale::invsymlog(double logValue) const {
        // Inverse symmetric log transformation
        if (std::abs(logValue) <= m_linearThreshold) {
            return logValue;
        } else if (logValue > 0) {
            return m_linearThreshold * std::pow(10.0, logValue / m_linearThreshold - 1.0);
        } else {
            return -m_linearThreshold * std::pow(10.0, -logValue / m_linearThreshold - 1.0);
        }
    }

    double LogScale::dataToPixel(double value) const {
        if (m_minValue == m_maxValue) return m_minPixel;

        double symlogMin = symlog(m_minValue);
        double symlogMax = symlog(m_maxValue);
        double symlogValue = symlog(value);

        double ratio = (symlogValue - symlogMin) / (symlogMax - symlogMin);
        return m_minPixel + ratio * (m_maxPixel - m_minPixel);
    }

    double LogScale::pixelToData(double pixel) const {
        if (m_minPixel == m_maxPixel) return m_minValue;

        double ratio = (pixel - m_minPixel) / (m_maxPixel - m_minPixel);
        double symlogMin = symlog(m_minValue);
        double symlogMax = symlog(m_maxValue);
        double symlogValue = symlogMin + ratio * (symlogMax - symlogMin);

        return invsymlog(symlogValue);
    }

    void LogScale::setDomain(double minValue, double maxValue) {
        if (maxValue == minValue) {
            throw std::invalid_argument("minValue and maxValue cannot be equal");
        }
        m_minValue = minValue;
        m_maxValue = maxValue;
    }

    void LogScale::setRange(double minPixel, double maxPixel) {
        if (maxPixel == minPixel) {
            throw std::invalid_argument("minPixel and maxPixel cannot be equal");
        }
        m_minPixel = minPixel;
        m_maxPixel = maxPixel;
    }

    void LogScale::setLinearThreshold(double threshold) {
        if (threshold <= 0.0) {
            throw std::invalid_argument("Linear threshold must be positive");
        }
        m_linearThreshold = threshold;
    }

    double LogScale::getLinearThreshold() const {
        return m_linearThreshold;
    }

    std::vector<double> LogScale::getTicks(int approxCount) const {
        std::vector<double> ticks;
        if (approxCount <= 0) return ticks;

        // Limit maximum ticks to prevent performance issues
        const int maxTicks = std::min(approxCount * 3, 100);

        double symlogMin = symlog(m_minValue);
        double symlogMax = symlog(m_maxValue);
        double symlogRange = symlogMax - symlogMin;

        // Determine if we're crossing zero
        bool crossesZero = (m_minValue < 0 && m_maxValue > 0);

        if (crossesZero) {
            // Add zero tick
            ticks.push_back(0.0);

            // Add positive ticks
            if (m_maxValue > m_linearThreshold) {
                int minPower = static_cast<int>(std::ceil(std::log10(m_linearThreshold)));
                int maxPower = static_cast<int>(std::ceil(std::log10(m_maxValue)));

                for (int power = minPower; power <= maxPower && ticks.size() < maxTicks; ++power) {
                    double tickValue = std::pow(10.0, power);
                    if (tickValue <= m_maxValue) {
                        ticks.push_back(tickValue);
                    }
                }
            } else if (m_maxValue > 0) {
                // Linear ticks in positive region - limit count
                int numSteps = std::min(approxCount / 2, 10);
                double step = m_maxValue / numSteps;
                for (int i = 1; i <= numSteps && ticks.size() < maxTicks; ++i) {
                    double v = i * step;
                    if (v <= m_maxValue) {
                        ticks.push_back(v);
                    }
                }
            }

            // Add negative ticks
            if (m_minValue < -m_linearThreshold) {
                int minPower = static_cast<int>(std::ceil(std::log10(m_linearThreshold)));
                int maxPower = static_cast<int>(std::ceil(std::log10(-m_minValue)));

                for (int power = minPower; power <= maxPower && ticks.size() < maxTicks; ++power) {
                    double tickValue = -std::pow(10.0, power);
                    if (tickValue >= m_minValue) {
                        ticks.push_back(tickValue);
                    }
                }
            } else if (m_minValue < 0) {
                // Linear ticks in negative region - limit count
                int numSteps = std::min(approxCount / 2, 10);
                double step = -m_minValue / numSteps;
                for (int i = 1; i <= numSteps && ticks.size() < maxTicks; ++i) {
                    double v = -i * step;
                    if (v >= m_minValue) {
                        ticks.push_back(v);
                    }
                }
            }
        } else if (m_minValue > 0 && m_maxValue > 0) {
            // All positive values
            if (m_minValue > m_linearThreshold) {
                // Pure logarithmic
                int minPower = static_cast<int>(std::floor(std::log10(m_minValue)));
                int maxPower = static_cast<int>(std::ceil(std::log10(m_maxValue)));

                for (int power = minPower; power <= maxPower && ticks.size() < maxTicks; ++power) {
                    double tickValue = std::pow(10.0, power);
                    if (tickValue >= m_minValue && tickValue <= m_maxValue) {
                        ticks.push_back(tickValue);
                    }

                    // Add intermediate ticks if there's space
                    if (maxPower - minPower <= 2 && ticks.size() < maxTicks) {
                        for (int mult = 2; mult <= 9 && ticks.size() < maxTicks; ++mult) {
                            double intermediateValue = mult * std::pow(10.0, power);
                            if (intermediateValue >= m_minValue && intermediateValue <= m_maxValue) {
                                ticks.push_back(intermediateValue);
                            }
                        }
                    }
                }
            } else {
                // Mixed linear and logarithmic - use fixed number of steps
                int numSteps = std::min(approxCount, 15);
                double step = (m_maxValue - m_minValue) / numSteps;
                for (int i = 0; i <= numSteps && ticks.size() < maxTicks; ++i) {
                    double v = m_minValue + i * step;
                    if (v >= m_minValue && v <= m_maxValue) {
                        ticks.push_back(v);
                    }
                }
            }
        } else {
            // All negative values
            if (m_maxValue < -m_linearThreshold) {
                // Pure logarithmic (negative)
                int minPower = static_cast<int>(std::floor(std::log10(-m_maxValue)));
                int maxPower = static_cast<int>(std::ceil(std::log10(-m_minValue)));

                for (int power = minPower; power <= maxPower && ticks.size() < maxTicks; ++power) {
                    double tickValue = -std::pow(10.0, power);
                    if (tickValue >= m_minValue && tickValue <= m_maxValue) {
                        ticks.push_back(tickValue);
                    }
                }
            } else {
                // Mixed linear and logarithmic (negative) - use fixed number of steps
                int numSteps = std::min(approxCount, 15);
                double step = (m_maxValue - m_minValue) / numSteps;
                for (int i = 0; i <= numSteps && ticks.size() < maxTicks; ++i) {
                    double v = m_minValue + i * step;
                    if (v >= m_minValue && v <= m_maxValue) {
                        ticks.push_back(v);
                    }
                }
            }
        }

        // Sort ticks
        std::sort(ticks.begin(), ticks.end());

        return ticks;
    }
    void LogScale::applyPadding(double &minValue, double &maxValue, double paddingRatio) const {
        if (minValue <= 0.0) minValue = std::max(minValue, 1e-12);

        double logMin = symlog(minValue);
        double logMax = symlog(maxValue);
        double padding = (logMax - logMin) * paddingRatio;

        logMin -= padding;
        logMax += padding;

        minValue = invsymlog(logMin);
        maxValue = invsymlog(logMax);
    }

    void LogScale::zoomDomain(double &minValue, double &maxValue, double zoomFactor, double anchorValue) const {
        if (minValue <= 0.0) minValue = 1e-12;
        if (maxValue <= 0.0) maxValue = 1e-12;
        if (anchorValue <= 0.0) anchorValue = 1e-12;

        double logMin = symlog(minValue);
        double logMax = symlog(maxValue);
        double logAnchor = symlog(anchorValue);

        double logRange = logMax - logMin;
        double newLogRange = logRange * zoomFactor;
        double anchorRatio = (logAnchor - logMin) / logRange;

        double newLogMin = logAnchor - anchorRatio * newLogRange;
        double newLogMax = logAnchor + (1.0 - anchorRatio) * newLogRange;

        minValue = invsymlog(newLogMin);
        maxValue = invsymlog(newLogMax);
    }
} // namespace QTradingView