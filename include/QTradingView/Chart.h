#ifndef QTRADINGVIEW_CHART_H
#define QTRADINGVIEW_CHART_H

#include <vector>
#include <memory>
#include "QTradingView/Pane.h"
#include "QTradingView/ViewPort.h"
#include "QTradingView/data/IDataProvider.h"
#include "QTradingView/qtradingview_global.h"
#include "QTradingView/style/ChartTheme.h"
#include "renderer/AxisRenderer.h"
#include "renderer/GridRenderer.h"
#include "renderer/CrosshairRenderer.h"

class QPainter;

namespace QTradingView {

class QTRADINGVIEW_EXPORT Chart
{
public:
    Chart();

    Pane* addPane(double heightRatio = 1.0);
    void removePane(Pane* pane);
    Pane* mainPane();
    const std::vector<std::shared_ptr<Pane>>& panes() const;

    void setDataProvider(std::shared_ptr<IDataProvider> provider);
    std::shared_ptr<IDataProvider> dataProvider() const;

    ViewPort& viewport();
    const ViewPort& viewport() const;

    void setSize(int width, int height);
    int width() const;
    int height() const;

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

private:
    std::vector<std::shared_ptr<Pane>> m_panes;
    std::shared_ptr<IDataProvider> m_dataProvider;
    ViewPort m_viewport;
    int m_width;
    int m_height;
    ChartTheme m_theme;

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
};

} // namespace QTradingView

#endif // QTRADINGVIEW_CHART_H
