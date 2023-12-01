/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of report class.


#include "Report.h"


namespace {
QString const dateFormat("yyyy-MM-dd");
QString const timeFormat("hh:mm:ss.zzz");
QString const dateTimeFormat(dateFormat + " " + timeFormat);
qint64 constexpr secondsPerMinute = 60;
qint64 constexpr secondsPerHour = secondsPerMinute * 60;
qint64 constexpr secondsPerDay = secondsPerHour * 24;
}


//****************************************************************************************************************************************************
/// \param[in] durationSecs The duration in seconds.
/// \return A string describing the duration.
//****************************************************************************************************************************************************
QString durationToString(qint64 durationSecs) {
    if (durationSecs < 0)
        return "invalid";

    QString result;

    qint64 d = durationSecs;
    qint64 const days = d / secondsPerDay;
    d = d % secondsPerDay;
    if (days > 0) {
        result += QString("%1d ").arg(days);
    }

    qint64 const hours = d / secondsPerHour;
    d = d % secondsPerHour;
    if (hours > 0) {
        result += QString("%1h ").arg(hours);
    }

    qint64 const minutes = d / secondsPerMinute;
    d = d % secondsPerMinute;
    if (minutes > 0) {
        result += QString("%1m ").arg(minutes);
    }

    if (d > 0) {
        result += QString("%1s").arg(d);
    }
    return result.trimmed();
}


//****************************************************************************************************************************************************
/// \return The report as a human readable string
//****************************************************************************************************************************************************
QString Report::toString() const {
    QString result = "## Time\n";
    result += QString("Start: %1\nEnd: %2\nDuration: %3").arg(
        startDate.toString(dateTimeFormat),
        endDate.toString(dateTimeFormat),
        durationToString(startDate.secsTo(endDate))
    );

    return result;
}
