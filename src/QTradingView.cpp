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

#include "QTradingView/QTradingView.h"
#include "QTradingView/Chart.h"
#include <QPainter>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QGestureEvent>
#include <QPinchGesture>
#include <QPanGesture>

namespace QTradingView {

QTradingView::QTradingView(QWidget* parent)
    : QWidget(parent)
    , m_chart(std::make_unique<Chart>())
    , m_isPanning(false)
    , m_lastMouseIndex(0)
    , m_initialVisibleCount(0)
    , m_dragMode(DragMode::None)
    , m_dragPane(nullptr)
    , m_dragStartValue(0.0)
    , m_resizingBorderIndex(-1)
    , m_minPaneHeight(50.0) {
    setMinimumSize(400, 300);

    // Enable mouse tracking for smooth interactions
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    // Enable touch and gesture events
    setAttribute(Qt::WA_AcceptTouchEvents, true);
    grabGesture(Qt::PinchGesture);
    grabGesture(Qt::PanGesture);
}

QTradingView::~QTradingView() = default;

Chart* QTradingView::chart() {
    return m_chart.get();
}

void QTradingView::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    m_chart->render(&painter);
}

void QTradingView::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    m_chart->setSize(width(), height());
}

void QTradingView::wheelEvent(QWheelEvent* event) {
    // Get the mouse position in chart coordinates
    int mouseX = event->position().x();

    // Convert pixel position to data index (anchor point for zoom)
    int anchorIndex = m_chart->viewport().pixelToIndex(mouseX);

    // Calculate zoom amount from wheel delta with adaptive sensitivity
    // Positive delta = zoom in (show less data), negative = zoom out (show more data)
    int zoomDelta = event->angleDelta().y() / 120; // Standard wheel step is 120

    // Make zoom speed proportional to visible data count
    int visibleCount = m_chart->viewport().visibleCount();
    double zoomSensitivity = std::max(2.0, visibleCount * 0.05); // Scale with visible count, minimum 2
    int indexDelta = static_cast<int>(zoomDelta * zoomSensitivity);

    // Apply zoom centered on mouse position
    m_chart->zoom(indexDelta, anchorIndex);

    // Redraw the chart
    update();

    event->accept();
}

void QTradingView::mousePressEvent(QMouseEvent* event) {
    if (!hasFocus()) {
        setFocus();
    }

    if (event->button() == Qt::LeftButton) {
        QPointF pos = event->pos();

        // Check if clicking on a pane border for resizing
        int borderIndex = m_chart->paneBorderAtPosition(pos);
        if (borderIndex >= 0) {
            m_dragMode = DragMode::PaneResize;
            m_resizingBorderIndex = borderIndex;
            m_lastMousePos = event->pos();
            setCursor(Qt::SplitVCursor);
            event->accept();
            return;
        }

        // Check if clicking on Y-axis (left or right)
        if (m_chart->leftAxisRect().contains(pos) || m_chart->rightAxisRect().contains(pos)) {
            // Find which pane's Y-axis was clicked
            for (const auto& pane : m_chart->panes()) {
                QRectF paneRect = pane->rect();
                if (pos.y() >= paneRect.top() && pos.y() <= paneRect.bottom()) {
                    m_dragMode = DragMode::YAxisZoom;
                    m_dragPane = pane.get();
                    m_lastMousePos = event->pos();

                    // Calculate the price value at the clicked position
                    double normalizedPos = (paneRect.bottom() - pos.y()) / paneRect.height();
                    m_dragStartValue = pane->minValue() + normalizedPos * (pane->maxValue() - pane->minValue());

                    setCursor(Qt::SizeVerCursor);
                    event->accept();
                    return;
                }
            }
        }
        // Check if clicking on X-axis
        else if (m_chart->xAxisRect().contains(pos)) {
            m_dragMode = DragMode::XAxisZoom;
            m_lastMousePos = event->pos();
            m_lastMouseIndex = m_chart->viewport().pixelToIndex(event->pos().x());
            setCursor(Qt::SizeHorCursor);
            event->accept();
            return;
        }
        // Otherwise, it's chart panning
        else {
            m_dragMode = DragMode::ChartPan;
            m_isPanning = true;
            m_lastMousePos = event->pos();
            m_lastMouseIndex = m_chart->viewport().pixelToIndex(event->pos().x());
            setCursor(Qt::ClosedHandCursor);
            event->accept();
        }
    }
}

