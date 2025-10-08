#ifndef QTRADINGVIEW_CHARTTHEME_H
#define QTRADINGVIEW_CHARTTHEME_H

#include <QColor>
#include <QFont>
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT ChartTheme
{
public:
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
    static ChartTheme tradingViewDark();
    static ChartTheme tradingViewLight();
    static ChartTheme dark();
    static ChartTheme light();
};

} // namespace QTradingView

#endif // QTRADINGVIEW_CHARTTHEME_H
