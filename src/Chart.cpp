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

#include "QTradingView/Chart.h"
#include "QTradingView/style/ChartTheme.h"
#include <algorithm>

namespace QTradingView {

    Chart::Chart(QWidget *parent)
        : QWidget(parent)
          , m_leftAxisWidth(60), m_rightAxisWidth(60), m_xAxisHeight(30)
          , m_theme(ChartTheme::tradingViewDark())
          , m_crosshairVisible(false)
          , m_isPanning(false)
          , m_lastMouseIndex(0)
          , m_initialVisibleCount(0)
          , m_dragMode(DragMode::None)
          , m_dragPane(nullptr)
          , m_dragStartValue(0.0)
          , m_resizingBorderIndex(-1)
          , m_minPaneHeight(50.0) {

        setMinimumSize(400, 300);
        setMouseTracking(true);
        setFocusPolicy(Qt::StrongFocus);
        setAttribute(Qt::WA_AcceptTouchEvents, true);
        grabGesture(Qt::PinchGesture);
        grabGesture(Qt::PanGesture);

        // Initialize axis renderer with theme colors
        m_axisRenderer.setTextColor(m_theme.axisTextColor);
        m_axisRenderer.setTickColor(m_theme.gridColor);
        m_axisRenderer.setBorderColor(m_theme.borderColor);
        m_axisRenderer.setBackgroundColor(m_theme.axisBackgroundColor);

        // Initialize grid renderer
        m_gridRenderer.setGridColor(m_theme.gridColor);

        // Initialize crosshair renderer
        m_crosshairRenderer.setCrosshairColor(m_theme.crosshairColor);
        m_crosshairRenderer.setLabelBackgroundColor(m_theme.axisBackgroundColor);
        m_crosshairRenderer.setLabelTextColor(m_theme.axisTextColor);
        m_crosshairRenderer.setFont(m_theme.textFont);

        // Setup update timer for throttling
        setupUpdateTimer();
    }

    Chart::~Chart() = default;

    std::shared_ptr<Pane> Chart::addPane(double heightRatio) {
        auto pane = std::make_shared<Pane>();
        pane->setHeightRatio(heightRatio);
        m_panes.push_back(pane);
        return pane;
    }

    void Chart::removePane(Pane *pane) {
        if (!pane) return;
        auto it = std::find_if(m_panes.begin(), m_panes.end(),
                               [pane](const std::shared_ptr<Pane> &p) { return p.get() == pane; });
        if (it != m_panes.end()) {
            m_panes.erase(it);
        }
    }

    Pane *Chart::mainPane() {
        return m_panes.empty() ? nullptr : m_panes[0].get();
    }

    std::vector<Pane*> Chart::panes() const {
        std::vector<Pane*> result;
        result.reserve(m_panes.size());
        for (const auto& pane : m_panes) {
            result.push_back(pane.get());
        }
        return result;
    }

    ViewPort &Chart::viewport() {
        return m_viewport;
    }

    void Chart::calculateLayout() {
        if (m_panes.empty()) return;

        double totalRatio = 0.0;
        for (const auto &pane: m_panes) {
            totalRatio += pane->heightRatio();
        }

        double chartWidth = width() - m_leftAxisWidth - m_rightAxisWidth;
        double chartHeight = height() - m_xAxisHeight;
        double currentY = 0.0;

        for (const auto &pane: m_panes) {
            double paneHeight = (pane->heightRatio() / totalRatio) * chartHeight;
            QRectF rect(m_leftAxisWidth, currentY, chartWidth, paneHeight);
            pane->setRect(rect);
            currentY += paneHeight;
        }

        m_viewport.setPixelRect(QRectF(m_leftAxisWidth, 0, chartWidth, chartHeight));
    }

