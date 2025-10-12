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

#ifndef QTRADINGVIEW_H
#define QTRADINGVIEW_H

#include <QWidget>
#include <memory>
#include <QPoint>
#include "Chart.h"
#include <QGestureEvent>
#include <QPinchGesture>
#include <QPanGesture>
#include "qtradingview_global.h"


namespace QTradingView {

class Chart;

class QTRADINGVIEW_EXPORT QTradingView : public QWidget
{
    Q_OBJECT

public:
    explicit QTradingView(QWidget* parent = nullptr);
    ~QTradingView() override;

    Chart* chart();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    bool event(QEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void handlePinchGesture(QPinchGesture* gesture);
    void handlePanGesture(QPanGesture* gesture);

    std::unique_ptr<Chart> m_chart;

    // Mouse interaction state
    bool m_isPanning;
    QPoint m_lastMousePos;
    int m_lastMouseIndex;
    double m_initialVisibleCount;

    // Axis drag state
    enum class DragMode {
        None,
        ChartPan,
        YAxisZoom,
        XAxisZoom,
        PaneResize
    };
    DragMode m_dragMode;
    Pane* m_dragPane;  // The pane being zoomed on Y-axis
    double m_dragStartValue;  // Starting price value for Y-axis zoom

    // Pane resize state
    int m_resizingBorderIndex;  // Index of the border being resized
    double m_minPaneHeight;  // Minimum height for a pane in pixels
};

} // namespace QTradingView

#endif // QTRADINGVIEW_H