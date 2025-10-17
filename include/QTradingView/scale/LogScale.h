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

#ifndef QTRADINGVIEW_LOGSCALE_H
#define QTRADINGVIEW_LOGSCALE_H

#include <vector>
#include "QTradingView/scale/IScale.h"
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

/**
 * @brief Logarithmic scale implementation for chart axes.
 *
 * Provides conversion between data values and pixel positions using a symmetric logarithmic scale.
 */
class QTRADINGVIEW_EXPORT LogScale : public IScale
{
public:
    /**
     * @brief Constructs a LogScale with default domain and range.
     */
    LogScale();
    /**
     * @brief Constructs a LogScale with specified domain and range.
     * @param minValue Minimum data value.
     * @param maxValue Maximum data value.
     * @param minPixel Minimum pixel position.
     * @param maxPixel Maximum pixel position.
     */
    LogScale(double minValue, double maxValue, double minPixel, double maxPixel);
    /**
     * @brief Destructor.
     */
    ~LogScale() override = default;

    /**
     * @brief Converts a data value to a pixel position.
     * @param value The data value to convert.
     * @return The corresponding pixel position.
     */
    double dataToPixel(double value) const override;
    /**
     * @brief Converts a pixel position to a data value.
     * @param pixel The pixel position to convert.
     * @return The corresponding data value.
     */
    double pixelToData(double pixel) const override;
    /**
     * @brief Sets the data domain for the scale.
     * @param minValue Minimum data value.
     * @param maxValue Maximum data value.
     */
    void setDomain(double minValue, double maxValue) override;
    /**
     * @brief Sets the pixel range for the scale.
     * @param minPixel Minimum pixel position.
     * @param maxPixel Maximum pixel position.
     */
    void setRange(double minPixel, double maxPixel) override;
    /**
     * @brief Returns tick positions for the scale.
     * @param approxCount Approximate number of ticks desired.
     * @return Vector of tick positions in data coordinates.
     */
    std::vector<double> getTicks(int approxCount) const override;
    /**
     * @brief Applies padding to the data domain.
     * @param minValue Reference to minimum value to pad.
     * @param maxValue Reference to maximum value to pad.
     * @param paddingRatio Ratio of padding to apply.
     */
    void applyPadding(double &minValue, double &maxValue, double paddingRatio) const override;
    /**
     * @brief Zooms the data domain by a factor around an anchor value.
     * @param minValue Reference to minimum value to zoom.
     * @param maxValue Reference to maximum value to zoom.
     * @param zoomFactor Zoom factor to apply.
     * @param anchorValue Value to anchor the zoom.
     */
    void zoomDomain(double &minValue, double &maxValue, double zoomFactor, double anchorValue) const override;
    /**
     * @brief Sets the threshold below which the scale behaves linearly.
     * @param threshold The linear threshold value.
     */
    void setLinearThreshold(double threshold);
    /**
     * @brief Gets the current linear threshold value.
     * @return The linear threshold.
     */
    double getLinearThreshold() const;

private:
    double m_minValue;
    double m_maxValue;
    double m_minPixel;
    double m_maxPixel;
    double m_linearThreshold; // Threshold for linear region around zero

    double symlog(double value) const;
    double invsymlog(double logValue) const;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_LOGSCALE_H

