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

#include "QTradingView/renderer/CrosshairRenderer.h"
#include "QTradingView/ViewPort.h"
#include "QTradingView/Pane.h"
#include "QTradingView/series/Series.h"
#include "QTradingView/scale/IScale.h"
#include <QPainter>
#include <QPen>
#include <QDateTime>
#include <QTimeZone>

namespace QTradingView {

CrosshairRenderer::CrosshairRenderer()
    : m_crosshairColor(QColor(120, 123, 134))
    , m_labelBackgroundColor(QColor(42, 46, 57))
    , m_labelTextColor(QColor(209, 212, 220)) {
    m_font.setFamily("Arial");
    m_font.setPixelSize(11);
}

void CrosshairRenderer::setCrosshairColor(const QColor& color) {
    m_crosshairColor = color;
}

void CrosshairRenderer::setLabelBackgroundColor(const QColor& color) {
    m_labelBackgroundColor = color;
}

void CrosshairRenderer::setLabelTextColor(const QColor& color) {
    m_labelTextColor = color;
}

void CrosshairRenderer::setFont(const QFont& font) {
    m_font = font;
}

void CrosshairRenderer::render(QPainter* painter, const QPointF& position, const ViewPort& viewport,
                                const Pane* pane, const Series* series, double xAxisY) {
    if (!painter || !pane) return;

    QRectF paneRect = pane->rect();

    // Check if position is within pane
    if (!paneRect.contains(position)) return;

    painter->save();

    // Snap the X position to the nearest data point
    int dataIndex = viewport.pixelToIndex(position.x());
    double snappedX = viewport.indexToPixel(dataIndex);

    // Draw crosshair lines with dashed style
    QPen crosshairPen(m_crosshairColor, 1, Qt::DashLine);
    painter->setPen(crosshairPen);
    painter->setRenderHint(QPainter::Antialiasing, false);

    // NOTE: Vertical line is now drawn in Chart::render() across all panes
    // We only draw the horizontal line here (within this pane)

    // Horizontal line
    painter->drawLine(QPointF(paneRect.left(), position.y()),
                     QPointF(paneRect.right(), position.y()));

    // Draw thick crosshair marker at mouse position
    drawCrosshairMarker(painter, position);

    // Draw price label (Y-axis)
    if (IScale* scale = pane->scale()) {
        double price = scale->pixelToData(position.y());
        drawPriceLabel(painter, position.y(), price, paneRect, true);
    }

    // Draw time label (X-axis) and data value tooltip
    if (series) {
        if (dataIndex >= 0 && dataIndex < series->dataCount()) {
            qint64 timestamp = series->timestampAt(dataIndex);
            QString timeStr = QDateTime::fromMSecsSinceEpoch(timestamp, QTimeZone::UTC).toString("yyyy-MM-dd");
            // Use the provided xAxisY position if valid, otherwise use pane bottom
            double labelY = (xAxisY >= 0) ? xAxisY : paneRect.bottom();
            drawTimeLabel(painter, snappedX, timeStr, paneRect, labelY);
        }
    }

    painter->restore();
}

void CrosshairRenderer::drawPriceLabel(QPainter* painter, double y, double price,
                                       const QRectF& chartArea, bool onRight) {
    QString priceText = QString::number(price, 'f', 2);

    painter->setFont(m_font);
    QFontMetrics fm(m_font);
    QRectF textRect = fm.boundingRect(priceText);

    int padding = 4;
    int labelWidth = textRect.width() + padding * 2;
    int labelHeight = textRect.height() + padding * 2;

    QRectF labelRect;
    if (onRight) {
        labelRect = QRectF(chartArea.right() + 2, y - labelHeight / 2,
                          labelWidth, labelHeight);
    } else {
        labelRect = QRectF(chartArea.left() - labelWidth - 2, y - labelHeight / 2,
                          labelWidth, labelHeight);
    }

    // Draw label background
    painter->fillRect(labelRect, m_labelBackgroundColor);

    // Draw label border
    painter->setPen(QPen(m_crosshairColor, 1));
    painter->drawRect(labelRect);

    // Draw price text
    painter->setPen(m_labelTextColor);
    painter->drawText(labelRect, Qt::AlignCenter, priceText);
}

void CrosshairRenderer::drawTimeLabel(QPainter* painter, double x, const QString& timeStr,
                                      const QRectF& chartArea, double bottomY) {
    painter->setFont(m_font);
    QFontMetrics fm(m_font);
    QRectF textRect = fm.boundingRect(timeStr);

    int padding = 4;
    int labelWidth = textRect.width() + padding * 2;
    int labelHeight = textRect.height() + padding * 2;

    QRectF labelRect(x - labelWidth / 2, bottomY + 2, labelWidth, labelHeight);

    // Draw label background
    painter->fillRect(labelRect, m_labelBackgroundColor);

    // Draw label border
    painter->setPen(QPen(m_crosshairColor, 1));
    painter->drawRect(labelRect);

    // Draw time text
    painter->setPen(m_labelTextColor);
    painter->drawText(labelRect, Qt::AlignCenter, timeStr);
}

void CrosshairRenderer::drawCrosshairMarker(QPainter* painter, const QPointF& position) {
    painter->setRenderHint(QPainter::Antialiasing, false);

    // Draw a thick "+" marker at the mouse position with a brighter white color
    QPen markerPen(QColor(255, 255, 255), 3, Qt::SolidLine, Qt::RoundCap);
    painter->setPen(markerPen);

    int markerSize = 8;

    // Horizontal line of the "+"
    painter->drawLine(QPointF(position.x() - markerSize, position.y()),
                     QPointF(position.x() + markerSize, position.y()));

    // Vertical line of the "+"
    painter->drawLine(QPointF(position.x(), position.y() - markerSize),
                     QPointF(position.x(), position.y() + markerSize));
}

} // namespace QTradingView
