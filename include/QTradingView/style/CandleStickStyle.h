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

#ifndef QTRADINGVIEW_CANDLESTICKSTYLE_H
#define QTRADINGVIEW_CANDLESTICKSTYLE_H

#include <QColor>
#include "QTradingView/style/SeriesStyle.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class CandleStickStyle : public SeriesStyle
{
public:
    // Body
    QColor bullishColor = QColor(8, 153, 129);
    QColor bearishColor = QColor(242, 54, 69);
    QColor borderColor = QColor(30, 30, 30);

    // Wick
    int wickWidth = 1;

    // Body sizing
    double bodyWidthRatio = 0.6;
    double maxBodyWidthPx = 20.0;
    double minBodyHeight = 1.0;

    // Body rendering
    bool drawBody = true;
    bool hollowCandles = false;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_CANDLESTICKSTYLE_H