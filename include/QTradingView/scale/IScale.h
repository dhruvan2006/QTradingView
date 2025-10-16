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

#ifndef QTRADINGVIEW_ISCALE_H
#define QTRADINGVIEW_ISCALE_H

#include <vector>
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

/**
 * @brief Abstract base class for value-to-pixel scaling in QTradingView.
 *
 * IScale provides an interface for converting between data values and pixel coordinates, managing scale domains and ranges, and generating axis ticks.
 */
class QTRADINGVIEW_EXPORT IScale
{
public:
    /**
     * @brief Virtual destructor for IScale.
     */
    virtual ~IScale() = default;

    /**
     * @brief Converts a data value to its corresponding pixel position.
     * @param value Data value.
     * @return Pixel position.
     */
    [[nodiscard]] virtual double dataToPixel(double value) const = 0;

    /**
     * @brief Converts a pixel position to its corresponding data value.
     * @param pixel Pixel position.
     * @return Data value.
     */
    [[nodiscard]] virtual double pixelToData(double pixel) const = 0;

    /**
     * @brief Sets the domain (min and max data values) for the scale.
     * @param minValue Minimum data value.
     * @param maxValue Maximum data value.
     */
    virtual void setDomain(double minValue, double maxValue) = 0;

    /**
     * @brief Sets the pixel range for the scale.
     * @param minPixel Minimum pixel value.
     * @param maxPixel Maximum pixel value.
     */
    virtual void setRange(double minPixel, double maxPixel) = 0;

    /**
     * @brief Returns a vector of tick values for the axis.
     * @param approxCount Approximate number of ticks desired.
     * @return Vector of tick values.
     */
    [[nodiscard]] virtual std::vector<double> getTicks(int approxCount) const = 0;

    /**
     * @brief Returns a default vector of tick values (approx. 5).
     * @return Vector of tick values.
     */
    [[nodiscard]] std::vector<double> getTicks() const {
        return getTicks(5);
    }

    /**
     * @brief Applies padding to the scale domain.
     * @param minValue Minimum data value (modified).
     * @param maxValue Maximum data value (modified).
     * @param paddingRatio Ratio of padding to apply.
     */
    virtual void applyPadding(double &minValue, double &maxValue, double paddingRatio) const = 0;

    /**
     * @brief Zooms the scale domain around an anchor value.
     * @param minValue Minimum data value (modified).
     * @param maxValue Maximum data value (modified).
     * @param zoomFactor Zoom factor to apply.
     * @param anchorValue Value to anchor the zoom operation.
     */
    virtual void zoomDomain(double &minValue, double &maxValue, double zoomFactor, double anchorValue) const = 0;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_ISCALE_H
