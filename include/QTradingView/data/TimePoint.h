#ifndef QTRADINGVIEW_TIMEPOINT_H
#define QTRADINGVIEW_TIMEPOINT_H

#include <QDateTime>
#include "QTradingView/qtradingview_global.h"

namespace QTradingView {

class QTRADINGVIEW_EXPORT TimePoint
{
public:
    TimePoint();
    TimePoint(const QDateTime &timestamp, int index = -1);

    QDateTime timestamp() const;
    void setTimestamp(const QDateTime &timestamp);

    int index() const;
    void setIndex(int index);

private:
    QDateTime m_timestamp;
    int m_index;
};

} // namespace QTradingView

#endif // QTRADINGVIEW_TIMEPOINT_H