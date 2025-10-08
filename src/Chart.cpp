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
#include "QTradingView/data/IDataProvider.h"
#include "QTradingView/style/ChartTheme.h"
#include <QPainter>
#include <algorithm>

namespace QTradingView {

    Chart::Chart()
        : m_width(800), m_height(600)
        , m_leftAxisWidth(60), m_rightAxisWidth(60), m_xAxisHeight(30)
        , m_theme(ChartTheme::tradingViewDark())
        , m_crosshairVisible(false) {
        // Initialize axis renderer with theme colors
        m_axisRenderer.setTextColor(m_theme.axisTextColor);
        m_axisRenderer.setGridColor(m_theme.gridColor);
        m_axisRenderer.setBorderColor(m_theme.borderColor);
        m_axisRenderer.setBackgroundColor(m_theme.axisBackgroundColor);

        // Initialize grid renderer
        m_gridRenderer.setGridColor(m_theme.gridColor);

        // Initialize crosshair renderer
        m_crosshairRenderer.setCrosshairColor(m_theme.crosshairColor);
        m_crosshairRenderer.setLabelBackgroundColor(m_theme.axisBackgroundColor);
        m_crosshairRenderer.setLabelTextColor(m_theme.axisTextColor);
        m_crosshairRenderer.setFont(m_theme.textFont);
    }

    Pane *Chart::addPane(double heightRatio) {
        auto pane = std::make_shared<Pane>();
        pane->setHeightRatio(heightRatio);
        m_panes.push_back(pane);
        return pane.get();
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

    const std::vector<std::shared_ptr<Pane> > &Chart::panes() const {
        return m_panes;
    }

    void Chart::setDataProvider(std::shared_ptr<IDataProvider> provider) {
        m_dataProvider = std::move(provider);
        if (m_dataProvider) {
            fitToData();
        }
    }

    std::shared_ptr<IDataProvider> Chart::dataProvider() const {
        return m_dataProvider;
    }

    ViewPort &Chart::viewport() {
        return m_viewport;
    }

    const ViewPort &Chart::viewport() const {
        return m_viewport;
    }

    void Chart::setSize(int width, int height) {
        m_width = width;
        m_height = height;
        calculateLayout();
    }

    int Chart::width() const {
        return m_width;
    }

    int Chart::height() const {
        return m_height;
    }

    void Chart::calculateLayout() {
        if (m_panes.empty()) return;

        double totalRatio = 0.0;
        for (const auto &pane: m_panes) {
            totalRatio += pane->heightRatio();
        }

        double chartWidth = m_width - m_leftAxisWidth - m_rightAxisWidth;
        double chartHeight = m_height - m_xAxisHeight;
        double currentY = 0.0;

        for (const auto &pane: m_panes) {
            double paneHeight = (pane->heightRatio() / totalRatio) * chartHeight;
            QRectF rect(m_leftAxisWidth, currentY, chartWidth, paneHeight);
            pane->setRect(rect);
            currentY += paneHeight;
        }

        m_viewport.setPixelRect(QRectF(m_leftAxisWidth, 0, chartWidth, chartHeight));
    }

    void Chart::setTheme(const ChartTheme& theme) {
        m_theme = theme;
        m_axisRenderer.setTextColor(m_theme.axisTextColor);
        m_axisRenderer.setGridColor(m_theme.gridColor);
        m_axisRenderer.setBorderColor(m_theme.borderColor);
        m_axisRenderer.setBackgroundColor(m_theme.axisBackgroundColor);

        m_gridRenderer.setGridColor(m_theme.gridColor);

        m_crosshairRenderer.setCrosshairColor(m_theme.crosshairColor);
        m_crosshairRenderer.setLabelBackgroundColor(m_theme.axisBackgroundColor);
        m_crosshairRenderer.setLabelTextColor(m_theme.axisTextColor);
        m_crosshairRenderer.setFont(m_theme.textFont);
    }

    const ChartTheme& Chart::theme() const {
        return m_theme;
    }

    void Chart::render(QPainter *painter) {
        if (!painter || m_panes.empty()) return;

        // Fill chart background
        painter->fillRect(0, 0, m_width, m_height, m_theme.backgroundColor);

        // Fill chart area background
        QRectF chartArea(m_leftAxisWidth, 0,
                        m_width - m_leftAxisWidth - m_rightAxisWidth,
                        m_height - m_xAxisHeight);
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
            m_gridRenderer.render(painter, pane.get(), m_viewport, m_dataProvider.get(), &m_axisRenderer);

            // Render the pane
            pane->render(painter, m_viewport);

            // Draw Y axes for this pane
            QRectF leftAxisRect(0, pane->rect().top(), m_leftAxisWidth, pane->rect().height());
            QRectF rightAxisRect(m_width - m_rightAxisWidth, pane->rect().top(),
                                m_rightAxisWidth, pane->rect().height());
            m_axisRenderer.drawYAxis(painter, leftAxisRect, rightAxisRect, pane.get());
        }

        // Draw pane borders (after rendering all panes, so borders are on top)
        painter->save();
        QPen borderPen(m_theme.paneBorderColor, 2.5);
        painter->setPen(borderPen);
        painter->setRenderHint(QPainter::Antialiasing, false); // Sharp lines for borders

        for (size_t i = 0; i < m_panes.size(); ++i) {
            const auto& pane = m_panes[i];
            QRectF paneRect = pane->rect();

            // Draw horizontal line at the bottom of each pane except the last one
            if (i < m_panes.size() - 1) {
                painter->drawLine(QPointF(0, paneRect.bottom()),
                                 QPointF(m_width, paneRect.bottom()));
            }
        }
        painter->restore();

        // Draw X axis
        QRectF xAxisRect(0, m_height - m_xAxisHeight,
                         m_width, m_xAxisHeight);
        m_axisRenderer.drawXAxis(painter, xAxisRect, m_viewport, m_dataProvider.get());

        // Render crosshair if visible (draw last, on top of everything)
        if (m_crosshairVisible && !m_panes.empty()) {
            // Find the pane that contains the crosshair
            Pane* activePane = nullptr;
            for (const auto& pane : m_panes) {
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
                double xAxisY = m_height - m_xAxisHeight;
                m_crosshairRenderer.render(painter, m_crosshairPosition, m_viewport,
                                          activePane, m_dataProvider.get(), xAxisY);
            }
        }
    }

