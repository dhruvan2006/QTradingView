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

#include "QTradingView/renderer/AxisRenderer.h"
#include <QFont>
#include <QDateTime>
#include <QTimeZone>

namespace {
    // Constants for time calculations (milliseconds)
    constexpr qint64 MS_PER_DAY = 86400000LL;

    // Cache for date components to avoid repeated QDateTime creation
    struct DateComponents {
        int year;
        int month;
        int day;

        bool operator!=(const DateComponents& other) const {
            return year != other.year || month != other.month || day != other.day;
        }
    };

    // Fast date component extraction from timestamp
    inline DateComponents getDateComponents(qint64 timestampMs) {
        // Convert to days since epoch
        qint64 days = timestampMs / MS_PER_DAY;

        // Algorithm to get year, month, day from days since epoch
        // This is faster than QDateTime creation
        qint64 z = days + 719468; // Adjust for epoch difference
        qint64 era = (z >= 0 ? z : z - 146096) / 146097;
        qint64 doe = z - era * 146097;
        qint64 yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;
        qint64 y = yoe + era * 400;
        qint64 doy = doe - (365*yoe + yoe/4 - yoe/100);
        qint64 mp = (5*doy + 2)/153;

        int day = static_cast<int>(doy - (153*mp+2)/5 + 1);
        int month = static_cast<int>(mp < 10 ? mp+3 : mp-9);
        int year = static_cast<int>(y + (month <= 2));

        return {year, month, day};
    }

    // Fast check if a timestamp is the first day of a month
    inline bool isFirstOfMonth(qint64 timestampMs) {
        qint64 days = timestampMs / MS_PER_DAY;
        qint64 z = days + 719468;
        qint64 era = (z >= 0 ? z : z - 146096) / 146097;
        qint64 doe = z - era * 146097;
        qint64 yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;
        qint64 doy = doe - (365*yoe + yoe/4 - yoe/100);
        qint64 mp = (5*doy + 2)/153;
        int day = static_cast<int>(doy - (153*mp+2)/5 + 1);
        return day == 1;
    }

    // Cache QTimeZone::utc() result
    const QTimeZone& utcZone() {
        // Construct from the "UTC" ID which works across Qt versions and
        // platforms (avoids referencing QTimeZone::UTC or QTimeZone::utc()
        // directly, which differ between builds).
        static const QTimeZone tz = QTimeZone(QByteArrayLiteral("UTC"));
        return tz;
    }

    // Format date string with minimal QDateTime usage
    QString formatDate(qint64 timestampMs, const QString& format) {
        static QDateTime dt;
        dt.setMSecsSinceEpoch(timestampMs);
        dt.setTimeZone(utcZone());
        return dt.toString(format);
    }

    // Format Y-axis label (translation-unit helper)
    static QString formatYAxisLabel(double value, double minValue, double maxValue, int availableWidth,
        const QFont &font) {
        double range = maxValue - minValue;

        if (std::abs(value) >= 1000000) {
            return QString::number(value / 1000000.0, 'f', 1) + "M";
        } else if (std::abs(value) >= 1000) {
            return QString::number(value / 1000.0, 'f', 1) + "K";
        }

        int precision = 2;
        if (range < 1.0) precision = 4;
        else if (range < 10.0) precision = 3;

        QString label = QString::number(value, 'f', precision);

        QFontMetrics fm(font);
        int textWidth = fm.horizontalAdvance(label);

        while (textWidth > availableWidth && precision > 0) {
            precision--;
            label = QString::number(value, 'f', precision);
            textWidth = fm.horizontalAdvance(label);
        }

        return label;
    }
}

