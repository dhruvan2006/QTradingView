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

/**
 * @class Chart
 * @brief Top level chart container for QTradingView.
 *
 * @details
 * The Chart class manages multiple panes, handles zoom/pan interactions,
 * crosshair rendering, and coordinate all series rendering.
 * It provides a high-performance, interactive charting experience
 * similar to TradingView lightweight charts.
 *
 * @code
 * QApplication app(argc, argv);
 * auto chart = new QTradingView::Chart();
 * auto pane = chart->addPane(1.0);
 * pane->addSeries(std::make_shared<QTradingView::CandleStickSeries>(data));
 * chart->show();
 * app.exec();
 * @endcode
 */
// TODO: Fix the shared in the @code
    class QTRADINGVIEW_EXPORT Chart : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Chart object.
     * @param parent Optional parent widget.
     */
    explicit Chart(QWidget *parent = nullptr);

    /**
     * @brief Destroys the Chart object.
     */
    ~Chart() override;

    /**
     * @brief Adds a new pane to the chart.
     * @param heightRatio Relative weight of the pane compared to others (default 1.0).
     * @return Pointer to the newly created Pane.
     */
    Pane* addPane(double heightRatio = 1.0);

    /**
     * @brief Removes a pane from the chart.
     * @param pane Pointer to the pane to remove.
     */
    void removePane(Pane* pane);

    /**
     * @brief Returns the main (first) pane of the chart.
     * @return Pointer to the main Pane.
     */
    Pane* mainPane();

    /**
     * @brief Returns all the panes in the chart.
     * @return Vector of pointers to all panes.
     */
    std::vector<Pane*> panes() const;

    /**
     * @brief Access the chart's viewport.
     * @return Reference to the ViewPort object.
     */
    ViewPort& viewport();

    /**
     * @brief Sets the chart theme.
     * @param theme ChartTheme object defining colors and styles.
     */
    void setTheme(const ChartTheme& theme);

    /**
     * @brief Gets the current chart theme.
     * @return Reference to the current ChartTheme.
     */
    const ChartTheme& theme() const;

    /**
     * @brief Recalculates the layout of all panes and axes based on the current chart size.
     */
    void calculateLayout();

    /**
     * @brief Pans the chart horizontally by a number of data points.
     * @param indexDelta Number of indices to pan (positive for right, negative for left).
     */
    void pan(int indexDelta);

    /**
     * @brief Zooms the chart horizontally by changing the number of visible data points.
     * @param indexDelta Change in the number of visible indices (positive to zoom out, negative to zoom in).
     * @param anchorIndex Data index to anchor the zoom operation.
     */
    void zoom(int indexDelta, int anchorIndex);

    /**
     * @brief Fits the chart view to show all available data, capped by the maximum allowed bars.
     */
    void fitToData();

    /**
     * @brief Shows the last N data points in the main pane, capped by the maximum allowed bars.
     * @param n Number of points to display.
     */
    void showLastNPoints(int n);

    /**
     * @brief Shows the chart with the default number of data points (e.g., last 300).
     */
    void show();

    /**
     * @brief Sets the crosshair visibility.
     * @param visible True to show the crosshair, false to hide.
     */
    void setCrosshairVisible(bool visible);

    /**
     * @brief Checks if the crosshair is currently visible.
     * @return True if the crosshair is visible, false otherwise.
     */
    bool isCrosshairVisible() const;

    /**
     * @brief Sets the crosshair position in chart coordinates.
     * @param position Position in pixels relative to the chart widget.
     */
    void setCrosshairPosition(const QPointF& position);

    /**
     * @brief Gets the current crosshair position in chart coordinates.
     * @return Position in pixels relative to the chart widget.
     */
    QPointF crosshairPosition() const;

    /**
     * @brief Access the axis renderer for customization.
     * @return Reference to the axis renderer.
     */
    AxisRenderer& axis();

    /**
     * @brief Access the grid renderer for customization.
     * @return Reference to the grid renderer.
     */
    GridRenderer& grid();

    /**
     * @brief Access the crosshair renderer for customization.
     * @return Reference to the crosshair renderer.
     */
    CrosshairRenderer& crosshair();

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
    // Gesture handling
    void handlePinchGesture(QPinchGesture* gesture);
    void handlePanGesture(QPanGesture* gesture);

    // Render
    void render(QPainter* painter);

    // Axis region helpers
    QRectF leftAxisRect() const;
    QRectF rightAxisRect() const;
    QRectF xAxisRect() const;

    // Pane helpers
    Pane* paneAtPosition(const QPointF& position) const;
    int paneBorderAtPosition(const QPointF& position, double threshold = 5.0) const;

    // Chart logic members
    std::vector<std::unique_ptr<Pane>> m_panes;
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
