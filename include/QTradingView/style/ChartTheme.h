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

#ifndef QTRADINGVIEW_CHARTTHEME_H
#define QTRADINGVIEW_CHARTTHEME_H

#include <QColor>
#include <QFont>
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

/**
 * @brief Defines the color and font theme for chart rendering.
 *
 * ChartTheme provides color and font settings for chart backgrounds, grids, axes, crosshairs, and series.
 */
class QTRADINGVIEW_EXPORT ChartTheme
{
public:
    /**
     * @brief Constructs a ChartTheme with default settings.
     */
    ChartTheme();

    // Background colors
    QColor backgroundColor;
    QColor chartBackgroundColor;

    // Grid colors
    QColor gridColor;
    QColor borderColor;
    QColor paneBorderColor;  // Border color for pane separation

    // Axis colors
    QColor axisTextColor;
    QColor axisBackgroundColor;

    // Crosshair colors
    QColor crosshairColor;

    // Text colors
    QColor textColor;
    QFont textFont;

    // Default series color
    QColor defaultSeriesColor;

    // Static factory methods for predefined themes
    /**
     * @brief Returns a predefined dark theme similar to TradingView.
     * @return ChartTheme instance with dark colors.
     */
    static ChartTheme tradingViewDark();
    /**
     * @brief Returns a predefined light theme similar to TradingView.
     * @return ChartTheme instance with light colors.
     */
    static ChartTheme tradingViewLight();
    /**
     * @brief Returns a generic dark theme.
     * @return ChartTheme instance with dark colors.
     */
    static ChartTheme dark();
    /**
     * @brief Returns a generic light theme.
     * @return ChartTheme instance with light colors.
     */
    static ChartTheme light();
};

} // namespace QTradingView

#endif // QTRADINGVIEW_CHARTTHEME_H
