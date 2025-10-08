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

#include "QTradingView/style/ChartTheme.h"

namespace QTradingView {

ChartTheme::ChartTheme()
    : backgroundColor(QColor(22, 26, 30))           // TradingView dark background
    , chartBackgroundColor(QColor(19, 23, 34))      // TradingView dark chart area
    , gridColor(QColor(42, 46, 57))                 // TradingView dark grid
    , borderColor(QColor(42, 46, 57))               // TradingView dark border
    , paneBorderColor(QColor(56, 61, 74))           // Lighter border for pane separation
    , axisTextColor(QColor(131, 137, 149))          // TradingView dark axis text
    , axisBackgroundColor(QColor(19, 23, 34))       // TradingView dark axis bg
    , crosshairColor(QColor(120, 123, 134))         // TradingView dark crosshair
    , textColor(QColor(209, 212, 220))              // TradingView dark text
    , defaultSeriesColor(QColor(41, 98, 255))       // TradingView blue
{
    textFont.setFamily("Arial");
    textFont.setPixelSize(11);
}

ChartTheme ChartTheme::tradingViewDark()
{
    ChartTheme theme;
    theme.backgroundColor = QColor(22, 26, 30);           // #161a1e
    theme.chartBackgroundColor = QColor(19, 23, 34);      // #131722
    theme.gridColor = QColor(42, 46, 57);                 // #2a2e39
    theme.borderColor = QColor(42, 46, 57);               // #2a2e39
    theme.paneBorderColor = QColor(56, 61, 74);           // Lighter for pane separation
    theme.axisTextColor = QColor(131, 137, 149);          // #838995
    theme.axisBackgroundColor = QColor(19, 23, 34);       // #131722
    theme.crosshairColor = QColor(120, 123, 134);         // #787b86
    theme.textColor = QColor(209, 212, 220);              // #d1d4dc
    theme.defaultSeriesColor = QColor(41, 98, 255);       // #2962ff

    theme.textFont.setFamily("Arial");
    theme.textFont.setPixelSize(11);

    return theme;
}

ChartTheme ChartTheme::tradingViewLight()
{
    ChartTheme theme;
    theme.backgroundColor = QColor(255, 255, 255);        // #ffffff
    theme.chartBackgroundColor = QColor(255, 255, 255);   // #ffffff
    theme.gridColor = QColor(233, 236, 239);              // #e9ecef
    theme.borderColor = QColor(233, 236, 239);            // #e9ecef
    theme.paneBorderColor = QColor(200, 203, 208);        // Darker for pane separation
    theme.axisTextColor = QColor(131, 137, 149);          // #838995
    theme.axisBackgroundColor = QColor(255, 255, 255);    // #ffffff
    theme.crosshairColor = QColor(120, 123, 134);         // #787b86
    theme.textColor = QColor(19, 23, 34);                 // #131722
    theme.defaultSeriesColor = QColor(41, 98, 255);       // #2962ff

    theme.textFont.setFamily("Arial");
    theme.textFont.setPixelSize(11);

    return theme;
}

ChartTheme ChartTheme::dark()
{
    ChartTheme theme;
    theme.backgroundColor = QColor(30, 30, 30);
    theme.chartBackgroundColor = QColor(20, 20, 20);
    theme.gridColor = QColor(60, 60, 60);
    theme.borderColor = QColor(60, 60, 60);
    theme.paneBorderColor = QColor(80, 80, 80);
    theme.axisTextColor = QColor(180, 180, 180);
    theme.axisBackgroundColor = QColor(20, 20, 20);
    theme.crosshairColor = QColor(150, 150, 150);
    theme.textColor = QColor(220, 220, 220);
    theme.defaultSeriesColor = QColor(0, 150, 255);

    theme.textFont.setFamily("Arial");
    theme.textFont.setPixelSize(11);

    return theme;
}

ChartTheme ChartTheme::light()
{
    ChartTheme theme;
    theme.backgroundColor = QColor(245, 245, 245);
    theme.chartBackgroundColor = QColor(255, 255, 255);
    theme.gridColor = QColor(220, 220, 220);
    theme.borderColor = QColor(200, 200, 200);
    theme.paneBorderColor = QColor(180, 180, 180);
    theme.axisTextColor = QColor(100, 100, 100);
    theme.axisBackgroundColor = QColor(255, 255, 255);
    theme.crosshairColor = QColor(120, 120, 120);
    theme.textColor = QColor(50, 50, 50);
    theme.defaultSeriesColor = QColor(0, 120, 255);

    theme.textFont.setFamily("Arial");
    theme.textFont.setPixelSize(11);

    return theme;
}

} // namespace QTradingView