    void Chart::setTheme(const ChartTheme &theme) {
        m_theme = theme;
        m_axisRenderer.setTextColor(m_theme.axisTextColor);
        m_axisRenderer.setTickColor(m_theme.gridColor);
        m_axisRenderer.setBorderColor(m_theme.borderColor);
        m_axisRenderer.setBackgroundColor(m_theme.axisBackgroundColor);

        m_gridRenderer.setGridColor(m_theme.gridColor);

        m_crosshairRenderer.setCrosshairColor(m_theme.crosshairColor);
        m_crosshairRenderer.setLabelBackgroundColor(m_theme.axisBackgroundColor);
        m_crosshairRenderer.setLabelTextColor(m_theme.axisTextColor);
        m_crosshairRenderer.setFont(m_theme.textFont);
    }

    const ChartTheme &Chart::theme() const {
        return m_theme;
    }

    void Chart::render(QPainter *painter) {
        if (!painter || m_panes.empty()) return;

        // Fill chart background
        painter->fillRect(0, 0, width(), height(), m_theme.backgroundColor);

        // Fill chart area background
        QRectF chartArea(m_leftAxisWidth, 0,
                         width() - m_leftAxisWidth - m_rightAxisWidth,
                         height() - m_xAxisHeight);
        painter->fillRect(chartArea, m_theme.chartBackgroundColor);

        for (const auto &pane: m_panes) {
            // Calculate range for visible data only if auto-range is enabled
            if (pane->isAutoRange()) {
                pane->calculateRange(m_viewport.startIndex(), m_viewport.endIndex());
            }

            // Update scale domain
            if (auto scale = pane->scale()) {
                scale->setDomain(pane->minValue(), pane->maxValue());
            }

            // Render grid first (behind series)
            Series* firstSeries = !pane->series().empty() ? pane->series()[0].get() : nullptr;
            m_gridRenderer.render(painter, pane.get(), m_viewport, firstSeries, &m_axisRenderer);

            // Render the pane
            pane->render(painter, m_viewport);

            // Draw Y axes for this pane
            QRectF leftAxisRect(0, pane->rect().top(), m_leftAxisWidth, pane->rect().height());
            QRectF rightAxisRect(width() - m_rightAxisWidth, pane->rect().top(),
                                 m_rightAxisWidth, pane->rect().height());
            m_axisRenderer.drawYAxis(painter, leftAxisRect, rightAxisRect, pane.get());
        }

        // Draw pane borders (after rendering all panes, so borders are on top)
        painter->save();
        QPen borderPen(m_theme.paneBorderColor, 2.5);
        painter->setPen(borderPen);
        painter->setRenderHint(QPainter::Antialiasing, false); // Sharp lines for borders

        for (size_t i = 0; i < m_panes.size(); ++i) {
            const auto &pane = m_panes[i];
            QRectF paneRect = pane->rect();

            // Draw horizontal line at the bottom of each pane except the last one
            if (i < m_panes.size() - 1) {
                painter->drawLine(QPointF(0, paneRect.bottom()),
                                  QPointF(width(), paneRect.bottom()));
            }
        }
        painter->restore();

        // Draw X axis
        Series* mainSeries = !m_panes.empty() && !m_panes[0]->series().empty()
                         ? m_panes[0]->series()[0].get()
                         : nullptr;
        QRectF xAxisRect(0, height() - m_xAxisHeight,
                         width(), m_xAxisHeight);
        m_axisRenderer.drawXAxis(painter, xAxisRect, m_viewport, mainSeries);

        // Render crosshair if visible (draw last, on top of everything)
        if (m_crosshairVisible && !m_panes.empty()) {
            // Find the pane that contains the crosshair
            Pane *activePane = nullptr;
            for (const auto &pane: m_panes) {
                if (pane->rect().contains(m_crosshairPosition)) {
                    activePane = pane.get();
                    break;
                }
            }

            if (activePane) {
                painter->save();

                // Snap the X position to the nearest data point
                int dataIndex = m_viewport.pixelToIndex(m_crosshairPosition.x());
                double snappedX = m_viewport.indexToPixel(dataIndex);

                // Draw vertical crosshair line across ALL panes (not clipped)
                QPen crosshairPen(m_theme.crosshairColor, 1, Qt::DashLine);
                painter->setPen(crosshairPen);
                painter->setRenderHint(QPainter::Antialiasing, false);

                // Draw vertical line from top of first pane to bottom of last pane
                double topY = m_panes.front()->rect().top();
                double bottomY = m_panes.back()->rect().bottom();
                painter->drawLine(QPointF(snappedX, topY), QPointF(snappedX, bottomY));

                painter->restore();

                // Now render the rest of the crosshair (horizontal line, labels, marker) for the active pane
                Series* activeSeries = !activePane->series().empty()
                                   ? activePane->series()[0].get()
                                   : nullptr;
                double xAxisY = height() - m_xAxisHeight;
                m_crosshairRenderer.render(painter, m_crosshairPosition, m_viewport,
                                           activePane, activeSeries, xAxisY);
            }
        }
    }