namespace QTradingView {

AxisRenderer::AxisRenderer()
    : m_textColor(Qt::black)
    , m_tickColor(Qt::gray)
    , m_borderColor(Qt::gray)
    , m_backgroundColor(Qt::white)
    , m_axisWidth(60) {
}

void AxisRenderer::setTextColor(const QColor& color) {
    m_textColor = color;
}

void AxisRenderer::setTickColor(const QColor& color) {
    m_tickColor = color;
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
                                  const ViewPort& viewport, const Series* series) {
    if (!painter) return;

    painter->save();

    // Fill axis background
    painter->fillRect(axisRect, m_backgroundColor);

    // Draw separator line at top of X axis
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(axisRect.topLeft(), axisRect.topRight());

    painter->setPen(m_textColor);
    painter->setFont(QFont("Arial", 9));

    auto labels = calculateXAxisLabels(viewport, series);

    for (const auto& label : labels) {
        double x = viewport.indexToPixel(label.dataIndex);

        // Only draw labels that are within the chart area (not in the corners)
        if (x < viewport.pixelRect().left() || x > viewport.pixelRect().right()) {
            continue;
        }

        int pixelX = qRound(x);

        // Draw tick marks with grid color
        painter->setPen(QPen(m_tickColor, 1));
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

    double minValue = pane->minValue();
    double maxValue = pane->maxValue();

    IScale* scale = pane->scale();
    if (!scale) {
        painter->restore();
        return;
    }

    // Draw separator lines between chart and axes
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(leftAxisRect.topRight(), QPointF(leftAxisRect.right(), leftAxisRect.bottom()));
    painter->drawLine(rightAxisRect.topLeft(), QPointF(rightAxisRect.left(), rightAxisRect.bottom()));

    QFont labelFont = QFont("Arial", 9);
    painter->setFont(labelFont);

    auto ticks = scale->getTicks();

    int availableWidth = static_cast<int>(leftAxisRect.width() - 10);

    const int labelMargin = 5;

    for (double value : ticks) {
        double y = scale->dataToPixel(value);
        int pixelY = qRound(y);

        // Check if label is too close to top or bottom edge
        if (pixelY - 5 < leftAxisRect.top() + labelMargin ||
            pixelY + 5 > leftAxisRect.bottom() - labelMargin) {
            continue;
        }

        QString label = formatYAxisLabel(value, minValue, maxValue, availableWidth, labelFont);

        // Draw tick marks with grid color
        painter->setPen(QPen(m_tickColor, 1));
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

// TODO: We assume dataProvider provides daily data. Adjust logic for different timeframes if needed.
std::vector<TimeLabel> AxisRenderer::calculateXAxisLabels(const ViewPort& viewport, const Series* series) {
    std::vector<TimeLabel> labels;
    if (!series || series->dataCount() == 0) {
        return labels; // No data, no labels
    }

    int visibleCount = viewport.visibleCount();
    DateComponents lastLabelDate = {0, 0, 0};
    int dataCount = series->dataCount();

    // Estimate time interval
    qint64 timeIntervalMs = 86400000;
    if (dataCount >= 2) {
        timeIntervalMs = series->timestampAt(1) - series->timestampAt(0);
    }

    for (int i = 0; i < visibleCount; ++i) {
        int dataIndex = viewport.startIndex() + i;
        qint64 timestamp = 0;

        // Calculate datetime for this index (even if outside data range)
        if (dataIndex >= 0 && dataIndex < dataCount) {
            // Within data range - use actual data
            timestamp = series->timestampAt(dataIndex);
        } else if (dataCount > 0) {
            // Outside data range - extrapolate from first or last known time
            if (dataIndex < 0) {
                // Before data starts - extrapolate backwards from first point
                qint64 first = series->timestampAt(0);
                timestamp = first + dataIndex * timeIntervalMs;
            } else {
                // After data ends - extrapolate forwards from last point
                qint64 last = series->timestampAt(dataCount - 1);
                timestamp = last + (dataIndex - (dataCount - 1)) * timeIntervalMs;
            }
        } else {
            continue;
        }

        QString label;
        bool shouldLabel = false;

        if (visibleCount <= 30) {
            // Show every 5 days
            DateComponents date = getDateComponents(timestamp);
            int day = date.day;
            if (day == 1 || day == 5 || day == 10 || day == 15 || day == 20 || day == 25) {
                label = formatDate(timestamp, "d MMM");
                shouldLabel = true;
            }
        } else if (visibleCount <= 90) {
            // Show month starts
            if (isFirstOfMonth(timestamp)) {
                label = formatDate(timestamp, "MMM");
                shouldLabel = true;
            }
        }  else if (visibleCount <= 365) {
            // Show months, format: "Feb", "Mar", "Apr"
            DateComponents date = getDateComponents(timestamp);
            if (date.day == 1 && date.month != lastLabelDate.month) {
                label = formatDate(timestamp, "MMM");
                shouldLabel = true;
                lastLabelDate = date;
            }
        } else if (visibleCount <= 730) {
            // Show year at Jan, then months
            DateComponents date = getDateComponents(timestamp);
            if (date.day == 1 && date.month != lastLabelDate.month) {
                if (date.month == 1) {
                    label = formatDate(timestamp, "yyyy");
                } else {
                    label = formatDate(timestamp, "MMM");
                }
                shouldLabel = true;
                lastLabelDate = date;
            }
        } else {
            // Show years only
            DateComponents date = getDateComponents(timestamp);
            if (date.year != lastLabelDate.year) {
                label = formatDate(timestamp, "yyyy");
                shouldLabel = true;
                lastLabelDate = date;
            }
        }

        if (shouldLabel) {
            labels.push_back({timestamp, label, dataIndex});
        }
    }

    return labels;
}

} // namespace QTradingView
