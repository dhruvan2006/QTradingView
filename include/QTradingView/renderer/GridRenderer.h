/*
 * Copyright (c) 2025 Dhruvan Gnanadhandayuthapani
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef QTRADINGVIEW_GRIDRENDERER_H
#define QTRADINGVIEW_GRIDRENDERER_H

#include <QColor>
#include "QTradingView/qtradingview_global.h"

class QPainter;
class QRectF;

namespace QTradingView {

class Pane;
class ViewPort;
class AxisRenderer;
class Series;

/**
 * @class GridRenderer
 * @brief Renders grid lines on the chart background.
 */
class QTRADINGVIEW_EXPORT GridRenderer
{
public:
    /**
     * @brief Constructs a GridRenderer with default settings.
     */
    GridRenderer();

    /**
     * @brief Sets the color of the grid lines.
     * @param color The grid color.
     */
    void setGridColor(const QColor& color);
    /**
     * @brief Enables or disables vertical grid lines.
     * @param enabled True to enable, false to disable.
     */
    void setVerticalGridEnabled(bool enabled);
    /**
     * @brief Enables or disables horizontal grid lines.
     * @param enabled True to enable, false to disable.
     */
    void setHorizontalGridEnabled(bool enabled);

    /**
     * @brief Renders the grid on the chart pane.
     * @param painter The QPainter instance used for drawing.
     * @param pane The pane being rendered.
     * @param viewport The current viewport settings.
     * @param dataProvider The data series providing grid data.
     * @param axisRenderer The axis renderer for tick positions.
     */
    void render(QPainter* painter, const Pane* pane, const ViewPort& viewport,
                const Series* dataProvider, const AxisRenderer* axisRenderer);

private:
    QColor m_gridColor;
    bool m_verticalGridEnabled;
    bool m_horizontalGridEnabled;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_GRIDRENDERER_H