void QTradingView::mouseMoveEvent(QMouseEvent* event) {
    if (m_dragMode == DragMode::PaneResize && m_resizingBorderIndex >= 0) {
        // Handle pane border dragging to resize panes
        int deltaY = event->pos().y() - m_lastMousePos.y();

        const auto& panes = m_chart->panes();
        if (m_resizingBorderIndex < static_cast<int>(panes.size()) - 1) {
            auto& upperPane = panes[m_resizingBorderIndex];
            auto& lowerPane = panes[m_resizingBorderIndex + 1];

            double upperRatio = upperPane->heightRatio();
            double lowerRatio = lowerPane->heightRatio();

            // Calculate total height available for these two panes
            double totalRatio = upperRatio + lowerRatio;
            double totalHeight = upperPane->rect().height() + lowerPane->rect().height();

            // Calculate new heights
            double newUpperHeight = upperPane->rect().height() + deltaY;
            double newLowerHeight = lowerPane->rect().height() - deltaY;

            // Enforce minimum height constraints
            if (newUpperHeight < m_minPaneHeight) {
                newUpperHeight = m_minPaneHeight;
                newLowerHeight = totalHeight - newUpperHeight;
            }
            if (newLowerHeight < m_minPaneHeight) {
                newLowerHeight = m_minPaneHeight;
                newUpperHeight = totalHeight - newLowerHeight;
            }

            // Convert heights back to ratios
            double newUpperRatio = (newUpperHeight / totalHeight) * totalRatio;
            double newLowerRatio = (newLowerHeight / totalHeight) * totalRatio;

            // Update the pane ratios
            upperPane->setHeightRatio(newUpperRatio);
            lowerPane->setHeightRatio(newLowerRatio);

            // Recalculate layout
            m_chart->calculateLayout();

            m_lastMousePos = event->pos();
            update();
            event->accept();
        }
    }
    else if (m_dragMode == DragMode::YAxisZoom && m_dragPane) {
        // Vertical drag on Y-axis zooms the price scale
        int deltaY = event->pos().y() - m_lastMousePos.y();

        // Calculate zoom factor based on vertical movement
        // Dragging down = zoom out (show more price range)
        // Dragging up = zoom in (show less price range)
        double zoomFactor = 1.0 + (deltaY * 0.01); // 1% per pixel

        // Apply zoom centered on the anchor value
        m_dragPane->zoomYAxis(zoomFactor, m_dragStartValue);

        m_lastMousePos = event->pos();
        update();
        event->accept();
    }
    else if (m_dragMode == DragMode::XAxisZoom) {
        // Horizontal drag on X-axis zooms the time scale
        int deltaX = event->pos().x() - m_lastMousePos.x();

        // Calculate zoom based on horizontal movement
        // Dragging right = zoom out (show more bars)
        // Dragging left = zoom in (show fewer bars)
        int zoomDelta = static_cast<int>(-deltaX * 0.5);

        if (zoomDelta != 0) {
            m_chart->zoom(zoomDelta, m_lastMouseIndex);
        }

        m_lastMousePos = event->pos();
        update();
        event->accept();
    }
    else if (m_dragMode == DragMode::ChartPan && m_isPanning) {
        // Calculate how many data points we've moved horizontally
        int currentIndex = m_chart->viewport().pixelToIndex(event->pos().x());
        int indexDelta = m_lastMouseIndex - currentIndex;

        if (indexDelta != 0) {
            m_chart->pan(indexDelta);
            m_lastMouseIndex = m_chart->viewport().pixelToIndex(event->pos().x());
        }

        // Handle vertical panning (price scale movement)
        int deltaY = event->pos().y() - m_lastMousePos.y();
        if (deltaY != 0) {
            // Find which pane we're in
            for (const auto& pane : m_chart->panes()) {
                if (pane->rect().contains(event->pos())) {
                    // Only allow vertical panning if auto-range is disabled
                    if (!pane->isAutoRange()) {
                        // Calculate the price shift based on vertical movement
                        double priceRange = pane->maxValue() - pane->minValue();
                        double priceShift = (deltaY / pane->rect().height()) * priceRange;

                        // Shift the price range
                        pane->setManualRange(pane->minValue() + priceShift,
                                            pane->maxValue() + priceShift);
                    }
                    break;
                }
            }
        }

        m_lastMousePos = event->pos();

        // Update crosshair position even while panning
        if (m_chart->isCrosshairVisible())
            m_chart->setCrosshairPosition(event->pos());

        update();
        event->accept();
    } else {
        // Not dragging - check if hovering over special areas and update cursor
        QPointF pos = event->pos();

        // Check if hovering over a pane border (for potential resizing)
        int borderIndex = m_chart->paneBorderAtPosition(pos);
        if (borderIndex >= 0) {
            // Mouse is near a pane border - show resize cursor
            m_chart->setCrosshairVisible(false);
            setCursor(Qt::SplitVCursor);
            update();
        }
        // Check if hovering over Y-axis
        else if (m_chart->leftAxisRect().contains(pos) || m_chart->rightAxisRect().contains(pos)) {
            m_chart->setCrosshairVisible(false);
            setCursor(Qt::SizeVerCursor);
            update();
        }
        // Check if hovering over X-axis
        else if (m_chart->xAxisRect().contains(pos)) {
            m_chart->setCrosshairVisible(false);
            setCursor(Qt::SizeHorCursor);
            update();
        }
        // Default - arrow cursor and show crosshair
        else {
            setCursor(Qt::ArrowCursor);

            // Update crosshair position when not dragging
            m_chart->setCrosshairVisible(true);
            m_chart->setCrosshairPosition(event->pos());
            update();
        }
    }
}

