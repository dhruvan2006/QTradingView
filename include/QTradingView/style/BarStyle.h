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

#ifndef QTRADINGVIEW_BARSTYLE_H
#define QTRADINGVIEW_BARSTYLE_H

#include <QColor>
#include "QTradingView/style/SeriesStyle.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT BarStyle : public SeriesStyle
{
public:
    // Bar appearance
    QColor color = QColor(100, 150, 200);
    QColor borderColor = QColor(70, 120, 170);

    // Bar sizing
    double barWidthRatio = 0.8;
    double maxBarWidthPx = 50.0;
    double minBarWidthPx = 1.0;

    // Bar rendering
    bool drawBorder = false;
    double borderWidth = 1.0;

    // Coloring based on value
    bool usePositiveNegativeColors = true;
    QColor positiveColor = QColor(8, 153, 129);
    QColor negativeColor = QColor(242, 54, 69);
};

} // namespace QTradingView

#endif // QTRADINGVIEW_BARSTYLE_H
