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

#include "QTradingView/series/LineSeries.h"

#include <QPen>
#include <QPainter>
#include <QPointF>

#include "QTradingView/Data.h"
#include "QTradingView/ViewPort.h"
#include "QTradingView/scale/IScale.h"

namespace QTradingView {

    LineSeries::LineSeries(const QList<DataPoint> &data)
        : Series(SeriesType::Line)
        , m_data(data)
        , m_color(Qt::blue)
        , m_width(2.0)
        , m_lineStyle(Qt::SolidLine)
        , m_antialiasing(true) {
    }

    LineSeries::~LineSeries() = default;

    void LineSeries::setData(const QList<DataPoint> &data) {
        m_data = data;
    }

    const QList<DataPoint>& LineSeries::data() const {
        return m_data;
    }

    qint64 LineSeries::timestampAt(int index) const {
        if (index < 0 || index >= m_data.size()) {
            return -1;
        }
        return m_data[index].timeMsecs;
    }

    int LineSeries::dataCount() const {
        return m_data.size();
    }

    void LineSeries::setColor(const QColor &color) {
        m_color = color;
    }

    void LineSeries::setLineWidth(double width) {
        m_width = width;
    }

    void LineSeries::setLineStyle(Qt::PenStyle style) {
        m_lineStyle = style;
    }

    void LineSeries::setAntialiasing(bool enabled) {
        m_antialiasing = enabled;
    }

    void LineSeries::render(QPainter *painter, const ViewPort& viewport, IScale* scale) {
        if (!painter || !scale) return;

        int count = m_data.count();
        if (count < 2) return;

        int start = std::max(viewport.startIndex(), 0);
        int end = std::min(viewport.endIndex(), count - 1);
        if (start >= end) return;

        QPen pen(m_color, m_width, m_lineStyle);
        painter->setPen(pen);
        painter->setRenderHint(QPainter::Antialiasing, m_antialiasing);

        QPointF prevPoint;
        bool first = true;

        for (int i = start; i <= end; ++i) {
            double value = m_data[i].value;
            double x = viewport.indexToPixel(i);
            double y = scale->dataToPixel(value);

            QPointF pt(x, y);
            if (!first) {
                painter->drawLine(prevPoint, pt);
            } else {
                first = false;
            }
            prevPoint = pt;
        }
    }

    bool LineSeries::hitTest(const QPointF &point, int &outIndex) const {
        int count = m_data.size();
        if (count == 0) return false;

        double minDist = std::numeric_limits<double>::max();
        int nearestIndex = -1;

        // TODO: Wth? Would this even work
        for (int i = 0; i < count; ++i) {
            double x = i;
            double dist = std::abs(point.x() - x);
            if (dist < minDist) {
                minDist = dist;
                nearestIndex = i;
            }
        }

        if (nearestIndex != -1) {
            outIndex = nearestIndex;
            return true;
        }

        return false;
    }

    void LineSeries::calculateRange(int startIndex, int endIndex, double &outMin, double &outMax) const {
        // Initialize with invalid markers
        outMin = std::numeric_limits<double>::max();
        outMax = std::numeric_limits<double>::lowest();

        int count = m_data.count();
        if (count == 0)
            return;

        // Check if viewport is completely outside data range
        if (endIndex < 0 || startIndex >= count) {
            // Viewport is outside data bounds - return invalid range
            return;
        }

        // Clamp to valid data range
        startIndex = std::clamp(startIndex, 0, count - 1);
        endIndex = std::clamp(endIndex, 0, count - 1);


        for (int i = startIndex; i <= endIndex; ++i) {
            double v = m_data[i].value;
            outMin = std::min(outMin, v);
            outMax = std::max(outMax, v);
        }
    }

} // namespace QTradingView