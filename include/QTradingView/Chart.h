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

#ifndef QTRADINGVIEW_CHART_H
#define QTRADINGVIEW_CHART_H

#include <QObject>
#include <QWidget>
#include <QRectF>
#include <QTimer>
#include <QPointF>
#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <vector>
#include <memory>
#include <QPinchGesture>

#include "QTradingView/Pane.h"
#include "QTradingView/ViewPort.h"
#include "QTradingView/qtradingview_global.h"
#include "QTradingView/style/ChartTheme.h"
#include "renderer/AxisRenderer.h"
#include "renderer/AxisRenderer.h"
#include "renderer/GridRenderer.h"
#include "renderer/CrosshairRenderer.h"

class QPainter;

namespace QTradingView {

class QTRADINGVIEW_EXPORT Chart : public QWidget
{
    Q_OBJECT

public:
    explicit Chart(QWidget *parent = nullptr);
    ~Chart() override;

    Pane* addPane(double heightRatio = 1.0);
    void removePane(Pane* pane);
    Pane* mainPane();
    const std::vector<std::shared_ptr<Pane>>& panes() const;

    ViewPort& viewport();
    const ViewPort& viewport() const;

    void setSize(int width, int height);

    void setTheme(const ChartTheme& theme);
    const ChartTheme& theme() const;

    void calculateLayout();
    void render(QPainter* painter);

    void pan(int indexDelta);
    void zoom(int indexDelta, int anchorIndex);
    void fitToData();

    void setCrosshairVisible(bool visible);
    bool isCrosshairVisible() const;
    void setCrosshairPosition(const QPointF& position);
    QPointF crosshairPosition() const;

    // Axis region helpers
    QRectF leftAxisRect() const;
    QRectF rightAxisRect() const;
    QRectF xAxisRect() const;
    Pane* paneAtPosition(const QPointF& position) const;

    // Pane border detection
    int paneBorderAtPosition(const QPointF& position, double threshold = 5.0) const;

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

    // Chart logic members
    std::vector<std::shared_ptr<Pane>> m_panes;
    ViewPort m_viewport;
    ChartTheme m_theme;

    // Mouse/interaction state
    bool m_isPanning;
    QPoint m_lastMousePos;
    int m_lastMouseIndex;
    double m_initialVisibleCount;
    double m_wheelDeltaAccumulator = 0;
#ifdef EMSCRIPTEN
    static constexpr int WHEEL_THRESHOLD = 72;
#else
    static constexpr int WHEEL_THRESHOLD = 120;
#endif

    enum class DragMode { None, ChartPan, YAxisZoom, XAxisZoom, PaneResize };
    DragMode m_dragMode;
    Pane* m_dragPane;
    double m_dragStartValue;
    int m_resizingBorderIndex;
    double m_minPaneHeight;

    // Renderers
    AxisRenderer m_axisRenderer;
    GridRenderer m_gridRenderer;
    CrosshairRenderer m_crosshairRenderer;

    int m_leftAxisWidth;
    int m_rightAxisWidth;
    int m_xAxisHeight;

    // Crosshair state
    bool m_crosshairVisible;
    QPointF m_crosshairPosition;

    // Update throttling
    QTimer m_updateTimer;
    bool m_pendingUpdate = false;
    static constexpr int UPDATE_INTERVAL_MS = 16; // ~60 FPS (1000/60 ≈ 16ms)
    void setupUpdateTimer();
    void onUpdateTimerTick();
    void scheduleUpdate();
};

} // namespace QTradingView

#endif // QTRADINGVIEW_CHART_H
