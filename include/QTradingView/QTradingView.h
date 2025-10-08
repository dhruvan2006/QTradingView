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
        XAxisZoom
    };
    DragMode m_dragMode;
    Pane* m_dragPane;  // The pane being zoomed on Y-axis
    double m_dragStartValue;  // Starting price value for Y-axis zoom
};

} // namespace QTradingView

#endif // QTRADINGVIEW_H