    void Chart::pan(int indexDelta) {
        if (!m_dataProvider) return;

        int newStart = m_viewport.startIndex() + indexDelta;
        int newEnd = m_viewport.endIndex() + indexDelta;

        m_viewport.setVisibleRange(newStart, newEnd);
    }

    void Chart::zoom(int indexDelta, int anchorIndex) {
        if (!m_dataProvider) return;

        int visibleCount = m_viewport.visibleCount();

        int newVisibleCount = visibleCount - indexDelta;
        // Prevent zooming to zero or negative
        if (newVisibleCount < 1) {
            newVisibleCount = 1;
        }

        double anchorRatio = static_cast<double>(anchorIndex - m_viewport.startIndex()) / visibleCount;

        int newStart = anchorIndex - static_cast<int>(anchorRatio * newVisibleCount);
        int newEnd = newStart + newVisibleCount - 1;

        m_viewport.setVisibleRange(newStart, newEnd);
    }

    void Chart::fitToData() {
        if (!m_dataProvider) return;

        int count = m_dataProvider->count();
        if (count > 0) {
            m_viewport.setVisibleRange(0, count - 1);
        }
    }

    void Chart::setCrosshairVisible(bool visible) {
        m_crosshairVisible = visible;
    }

    bool Chart::isCrosshairVisible() const {
        return m_crosshairVisible;
    }

    void Chart::setCrosshairPosition(const QPointF& position) {
        m_crosshairPosition = position;
    }

    QPointF Chart::crosshairPosition() const {
        return m_crosshairPosition;
    }

    QRectF Chart::leftAxisRect() const {
        if (m_panes.empty()) return QRectF();
        return QRectF(0, 0, m_leftAxisWidth, m_height - m_xAxisHeight);
    }

    QRectF Chart::rightAxisRect() const {
        if (m_panes.empty()) return QRectF();
        return QRectF(m_width - m_rightAxisWidth, 0, m_rightAxisWidth, m_height - m_xAxisHeight);
    }

    QRectF Chart::xAxisRect() const {
        return QRectF(0, m_height - m_xAxisHeight, m_width, m_xAxisHeight);
    }

    Pane* Chart::paneAtPosition(const QPointF& position) const {
        for (const auto& pane : m_panes) {
            if (pane->rect().contains(position)) {
                return pane.get();
            }
        }
        return nullptr;
    }

    int Chart::paneBorderAtPosition(const QPointF& position, double threshold) const {
        // Returns the index of the border below the pane at the position
        // Returns -1 if no border is near the position
        for (size_t i = 0; i < m_panes.size() - 1; ++i) {
            const auto& pane = m_panes[i];
            double borderY = pane->rect().bottom();

            // Check if position is within threshold distance of the border
            if (std::abs(position.y() - borderY) <= threshold &&
                position.x() >= 0 && position.x() <= m_width) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

} // namespace QTradingView