void QTradingView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_dragMode = DragMode::None;
        m_isPanning = false;
        m_dragPane = nullptr;
        setCursor(Qt::ArrowCursor);
        event->accept();
    }
}

void QTradingView::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPointF pos = event->pos();

        // Check if double-clicking on Y-axis (left or right)
        if (m_chart->leftAxisRect().contains(pos) || m_chart->rightAxisRect().contains(pos)) {
            // Find which pane's Y-axis was double-clicked and reset to auto-range
            for (const auto& pane : m_chart->panes()) {
                QRectF paneRect = pane->rect();
                if (pos.y() >= paneRect.top() && pos.y() <= paneRect.bottom()) {
                    pane->resetAutoRange();
                    update();
                    event->accept();
                    return;
                }
            }
        }
        // Check if double-clicking on X-axis
        else if (m_chart->xAxisRect().contains(pos)) {
            // Reset X-axis to show all data
            m_chart->fitToData();
            update();
            event->accept();
            return;
        }
    }
}

void QTradingView::leaveEvent(QEvent* event) {
    // Hide crosshair when mouse leaves the widget
    m_chart->setCrosshairVisible(false);
    update();
    QWidget::leaveEvent(event);
}

bool QTradingView::event(QEvent* event) {
    if (event->type() == QEvent::Gesture) {
        QGestureEvent* gestureEvent = dynamic_cast<QGestureEvent*>(event);

        if (QGesture* pinch = gestureEvent->gesture(Qt::PinchGesture)) {
            handlePinchGesture(dynamic_cast<QPinchGesture*>(pinch));
            return true;
        }

        if (QGesture* pan = gestureEvent->gesture(Qt::PanGesture)) {
            handlePanGesture(dynamic_cast<QPanGesture*>(pan));
            return true;
        }
    }

    return QWidget::event(event);
}

void QTradingView::keyPressEvent(QKeyEvent *event) {
    Qt::KeyboardModifiers modifiers = event->modifiers();
    int modified = modifiers & (Qt::ControlModifier | Qt::AltModifier);
    int panAmount = modified ? 10 : 1;

    if (event->type() == QEvent::KeyPress) {
        switch (event->key()) {
            case Qt::Key_Right:
                m_chart->pan(panAmount);
                event->accept();
                break;
            case Qt::Key_Left:
                m_chart->pan(-panAmount);
                event->accept();
                break;
            default:
                QWidget::keyPressEvent(event);
                break;
        }
    }
}

void QTradingView::handlePinchGesture(QPinchGesture* gesture) {
    if (!gesture) return;

    // Get the center point of the pinch gesture
    QPointF centerPoint = gesture->centerPoint();
    int anchorIndex = m_chart->viewport().pixelToIndex(centerPoint.x());

    if (gesture->state() == Qt::GestureStarted) {
        // Store the initial visible count on gesture start
        m_initialVisibleCount = m_chart->viewport().visibleCount();
    }
    else if (gesture->state() == Qt::GestureUpdated) {
        // Use totalScaleFactor for cumulative scaling from gesture start
        qreal scaleFactor = gesture->totalScaleFactor();

        // Calculate target visible count based on total scale factor
        int targetVisibleCount = static_cast<int>(m_initialVisibleCount / scaleFactor);
        int currentVisibleCount = m_chart->viewport().visibleCount();
        int indexDelta = currentVisibleCount - targetVisibleCount;

        if (indexDelta != 0) {
            m_chart->zoom(indexDelta, anchorIndex);
            update();
        }
    }
}

void QTradingView::handlePanGesture(QPanGesture* gesture) {
    if (!gesture) return;

    if (gesture->state() == Qt::GestureUpdated) {
        // Get the delta movement
        QPointF delta = gesture->delta();

        // Convert pixel movement to index movement
        double pixelWidth = m_chart->viewport().pixelRect().width();
        int visibleCount = m_chart->viewport().visibleCount();

        if (pixelWidth > 0) {
            int indexDelta = static_cast<int>(-delta.x() * visibleCount / pixelWidth);

            if (indexDelta != 0) {
                m_chart->pan(indexDelta);
                update();
            }
        }
    }
}

} // namespace QTradingView
