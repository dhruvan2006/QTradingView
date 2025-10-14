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

#include "QTradingView/Pane.h"
#include "QTradingView/series/Series.h"
#include "QTradingView/scale/IScale.h"
#include <QPainter>
#include <algorithm>
#include <limits>

#include "QTradingView/scale/LinearScale.h"
#include "QTradingView/scale/LogScale.h"
#include "QTradingView/scale/ScaleType.h"

namespace QTradingView {

    Pane::Pane()
        : m_heightRatio(1.0)
          , m_minValue(0.0)
          , m_maxValue(0.0)
          , m_autoRange(true) {
        m_scale = std::make_unique<LinearScale>();
    }

    void Pane::addSeries(std::shared_ptr<Series> series) {
        if (!series) return;
        m_series.push_back(std::move(series));
    }

    void Pane::removeSeries(std::shared_ptr<Series> series) {
        if (!series) return;
        auto it = std::find(m_series.begin(), m_series.end(), series);
        if (it != m_series.end()) {
            m_series.erase(it);
        }
    }

    const std::vector<std::shared_ptr<Series> > &Pane::series() const {
        return m_series;
    }

    void Pane::setScale(ScaleType type) {
        switch (type) {
            case ScaleType::Linear:
                m_scale = std::make_unique<LinearScale>();
                break;
            case ScaleType::Logarithmic:
                m_scale = std::make_unique<LogScale>();
                break;
            default:
                m_scale = std::make_unique<LinearScale>();
                break;
        }
    }

    IScale* Pane::scale() const {
        return m_scale.get();
    }

    void Pane::setHeightRatio(double ratio) {
        m_heightRatio = ratio;
    }

    double Pane::heightRatio() const {
        return m_heightRatio;
    }

    void Pane::setRect(const QRectF &rect) {
        m_rect = rect;
    }

    QRectF Pane::rect() const {
        return m_rect;
    }

    void Pane::calculateRange(int start, int end) {
        if (m_series.empty()) {
            m_minValue = 0.0;
            m_maxValue = 1.0;
            return;
        }

        m_minValue = std::numeric_limits<double>::max();
        m_maxValue = std::numeric_limits<double>::lowest();

        for (const auto &series: m_series) {
            double seriesMin, seriesMax;
            series->calculateRange(start, end, seriesMin, seriesMax);

            m_minValue = std::min(m_minValue, seriesMin);
            m_maxValue = std::max(m_maxValue, seriesMax);
        }

        // Handle case where no valid data was found or all values are equal
        if (m_minValue == std::numeric_limits<double>::max() ||
            m_maxValue == std::numeric_limits<double>::lowest() ||
            m_minValue == m_maxValue) {
            // Set a default range to avoid scale errors
            m_minValue = 0.0;
            m_maxValue = 1.0;
            return;
        }

        // Add 5% padding to top and bottom
        if (m_scale) {
            m_scale->applyPadding(m_minValue, m_maxValue, 0.05);
        }
    }

    double Pane::minValue() const {
        return m_minValue;
    }

    double Pane::maxValue() const {
        return m_maxValue;
    }

    void Pane::setManualRange(double minValue, double maxValue) {
        m_minValue = minValue;
        m_maxValue = maxValue;
        m_autoRange = false;
    }

    void Pane::zoomYAxis(double zoomFactor, double anchorValue) {
        if (m_scale) {
            double min = m_minValue;
            double max = m_maxValue;
            m_scale->zoomDomain(min, max, zoomFactor, anchorValue);
            setManualRange(min, max);
            m_autoRange = false;
        }
    }

    void Pane::resetAutoRange() {
        m_autoRange = true;
    }

    bool Pane::isAutoRange() const {
        return m_autoRange;
    }

    void Pane::render(QPainter *painter, const ViewPort &viewport) {
        if (!painter || m_series.empty()) return;

        // Set clip rect to pane bounds
        painter->save();
        painter->setClipRect(m_rect);

        // Update scale range to match pane rect (inverted Y-axis for screen coordinates)
        if (m_scale) {
            m_scale->setRange(m_rect.bottom(), m_rect.top());
        }

        // Render all series with the pane's scale
        for (const auto &series: m_series) {
            series->render(painter, viewport, m_scale.get());
        }

        painter->restore();
    }

} // namespace QTradingView
