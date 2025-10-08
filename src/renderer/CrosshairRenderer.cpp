#include "QTradingView/renderer/CrosshairRenderer.h"
#include "QTradingView/ViewPort.h"
#include "QTradingView/Pane.h"
#include "QTradingView/data/IDataProvider.h"
#include "QTradingView/scale/IScale.h"
#include <QPainter>
#include <QPen>
#include <QDateTime>

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
                                const Pane* pane, IDataProvider* dataProvider, double xAxisY) {
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
    if (dataProvider) {
        if (dataIndex >= 0 && dataIndex < dataProvider->count()) {
            QDateTime dt = dataProvider->timeAt(dataIndex).timestamp();
            QString timeStr = dt.toString("yyyy-MM-dd");
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
    painter->setRenderHint(QPainter::Antialiasing, true);

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
