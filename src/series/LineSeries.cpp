#include "QTradingView/series/LineSeries.h"

namespace QTradingView {

    LineSeries::LineSeries(std::shared_ptr<IDataProvider> data)
        : m_data(std::move(data)) {
    }

    LineSeries::~LineSeries() = default;

    QString LineSeries::type() const {
        return QStringLiteral("LinearSeries");
    }

    std::shared_ptr<IDataProvider> LineSeries::dataProvider() const {
        return m_data;
    }

    void LineSeries::setStyle(const SeriesStyle &style) {
        auto lineStyle = dynamic_cast<const LineStyle*>(&style);
        if (lineStyle)
            m_style = *lineStyle;
    }

    void LineSeries::render(QPainter *painter, const ViewPort& viewport, IScale* scale) {
        if (!painter || !m_data || !scale) return;

        int count = m_data->count();
        if (count < 2) return;

        int start = std::max(viewport.startIndex(), 0);
        int end = std::min(viewport.endIndex(), count - 1);
        if (start >= end) return;

        QPen pen(m_style.color, m_style.width, m_style.style);
        painter->setPen(pen);
        painter->setRenderHint(QPainter::Antialiasing, true); // TODO: Wth is antialiasing

        QPointF prevPoint;
        bool first = true;

        for (int i = start; i <= end; ++i) {
            double value = m_data->valueAt(i).toDouble();
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
        if (!m_data) return false;

        int count = m_data->count();
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
        
        if (!m_data)
            return;

        int count = m_data->count();
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
            double v = m_data->valueAt(i).toDouble();
            outMin = std::min(outMin, v);
            outMax = std::max(outMax, v);
        }
    }


} // namespace QTradingView