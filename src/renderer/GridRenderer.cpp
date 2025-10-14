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

#include "QTradingView/renderer/GridRenderer.h"
#include "QTradingView/renderer/AxisRenderer.h"
#include "QTradingView/Pane.h"
#include "QTradingView/ViewPort.h"
#include "QTradingView/series/Series.h"
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
                         const Series* series, const AxisRenderer* axisRenderer) {
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

        std::vector<double> ticks = scale->getTicks();

        for (double value : ticks) {
            double y = scale->dataToPixel(value);
            painter->drawLine(QPointF(paneRect.left(), y), QPointF(paneRect.right(), y));
        }
    }

    // Draw vertical grid lines - use AxisRenderer's label calculation for perfect sync
    if (m_verticalGridEnabled && series && axisRenderer) {
        // Use the same label positions as the axis renderer
        std::vector<TimeLabel> labels = axisRenderer->calculateXAxisLabels(viewport, series);

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
