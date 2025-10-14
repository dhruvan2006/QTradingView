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

#include <QPainter>
#include "QTradingView/series/BarSeries.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include "QTradingView/ViewPort.h"
#include "QTradingView/scale/IScale.h"


namespace QTradingView {

    BarSeries::BarSeries(const QList<DataPoint> &data)
        : Series(SeriesType::Bar)
    , m_data(data)
    , m_upColor(Qt::green)
    , m_downColor(Qt::red)
    , m_barWidthRatio(0.7)
    , m_lineWidth(1.0)
    , m_antialiasing(true) {
    }


    BarSeries::~BarSeries() = default;

    void BarSeries::setData(const QList<DataPoint> &data) {
        m_data = data;
    }

    const QList<DataPoint> & BarSeries::data() const {
        return m_data;
    }

    QDateTime BarSeries::timestampAt(int index) const {
        if (index < 0 || index >= m_data.size()) {
            return QDateTime();
        }
        return m_data[index].time;
    }

    int BarSeries::dataCount() const {
        return m_data.size();
    }

    void BarSeries::setUpColor(const QColor &color) {
        m_upColor = color;
    }

    void BarSeries::setDownColor(const QColor &color) {
        m_downColor = color;
    }

    void BarSeries::setBarWidthRatio(double ratio) {
        m_barWidthRatio = std::clamp(ratio, 0.1, 1.0);
    }

    void BarSeries::setLineWidth(double width) {
        m_lineWidth = width;
    }

    void BarSeries::setAntialiasing(bool enabled) {
        m_antialiasing = enabled;
    }

    void BarSeries::render(QPainter *painter, const ViewPort &viewport, IScale *scale) {
        if (!painter || !scale) return;

        const int count = m_data.size();
        if (count <= 0) return;

        int start = std::max(viewport.startIndex(), 0);
        int end = std::min(viewport.endIndex(), count - 1);
        if (start > end) return;

        painter->setRenderHint(QPainter::Antialiasing, m_antialiasing);

        // Calculate bar width based on available space
        double stepX = 5.0;
        if (end > start) {
            const double pxStart = viewport.indexToPixel(start);
            const double pxEnd = viewport.indexToPixel(end);
            stepX = std::max(1.0, (pxEnd - pxStart) / static_cast<double>(end - start));
        }

        // Use m_barWidth as a ratio
        double barWidth = stepX * m_barWidthRatio;

        // Get baseline (zero line) in pixel coordinates
        const double baselineY = scale->dataToPixel(0.0);

        for (int i = start; i <= end; ++i) {
            double value = m_data[i].value;

            const double x = viewport.indexToPixel(i);
            const double valueY = scale->dataToPixel(value);

            // Determine bar color
            QColor fillColor = (value >= 0.0) ? m_upColor : m_downColor;

            // Calculate bar rectangle
            const double barTop = std::min(baselineY, valueY);
            const double barHeight = std::abs(valueY - baselineY);

            QRectF barRect(x - barWidth * 0.5, barTop, barWidth, barHeight);

            // Draw bar
            painter->setBrush(fillColor);
            QPen pen(fillColor, m_lineWidth);
            painter->setPen(pen);
            painter->drawRect(barRect);
        }
    }

    bool BarSeries::hitTest(const QPointF& point, int& outIndex) const {
        const int count = m_data.size();
        if (count <= 0) return false;

        double minDist = std::numeric_limits<double>::max();
        int nearest = -1;
        for (int i = 0; i < count; ++i) {
            const double dist = std::abs(point.x() - double(i));
            if (dist < minDist) {
                minDist = dist;
                nearest = i;
            }
        }
        if (nearest >= 0) {
            outIndex = nearest;
            return true;
        }
        return false;
    }

    void BarSeries::calculateRange(int startIndex, int endIndex, double& outMin, double& outMax) const {
        outMin = std::numeric_limits<double>::max();
        outMax = std::numeric_limits<double>::lowest();

        const int count = m_data.size();
        if (count <= 0) return;

        if (endIndex < 0 || startIndex >= count) return;

        startIndex = std::clamp(startIndex, 0, count - 1);
        endIndex   = std::clamp(endIndex,   0, count - 1);

        // Include zero in the range for bar charts
        outMin = 0.0;
        outMax = 0.0;

        for (int i = startIndex; i <= endIndex; ++i) {
            double value = m_data[i].value;
            outMax = std::max(outMax, value);
            outMin = std::min(outMin, value);
        }
    }

} // namespace QTradingView