    void Chart::pan(int indexDelta) {
        int newStart = m_viewport.startIndex() + indexDelta;
        int newEnd = m_viewport.endIndex() + indexDelta;

        m_viewport.setVisibleRange(newStart, newEnd);
    }

    void Chart::zoom(int indexDelta, int anchorIndex) {
        int visibleCount = m_viewport.visibleCount();

        int newVisibleCount = visibleCount - indexDelta;
        // Prevent zooming to zero or negative
        if (newVisibleCount < 1) {
            newVisibleCount = 1;
        }

        if (newVisibleCount > MAX_VISIBLE_BARS) return;

        double anchorRatio = static_cast<double>(anchorIndex - m_viewport.startIndex()) / visibleCount;

        int newStart = anchorIndex - static_cast<int>(anchorRatio * newVisibleCount);
        int newEnd = newStart + newVisibleCount - 1;

        m_viewport.setVisibleRange(newStart, newEnd);
    }

    void Chart::fitToData() {
        if (m_panes.empty()) return;

        Pane* pane = mainPane();
        if (!pane || pane->series().empty()) return;

        int count = pane->series()[0]->dataCount();
        if (count > 0) {
            int start = 0;
            int end = count - 1;

            // If there are more bars than MAX_VISIBLE_BARS, show the last MAX_VISIBLE_BARS
            if (count > MAX_VISIBLE_BARS) {
                start = count - MAX_VISIBLE_BARS;
                end = count - 1;
            }

            m_viewport.setVisibleRange(start, end);
        }
    }

    void Chart::showLastNPoints(int n) {
        if (m_panes.empty()) return;

        Pane* pane = mainPane();
        if (!pane || pane->series().empty()) return;

        int count = pane->series()[0]->dataCount();
        if (count <= 0) return;

        n = std::clamp(n, 1, MAX_VISIBLE_BARS);

        // Ensure n is not greater than total points
        if (n > count) n = count;

        int startIndex = count - n;
        int endIndex = count - 1;

        m_viewport.setVisibleRange(startIndex, endIndex);
        scheduleUpdate();
    }

    void Chart::show() {
        showLastNPoints(300);
    }

    void Chart::setCrosshairVisible(bool visible) {
        m_crosshairVisible = visible;
    }

    AxisRenderer& Chart::axis() {
        return m_axisRenderer;
    }

    bool Chart::isCrosshairVisible() const {
        return m_crosshairVisible;
    }

    void Chart::setCrosshairPosition(const QPointF &position) {
        m_crosshairPosition = position;
    }

    QPointF Chart::crosshairPosition() const {
        return m_crosshairPosition;
    }

    QRectF Chart::leftAxisRect() const {
        if (m_panes.empty()) return QRectF();
        return QRectF(0, 0, m_leftAxisWidth, height() - m_xAxisHeight);
    }

    QRectF Chart::rightAxisRect() const {
        if (m_panes.empty()) return QRectF();
        return QRectF(width() - m_rightAxisWidth, 0, m_rightAxisWidth, height() - m_xAxisHeight);
    }

    QRectF Chart::xAxisRect() const {
        return QRectF(0, height() - m_xAxisHeight, width(), m_xAxisHeight);
    }

    Pane *Chart::paneAtPosition(const QPointF &position) const {
        for (const auto &pane: m_panes) {
            if (pane->rect().contains(position)) {
                return pane.get();
            }
        }
        return nullptr;
    }

