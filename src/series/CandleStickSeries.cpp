#include "QTradingView/series/CandleStickSeries.h"
#include <algorithm>
#include <limits>
#include <cmath>

namespace QTradingView {
    CandleStickSeries::CandleStickSeries(std::shared_ptr<IDataProvider> data)
        : m_data(std::move(data)) {
    }

    CandleStickSeries::~CandleStickSeries() = default;

    QString CandleStickSeries::type() const {
        return QStringLiteral("CandleStickSeries");
    }

    std::shared_ptr<IDataProvider> CandleStickSeries::dataProvider() const {
        return m_data;
    }

    void CandleStickSeries::setStyle(const SeriesStyle &style) {
        if (auto cs = dynamic_cast<const CandleStickStyle*>(&style)) {
            m_style = *cs;
        }
    }

    void CandleStickSeries::render(QPainter *painter, const ViewPort &viewport, IScale *scale) {
        if (!painter || !m_data || !scale) return;

        const int count = m_data->count();
        if (count <= 0) return;

        int start = std::max(viewport.startIndex(), 0);
        int end = std::min(viewport.endIndex(), count - 1);
        if (start > end) return;

        painter->setRenderHint(QPainter::Antialiasing, false);

        // Estimate per index horizontal step
        double stepX = 5.0;
        if (end > start) {
            const double pxStart = viewport.indexToPixel(start);
            const double pxEnd = viewport.indexToPixel(end);
            stepX = std::max(1.0, (pxEnd - pxStart) / static_cast<double>(end - start));
        }

        double bodyW = std::clamp(stepX * m_style.bodyWidthRatio, 1.0, m_style.maxBodyWidthPx);

        QPen borderPen(m_style.borderColor, 1.0, Qt::SolidLine, Qt::SquareCap);

        for (int i = start; i <= end; ++i) {
            const QVariant v = m_data->valueAt(i);
            if (!v.isValid() || !v.canConvert<QVariantMap>()) continue;

            const QVariantMap map = v.toMap();
            const double open = mapOr(map, "open");
            const double high = mapOr(map, "high");
            const double low = mapOr(map, "low");
            const double close = mapOr(map, "close");

            const double x = viewport.indexToPixel(i);

            const double yH = scale->dataToPixel(high);
            const double yL = scale->dataToPixel(low);
            const double yO = scale->dataToPixel(open);
            const double yC = scale->dataToPixel(close);

            // Color
            const bool bullish = (close >= open);
            const QColor color = bullish ? m_style.bullishColor : m_style.bearishColor;

            // Wick
            QPen wickPen(color, m_style.wickWidth, Qt::SolidLine, Qt::FlatCap);
            painter->setPen(wickPen);
            painter->drawLine(QPointF(x, yH), QPointF(x, yL));

            // Body
            const double topY = std::min(yO, yC);
            double bodyH = std::abs(yC - yO);
            if (bodyH < m_style.minBodyHeight) bodyH = m_style.minBodyHeight;

            QRectF bodyRect(x - bodyW * 0.5, topY, bodyW, bodyH);
            if (bullish && m_style.hollowCandles) {
                painter->setBrush(Qt::NoBrush);
                painter->setPen(QPen(color, 1.0));
                painter->drawRect(bodyRect);
            } else {
                painter->setBrush(color);
                painter->setPen(Qt::NoPen);
                painter->drawRect(bodyRect);
            }
        }
    }

    bool CandleStickSeries::hitTest(const QPointF& point, int& outIndex) const {
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

    void CandleStickSeries::calculateRange(int startIndex, int endIndex, double& outMin, double& outMax) const {
        outMin = std::numeric_limits<double>::max();
        outMax = std::numeric_limits<double>::lowest();

        if (!m_data) return;
        const int count = m_data->count();
        if (count <= 0) return;

        if (endIndex < 0 || startIndex >= count) return;

        startIndex = std::clamp(startIndex, 0, count - 1);
        endIndex   = std::clamp(endIndex,   0, count - 1);

        for (int i = startIndex; i <= endIndex; ++i) {
            const QVariant v = m_data->valueAt(i);
            if (!v.isValid() || !v.canConvert<QVariantMap>()) continue;
            const QVariantMap m = v.toMap();

            const double h = mapOr(m, "high");
            const double l = mapOr(m, "low");

            if (std::isfinite(h)) outMax = std::max(outMax, h);
            if (std::isfinite(l)) outMin = std::min(outMin, l);
        }
    }
} // namespace QTradingView