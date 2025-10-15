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

#ifndef QTRADINGVIEW_H
#define QTRADINGVIEW_H

// Global exports
#include <QTradingView/qtradingview_global.h>

// Core classes
#include <QTradingView/Chart.h>
#include <QTradingView/ViewPort.h>
#include <QTradingView/Pane.h>

// Renderers
#include <QTradingView/renderer/AxisRenderer.h>
#include <QTradingView/renderer/GridRenderer.h>
#include <QTradingView/renderer/CrosshairRenderer.h>

// Scale system
#include <QTradingView/scale/IScale.h>
#include <QTradingView/scale/LinearScale.h>
#include <QTradingView/scale/LogScale.h>
#include <QTradingView/scale/ScaleType.h>

// Series types
#include <QTradingView/series/Series.h>
#include <QTradingView/series/LineSeries.h>
#include <QTradingView/series/CandleStickSeries.h>
#include <QTradingView/series/BarSeries.h>

// Styling
#include <QTradingView/style/ChartTheme.h>

#endif // QTRADINGVIEW_H