    int Chart::paneBorderAtPosition(const QPointF &position, double threshold) const {
        // Returns the index of the border below the pane at the position
        // Returns -1 if no border is near the position
        for (size_t i = 0; i < m_panes.size() - 1; ++i) {
            const auto &pane = m_panes[i];
            double borderY = pane->rect().bottom();

            // Check if position is within threshold distance of the border
            if (std::abs(position.y() - borderY) <= threshold &&
                position.x() >= 0 && position.x() <= width()) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    void Chart::paintEvent(QPaintEvent *event) {
        QPainter painter(this);
        render(&painter);
    }

    void Chart::resizeEvent(QResizeEvent *event) {
        QWidget::resizeEvent(event);
        calculateLayout();
    }

    void Chart::wheelEvent(QWheelEvent *event) {
        int mouseX = event->position().x();
        int anchorIndex = m_viewport.pixelToIndex(mouseX);

        double wheelDelta = event->angleDelta().y();

        // accumulate the wheel delta
        m_wheelDeltaAccumulator += wheelDelta;

        if (std::abs(m_wheelDeltaAccumulator) >= WHEEL_THRESHOLD) {
            int zoomSteps = m_wheelDeltaAccumulator / WHEEL_THRESHOLD;
            m_wheelDeltaAccumulator = std::fmod(m_wheelDeltaAccumulator, WHEEL_THRESHOLD);

            // make zoom proportional to visible data count
            int visibleCount = m_viewport.visibleCount();
            double zoomSensitivity = std::max(2.0, visibleCount * 0.05); // Scale with visible count, minimum 2
            int indexDelta = static_cast<int>(zoomSteps * zoomSensitivity);

            if (indexDelta != 0) {
                zoom(indexDelta, anchorIndex);
                update();
            }
        }

        event->accept();
    }

    void Chart::mousePressEvent(QMouseEvent *event) {
        if (!hasFocus()) {
            setFocus();
        }

        if (event->button() == Qt::LeftButton) {
            QPointF pos = event->pos();

            // Check if clicking on a pane border for resizing
            int borderIndex = paneBorderAtPosition(pos);
            if (borderIndex >= 0) {
                m_dragMode = DragMode::PaneResize;
                m_resizingBorderIndex = borderIndex;
                m_lastMousePos = event->pos();
                setCursor(Qt::SplitVCursor);
                event->accept();
                return;
            }

            // Check if clicking on Y axis
            if (leftAxisRect().contains(pos) || rightAxisRect().contains(pos)) {
                for (const auto &pane: m_panes) {
                    QRectF paneRect = pane->rect();
                    if (pos.y() >= paneRect.top() && pos.y() <= paneRect.bottom()) {
                        m_dragMode = DragMode::YAxisZoom;
                        m_dragPane = pane.get();
                        m_lastMousePos = event->pos();

                        double normalizedPos = (paneRect.bottom() - pos.y()) / paneRect.height();
                        m_dragStartValue = pane->minValue() + normalizedPos * (pane->maxValue() - pane->minValue());

                        setCursor(Qt::SizeVerCursor);
                        event->accept();
                        return;
                    }
                }
            }
            // Check if clicking on X axis
            else if (xAxisRect().contains(pos)) {
                m_dragMode = DragMode::XAxisZoom;
                m_lastMousePos = event->pos();
                m_lastMouseIndex = m_viewport.pixelToIndex(event->pos().x());
                setCursor(Qt::SizeHorCursor);
                event->accept();
                return;
            }
            // Otherwise chart panning
            else {
                m_dragMode = DragMode::ChartPan;
                m_isPanning = true;
                m_lastMousePos = event->pos();
                m_lastMouseIndex = viewport().pixelToIndex(event->pos().x());
                setCursor(Qt::ClosedHandCursor);
                event->accept();
            }
        }
    }

    void Chart::mouseMoveEvent(QMouseEvent *event) {
        if (m_dragMode == DragMode::PaneResize && m_resizingBorderIndex >= 0) {
            int deltaY = event->pos().y() - m_lastMousePos.y();

            const auto &panes = m_panes;
            if (m_resizingBorderIndex < static_cast<int>(panes.size()) - 1) {
                auto &upperPane = panes[m_resizingBorderIndex];
                auto &lowerPane = panes[m_resizingBorderIndex + 1];

                double upperRatio = upperPane->heightRatio();
                double lowerRatio = lowerPane->heightRatio();

                double totalRatio = upperRatio + lowerRatio;
                double totalHeight = upperPane->rect().height() + lowerPane->rect().height();

                double newUpperHeight = upperPane->rect().height() + deltaY;
                double newLowerHeight = lowerPane->rect().height() - deltaY;

                if (newUpperHeight < m_minPaneHeight) {
                    newUpperHeight = m_minPaneHeight;
                    newLowerHeight = totalHeight - newUpperHeight;
                }
                if (newLowerHeight < m_minPaneHeight) {
                    newLowerHeight = m_minPaneHeight;
                    newUpperHeight = totalHeight - newLowerHeight;
                }

                double newUpperRatio = (newUpperHeight / totalHeight) * totalRatio;
                double newLowerRatio = (newLowerHeight / totalHeight) * totalRatio;

                upperPane->setHeightRatio(newUpperRatio);
                lowerPane->setHeightRatio(newLowerRatio);

                calculateLayout();

                m_lastMousePos = event->pos();
                scheduleUpdate();
                event->accept();
            }
        } else if (m_dragMode == DragMode::YAxisZoom && m_dragPane) {
            int deltaY = event->pos().y() - m_lastMousePos.y();
            double zoomFactor = 1.0 + (deltaY * 0.01);
            m_dragPane->zoomYAxis(zoomFactor, m_dragStartValue);

            m_lastMousePos = event->pos();
            scheduleUpdate();
            event->accept();
        } else if (m_dragMode == DragMode::XAxisZoom) {
            int deltaX = event->pos().x() - m_lastMousePos.x();

            int visibleCount = m_viewport.visibleCount();
            double zoomSensitivity = std::max(0.01, visibleCount * 0.002);
            int zoomDelta = static_cast<int>(-deltaX * zoomSensitivity);

            if (zoomDelta != 0) {
                zoom(zoomDelta, m_lastMouseIndex);
            }

            m_lastMousePos = event->pos();
            scheduleUpdate();
            event->accept();
        } else if (m_dragMode == DragMode::ChartPan && m_isPanning) {
            int currentIndex = m_viewport.pixelToIndex(event->pos().x());
            int indexDelta = m_lastMouseIndex - currentIndex;

            if (indexDelta != 0) {
                pan(indexDelta);
                m_lastMouseIndex = m_viewport.pixelToIndex(event->pos().x());
            }

            int deltaY = event->pos().y() - m_lastMousePos.y();
            if (deltaY != 0) {
                for (const auto &pane: m_panes) {
                    if (pane->rect().contains(event->pos()) && !pane->isAutoRange()) {
                        auto scale = pane->scale();
                        if (!scale) continue;

                        double minData = scale->pixelToData(pane->rect().bottom() - deltaY);
                        double maxData = scale->pixelToData(pane->rect().top() - deltaY);

                        pane->setManualRange(minData, maxData);
                    }
                }
            }

            m_lastMousePos = event->pos();

            if (m_crosshairVisible)
                setCrosshairPosition(event->pos());

            scheduleUpdate();
            event->accept();
        } else {
            QPointF pos = event->pos();

            int borderIndex = paneBorderAtPosition(pos);
            if (borderIndex >= 0) {
                setCrosshairVisible(false);
                setCursor(Qt::SplitVCursor);
                scheduleUpdate();
            } else if (leftAxisRect().contains(pos) || rightAxisRect().contains(pos)) {
                setCrosshairVisible(false);
                setCursor(Qt::SizeVerCursor);
                scheduleUpdate();
            } else if (xAxisRect().contains(pos)) {
                setCrosshairVisible(false);
                setCursor(Qt::SizeHorCursor);
                scheduleUpdate();
            } else {
                setCursor(Qt::ArrowCursor);
                setCrosshairVisible(true);
                setCrosshairPosition(event->pos());
                scheduleUpdate();
            }
        }
    }

    void Chart::mouseReleaseEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
            m_dragMode = DragMode::None;
            m_isPanning = false;
            m_dragPane = nullptr;
            setCursor(Qt::ArrowCursor);
            event->accept();
        }
    }

