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

#include "QTradingView/series/CandleStickSeries.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <QPainter>

#include "QTradingView/ViewPort.h"
#include "QTradingView/scale/IScale.h"

namespace QTradingView {
    CandleStickSeries::CandleStickSeries(const QList<CandleStick> &data)
        : Series(SeriesType::CandleStick)
          , m_data(data)
          , m_bullColor(Qt::green)
          , m_bearColor(Qt::red)
          , m_borderColor(Qt::transparent)
          , m_borderWidth(1.0)
          , m_bodyWidthRatio(0.7)
          , m_maxBodyWidthPx(100.0)
    #ifdef EMSCRIPTEN
          , m_antialiasing(false)
    #else
          , m_antialiasing(true)
    #endif
    {
    }

    CandleStickSeries::~CandleStickSeries() = default;

    void CandleStickSeries::setData(const QList<CandleStick> &data) {
        m_data = data;
    }

    const QList<CandleStick> & CandleStickSeries::data() const {
        return m_data;
    }

    qint64 CandleStickSeries::timestampAt(int index) const {
        if (index < 0 || index >= m_data.size()) {
            return -1;
        }
        return m_data[index].timeMsecs;
    }

    int CandleStickSeries::dataCount() const {
        return m_data.size();
    }

    void CandleStickSeries::setBullColor(const QColor &color) {
        m_bullColor = color;
    }

    void CandleStickSeries::setBearColor(const QColor &color) {
        m_bearColor = color;
    }

    void CandleStickSeries::setBorderColor(const QColor &color) {
        m_borderColor = color;
    }

    void CandleStickSeries::setBorderWidth(double width) {
        m_borderWidth = width;
    }

    void CandleStickSeries::setBodyWithRatio(double ratio) {
        m_bodyWidthRatio = ratio;
    }

    void CandleStickSeries::setMaxBodyWidth(double maxWidth) {
        m_maxBodyWidthPx = maxWidth;
    }

    void CandleStickSeries::setAntialiasing(bool enabled) {
        m_antialiasing = enabled;
    }

    void CandleStickSeries::render(QPainter *painter, const ViewPort &viewport, IScale *scale) {
        if (!painter || !scale) return;

        const int count = m_data.size();
        if (count <= 0) return;

        int start = std::max(viewport.startIndex(), 0);
        int end = std::min(viewport.endIndex(), count - 1);
        if (start > end) return;

        painter->setRenderHint(QPainter::Antialiasing, m_antialiasing);

        // Estimate per index horizontal step
        double stepX = 5.0;
        if (end > start) {
            const double pxStart = viewport.indexToPixel(start);
            const double pxEnd = viewport.indexToPixel(end);
            stepX = std::max(1.0, (pxEnd - pxStart) / static_cast<double>(end - start));
        }

        double bodyW = std::clamp(stepX * m_bodyWidthRatio, 1.0, m_maxBodyWidthPx);

        // Batching containers
        QVector<QLineF> bullWicks, bearWicks;
        QVector<QRectF> bullBodies, bearBodies;

        for (int i = start; i <= end; ++i) {
            const CandleStick& candle = m_data[i];
            const double x = viewport.indexToPixel(i);
            const double yH = scale->dataToPixel(candle.high);
            const double yL = scale->dataToPixel(candle.low);
            const double yO = scale->dataToPixel(candle.open);
            const double yC = scale->dataToPixel(candle.close);

            const bool bullish = (candle.close >= candle.open);
            const double topY = std::min(yO, yC);
            const double bodyH = std::abs(yC - yO);
            QRectF bodyRect(x - bodyW * 0.5, topY, bodyW, bodyH);
            QLineF wickLine(QPointF(x, yH), QPointF(x, yL));

            if (bullish) {
                bullWicks.append(wickLine);
                bullBodies.append(bodyRect);
            } else {
                bearWicks.append(wickLine);
                bearBodies.append(bodyRect);
            }
        }

        // Draw bullish wicks
        if (!bullWicks.isEmpty()) {
            QPen wickPen(m_bullColor, 1.0);
            painter->setPen(wickPen);
            painter->setBrush(Qt::NoBrush);
            painter->drawLines(bullWicks);
        }
        // Draw bearish wicks
        if (!bearWicks.isEmpty()) {
            QPen wickPen(m_bearColor, 1.0);
            painter->setPen(wickPen);
            painter->setBrush(Qt::NoBrush);
            painter->drawLines(bearWicks);
        }
        // Draw bullish bodies
        if (!bullBodies.isEmpty()) {
            painter->setBrush(m_bullColor);
            QPen borderPen(m_borderColor, m_borderWidth, Qt::SolidLine, Qt::SquareCap);
            painter->setPen(borderPen);
            painter->drawRects(bullBodies);
        }
        // Draw bearish bodies
        if (!bearBodies.isEmpty()) {
            painter->setBrush(m_bearColor);
            QPen borderPen(m_borderColor, m_borderWidth, Qt::SolidLine, Qt::SquareCap);
            painter->setPen(borderPen);
            painter->drawRects(bearBodies);
        }
    }

    bool CandleStickSeries::hitTest(const QPointF& point, int& outIndex) const {
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

    void CandleStickSeries::calculateRange(int startIndex, int endIndex, double& outMin, double& outMax) const {
        outMin = std::numeric_limits<double>::max();
        outMax = std::numeric_limits<double>::lowest();

        const int count = m_data.size();
        if (count <= 0) return;

        if (endIndex < 0 || startIndex >= count) return;

        startIndex = std::clamp(startIndex, 0, count - 1);
        endIndex   = std::clamp(endIndex,   0, count - 1);

        for (int i = startIndex; i <= endIndex; ++i) {
            const CandleStick& candle = m_data[i];
            outMin = std::min(outMin, candle.low);
            outMax = std::max(outMax, candle.high);
        }
    }
} // namespace QTradingView