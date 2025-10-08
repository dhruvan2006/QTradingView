#include "QTradingView/renderer/AxisRenderer.h"
#include <QFont>
#include <QDateTime>

namespace {
    // Helper to round to nice numbers (1, 2, 5, 10, 20, 50, etc.)
    double niceNumber(double range, bool round) {
        double exponent = std::floor(std::log10(range));
        double fraction = range / std::pow(10, exponent);
        double niceFraction;

        if (round) {
            if (fraction < 1.5) niceFraction = 1;
            else if (fraction < 3) niceFraction = 2;
            else if (fraction < 7) niceFraction = 5;
            else niceFraction = 10;
        } else {
            if (fraction <= 1) niceFraction = 1;
            else if (fraction <= 2) niceFraction = 2;
            else if (fraction <= 5) niceFraction = 5;
            else niceFraction = 10;
        }

        return niceFraction * std::pow(10, exponent);
    }
}

namespace QTradingView {

AxisRenderer::AxisRenderer()
    : m_textColor(Qt::black)
    , m_gridColor(Qt::gray)
    , m_borderColor(Qt::gray)
    , m_backgroundColor(Qt::white)
    , m_axisWidth(60) {
}

void AxisRenderer::setTextColor(const QColor& color) {
    m_textColor = color;
}

void AxisRenderer::setGridColor(const QColor& color) {
    m_gridColor = color;
}

void AxisRenderer::setBorderColor(const QColor& color) {
    m_borderColor = color;
}

void AxisRenderer::setBackgroundColor(const QColor& color) {
    m_backgroundColor = color;
}

void AxisRenderer::setAxisWidth(int width) {
    m_axisWidth = width;
}

void AxisRenderer::drawXAxis(QPainter* painter, const QRectF& axisRect,
                                  const ViewPort& viewport, IDataProvider* dataProvider) {
    if (!painter || !dataProvider) return;

    painter->save();

    // Fill axis background
    painter->fillRect(axisRect, m_backgroundColor);

    // Draw separator line at top of X axis
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(axisRect.topLeft(), axisRect.topRight());

    painter->setPen(m_textColor);
    painter->setFont(QFont("Arial", 9));

    auto labels = calculateXAxisLabels(viewport, dataProvider);

    for (const auto& label : labels) {
        double x = viewport.indexToPixel(label.dataIndex);

        // Only draw labels that are within the chart area (not in the corners)
        if (x < viewport.pixelRect().left() || x > viewport.pixelRect().right()) {
            continue;
        }

        int pixelX = qRound(x);

        // Draw tick marks with grid color
        painter->setPen(QPen(m_gridColor, 1));
        painter->drawLine(QPointF(pixelX, axisRect.top()), QPointF(pixelX, axisRect.top() + 5));

        // Draw text with text color
        painter->setPen(m_textColor);
        QRectF textRect(pixelX - 30, axisRect.top() + 5, 60, axisRect.height() - 5);
        painter->drawText(textRect, Qt::AlignCenter, label.label);
    }

    painter->restore();
}

void AxisRenderer::drawYAxis(QPainter* painter, const QRectF& leftAxisRect,
                                  const QRectF& rightAxisRect, const Pane* pane) {
    if (!painter || !pane) return;

    painter->save();

    // Fill axis backgrounds
    painter->fillRect(leftAxisRect, m_backgroundColor);
    painter->fillRect(rightAxisRect, m_backgroundColor);

    QRectF paneRect = pane->rect();
    double minValue = pane->minValue();
    double maxValue = pane->maxValue();

    // Draw separator lines between chart and axes
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(leftAxisRect.topRight(), QPointF(leftAxisRect.right(), leftAxisRect.bottom()));
    painter->drawLine(rightAxisRect.topLeft(), QPointF(rightAxisRect.left(), rightAxisRect.bottom()));

    painter->setPen(m_textColor);
    painter->setFont(QFont("Arial", 9));

    auto ticks = calculateYAxisTicks(minValue, maxValue, 5);

    for (double value : ticks) {
        double normalizedPos = (value - minValue) / (maxValue - minValue);
        double y = paneRect.bottom() - normalizedPos * paneRect.height();

        int pixelY = qRound(y);

        QString label = QString::number(value, 'f', 2);

        // Draw tick marks with grid color
        painter->setPen(QPen(m_gridColor, 1));
        painter->drawLine(QPointF(leftAxisRect.right() - 5, pixelY), QPointF(leftAxisRect.right(), pixelY));
        painter->drawLine(QPointF(rightAxisRect.left(), pixelY), QPointF(rightAxisRect.left() + 5, pixelY));

        // Draw text with text color
        painter->setPen(m_textColor);

        // Right axis label
        QRectF rightRect(rightAxisRect.left() + 5, pixelY - 10, rightAxisRect.width() - 5, 20);
        painter->drawText(rightRect, Qt::AlignLeft | Qt::AlignVCenter, label);

        // Left axis label
        QRectF leftRect(leftAxisRect.left(), pixelY - 10, leftAxisRect.width() - 5, 20);
        painter->drawText(leftRect, Qt::AlignRight | Qt::AlignVCenter, label);
    }

    painter->restore();
}

std::vector<double> AxisRenderer::calculateYAxisTicks(double minValue, double maxValue, int approxTickCount) const {
    std::vector<double> ticks;

    if (maxValue <= minValue) return ticks;

    double range = niceNumber(maxValue - minValue, false);
    double tickSpacing = niceNumber(range / (approxTickCount - 1), true);
    double niceMin = std::floor(minValue / tickSpacing) * tickSpacing;
    double niceMax = std::ceil(maxValue / tickSpacing) * tickSpacing;

    for (double value = niceMin; value <= niceMax + 0.5 * tickSpacing; value += tickSpacing) {
        if (value >= minValue && value <= maxValue) {
            ticks.push_back(value);
        }
    }

    return ticks;
}

// TODO: We assume dataProvider provides daily data. Adjust logic for different timeframes if needed.
std::vector<TimeLabel> AxisRenderer::calculateXAxisLabels(const ViewPort& viewport, IDataProvider* dataProvider) const {
    std::vector<TimeLabel> labels;

    if (dataProvider->count() == 0) {
        return labels; // No data, no labels
    }

    int visibleCount = viewport.visibleCount();
    QDateTime lastLabelDate = QDateTime::fromMSecsSinceEpoch(0);
    int dataCount = dataProvider->count();

    // Calculate the time interval between data points (assume uniform spacing)
    qint64 timeIntervalMs = 86400000;
    if (dataCount >= 2) {
        QDateTime t0 = dataProvider->timeAt(0).timestamp();
        QDateTime t1 = dataProvider->timeAt(1).timestamp();
        timeIntervalMs = t0.msecsTo(t1);
    }

    for (int i = 0; i < visibleCount; ++i) {
        int dataIndex = viewport.startIndex() + i;
        QDateTime dt;

        // Calculate datetime for this index (even if outside data range)
        if (dataIndex >= 0 && dataIndex < dataCount) {
            // Within data range - use actual data
            dt = dataProvider->timeAt(dataIndex).timestamp();
        } else if (dataCount > 0) {
            // Outside data range - extrapolate from first or last known time
            if (dataIndex < 0) {
                // Before data starts - extrapolate backwards from first point
                QDateTime firstTime = dataProvider->timeAt(0).timestamp();
                dt = firstTime.addMSecs(dataIndex * timeIntervalMs);
            } else {
                // After data ends - extrapolate forwards from last point
                QDateTime lastTime = dataProvider->timeAt(dataCount - 1).timestamp();
                qint64 offsetFromLast = (dataIndex - (dataCount - 1)) * timeIntervalMs;
                dt = lastTime.addMSecs(offsetFromLast);
            }
        } else {
            continue;
        }

        QString label;
        bool shouldLabel = false;

        if (visibleCount <= 30) {
            // Show every 5 days
            if (dt.date().day() == 1 || dt.date().day() == 5 ||
                dt.date().day() == 10 || dt.date().day() == 15 ||
                dt.date().day() == 20 || dt.date().day() == 25) {
                label = dt.toString("d MMM");
                shouldLabel = true;
            }
        } else if (visibleCount <= 90) {
            // Show month starts
            if (dt.date().day() == 1) {
                label = dt.toString("MMM");
                shouldLabel = true;
            }
        } else if (visibleCount <= 365) {
            // Show months, format: "Feb", "Mar", "Apr"
            if (dt.date().day() == 1 && dt.date().month() != lastLabelDate.date().month()) {
                label = dt.toString("MMM");
                shouldLabel = true;
            }
        } else if (visibleCount <= 730) {
            // Show year at Jan, then months
            if (dt.date().day() == 1 && dt.date().month() != lastLabelDate.date().month()) {
                if (dt.date().month() == 1) {
                    label = dt.toString("yyyy");
                } else {
                    label = dt.toString("MMM");
                }
                shouldLabel = true;
            }
        } else {
            // Show years only
            if (dt.date().year() != lastLabelDate.date().year()) {
                label = dt.toString("yyyy");
                shouldLabel = true;
            }
        }

        if (shouldLabel) {
            labels.push_back({dt, label, dataIndex});
            lastLabelDate = dt;
        }
    }

    return labels;
}

} // namespace QTradingView