    void Chart::mouseDoubleClickEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
            QPointF pos = event->pos();

            if (leftAxisRect().contains(pos) || rightAxisRect().contains(pos)) {
                for (const auto &pane: m_panes) {
                    QRectF paneRect = pane->rect();
                    if (pos.y() >= paneRect.top() && pos.y() <= paneRect.bottom()) {
                        pane->resetAutoRange();
                        scheduleUpdate();
                        event->accept();
                        return;
                    }
                }
            } else if (xAxisRect().contains(pos)) {
                fitToData();
                scheduleUpdate();
                event->accept();
                return;
            }
        }
    }

    void Chart::leaveEvent(QEvent *event) {
        setCrosshairVisible(false);
        scheduleUpdate();
        QWidget::leaveEvent(event);
    }

    bool Chart::event(QEvent *event) {
        if (event->type() == QEvent::Gesture) {
            QGestureEvent *gestureEvent = dynamic_cast<QGestureEvent *>(event);

            if (QGesture *pinch = gestureEvent->gesture(Qt::PinchGesture)) {
                handlePinchGesture(dynamic_cast<QPinchGesture *>(pinch));
                return true;
            }

            if (QGesture *pan = gestureEvent->gesture(Qt::PanGesture)) {
                handlePanGesture(dynamic_cast<QPanGesture *>(pan));
                return true;
            }
        }

        return QWidget::event(event);
    }

    void Chart::keyPressEvent(QKeyEvent *event) {
        Qt::KeyboardModifiers modifiers = event->modifiers();
        int modified = modifiers & (Qt::ControlModifier | Qt::AltModifier);
        int panAmount = modified ? 10 : 1;

        if (event->type() == QEvent::KeyPress) {
            switch (event->key()) {
                case Qt::Key_Right:
                    pan(panAmount);
                    event->accept();
                    break;
                case Qt::Key_Left:
                    pan(-panAmount);
                    event->accept();
                    break;
                default:
                    QWidget::keyPressEvent(event);
                    break;
            }
        }
    }

    void Chart::handlePinchGesture(QPinchGesture *gesture) {
        if (!gesture) return;

        QPointF centerPoint = gesture->centerPoint();
        int anchorIndex = m_viewport.pixelToIndex(centerPoint.x());

        if (gesture->state() == Qt::GestureStarted) {
            m_initialVisibleCount = m_viewport.visibleCount();
        } else if (gesture->state() == Qt::GestureUpdated) {
            qreal scaleFactor = gesture->totalScaleFactor();

            int targetVisibleCount = static_cast<int>(m_initialVisibleCount / scaleFactor);
            int currentVisibleCount = m_viewport.visibleCount();
            int indexDelta = currentVisibleCount - targetVisibleCount;

            if (indexDelta != 0) {
                zoom(indexDelta, anchorIndex);
                scheduleUpdate();
            }
        }
    }

    void Chart::handlePanGesture(QPanGesture *gesture) {
        if (!gesture) return;

        if (gesture->state() == Qt::GestureUpdated) {
            QPointF delta = gesture->delta();

            double pixelWidth = m_viewport.pixelRect().width();
            int visibleCount = m_viewport.visibleCount();

            if (pixelWidth > 0) {
                int indexDelta = static_cast<int>(-delta.x() * visibleCount / pixelWidth);

                if (indexDelta != 0) {
                    pan(indexDelta);
                    scheduleUpdate();
                }
            }
        }
    }

    void Chart::setupUpdateTimer() {
        m_updateTimer.setInterval(UPDATE_INTERVAL_MS);
        connect(&m_updateTimer, &QTimer::timeout, this, &Chart::onUpdateTimerTick);
        m_updateTimer.start();
    }

    void Chart::onUpdateTimerTick() {
        if (m_pendingUpdate) {
            QWidget::update();
            m_pendingUpdate = false;
        }
    }

    void Chart::scheduleUpdate() {
        m_pendingUpdate = true;
    }
} // namespace QTradingView
