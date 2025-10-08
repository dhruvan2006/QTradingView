#include "QTradingView/renderer/GridRenderer.h"
#include "QTradingView/renderer/AxisRenderer.h"
#include "QTradingView/Pane.h"
#include "QTradingView/ViewPort.h"
#include "QTradingView/data/IDataProvider.h"
#include "QTradingView/scale/IScale.h"
#include <QPainter>
#include <QPen>
#include <QDateTime>

namespace QTradingView {

GridRenderer::GridRenderer()
    : m_gridColor(QColor(42, 46, 57))
    , m_verticalGridEnabled(true)
    , m_horizontalGridEnabled(true) {
}

void GridRenderer::setGridColor(const QColor& color) {
    m_gridColor = color;
}

void GridRenderer::setVerticalGridEnabled(bool enabled) {
    m_verticalGridEnabled = enabled;
}

void GridRenderer::setHorizontalGridEnabled(bool enabled) {
    m_horizontalGridEnabled = enabled;
}

void GridRenderer::render(QPainter* painter, const Pane* pane, const ViewPort& viewport,
                         IDataProvider* dataProvider, const AxisRenderer* axisRenderer) {
    if (!painter || !pane) return;

    QRectF paneRect = pane->rect();
    IScale* scale = pane->scale();

    if (!scale) return;

    painter->save();
    painter->setClipRect(paneRect);

    QPen gridPen(m_gridColor, 1, Qt::SolidLine);
    painter->setPen(gridPen);
    painter->setRenderHint(QPainter::Antialiasing, false); // Sharp grid lines

    // Draw horizontal grid lines
    if (m_horizontalGridEnabled && axisRenderer) {
        double minValue = pane->minValue();
        double maxValue = pane->maxValue();

        std::vector<double> ticks = axisRenderer->calculateYAxisTicks(minValue, maxValue, 5);

        for (double value : ticks) {
            double y = scale->dataToPixel(value);
            painter->drawLine(QPointF(paneRect.left(), y), QPointF(paneRect.right(), y));
        }
    }

    // Draw vertical grid lines - use AxisRenderer's label calculation for perfect sync
    if (m_verticalGridEnabled && dataProvider && axisRenderer) {
        // Use the same label positions as the axis renderer
        std::vector<TimeLabel> labels = axisRenderer->calculateXAxisLabels(viewport, dataProvider);

        for (const auto& label : labels) {
            double x = viewport.indexToPixel(label.dataIndex);
            if (x >= paneRect.left() && x <= paneRect.right()) {
                painter->drawLine(QPointF(x, paneRect.top()), QPointF(x, paneRect.bottom()));
            }
        }
    }

    painter->restore();
}

} // namespace QTradingView
