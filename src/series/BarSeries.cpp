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

#include "QTradingView/series/BarSeries.h"
#include <algorithm>
#include <limits>
#include <cmath>

namespace QTradingView {

    BarSeries::BarSeries(std::shared_ptr<IDataProvider> data)
        : m_data(std::move(data)) {
    }

    BarSeries::~BarSeries() = default;

    QString BarSeries::type() const {
        return QStringLiteral("BarSeries");
    }

    std::shared_ptr<IDataProvider> BarSeries::dataProvider() const {
        return m_data;
    }

    void BarSeries::setStyle(const SeriesStyle &style) {
        if (auto bs = dynamic_cast<const BarStyle*>(&style)) {
            m_style = *bs;
        }
    }

    void BarSeries::render(QPainter *painter, const ViewPort &viewport, IScale *scale) {
        if (!painter || !m_data || !scale) return;

        const int count = m_data->count();
        if (count <= 0) return;

        int start = std::max(viewport.startIndex(), 0);
        int end = std::min(viewport.endIndex(), count - 1);
        if (start > end) return;

        painter->setRenderHint(QPainter::Antialiasing, false);

        // Calculate bar width based on available space
        double stepX = 5.0;
        if (end > start) {
            const double pxStart = viewport.indexToPixel(start);
            const double pxEnd = viewport.indexToPixel(end);
            stepX = std::max(1.0, (pxEnd - pxStart) / static_cast<double>(end - start));
        }

        double barWidth = std::clamp(
            stepX * m_style.barWidthRatio,
            m_style.minBarWidthPx,
            m_style.maxBarWidthPx
        );

        // Get baseline (zero line) in pixel coordinates
        const double baselineY = scale->dataToPixel(0.0);

        for (int i = start; i <= end; ++i) {
            const QVariant v = m_data->valueAt(i);
            if (!v.isValid() || !v.canConvert<double>()) continue;

            const double value = v.toDouble();
            if (!std::isfinite(value)) continue;

            const double x = viewport.indexToPixel(i);
            const double valueY = scale->dataToPixel(value);

            // Determine bar color
            QColor fillColor = m_style.color;
            if (m_style.usePositiveNegativeColors) {
                fillColor = (value >= 0.0) ? m_style.positiveColor : m_style.negativeColor;
            }

            // Calculate bar rectangle
            const double barTop = std::min(baselineY, valueY);
            const double barHeight = std::abs(valueY - baselineY);

            QRectF barRect(x - barWidth * 0.5, barTop, barWidth, barHeight);

            // Draw bar
            painter->setBrush(fillColor);
            if (m_style.drawBorder) {
                QPen borderPen(m_style.borderColor, m_style.borderWidth);
                painter->setPen(borderPen);
            } else {
                painter->setPen(Qt::NoPen);
            }
            painter->drawRect(barRect);
        }
    }

    bool BarSeries::hitTest(const QPointF& point, int& outIndex) const {
        if (!m_data) return false;
        const int count = m_data->count();
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

        if (!m_data) return;
        const int count = m_data->count();
        if (count <= 0) return;

        if (endIndex < 0 || startIndex >= count) return;

        startIndex = std::clamp(startIndex, 0, count - 1);
        endIndex   = std::clamp(endIndex,   0, count - 1);

        // Include zero in the range for bar charts
        outMin = 0.0;
        outMax = 0.0;

        for (int i = startIndex; i <= endIndex; ++i) {
            const QVariant v = m_data->valueAt(i);
            if (!v.isValid() || !v.canConvert<double>()) continue;

            const double value = v.toDouble();
            if (!std::isfinite(value)) continue;

            outMax = std::max(outMax, value);
            outMin = std::min(outMin, value);
        }
    }

} // namespace QTradingView
