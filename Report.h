/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of report structure.


#ifndef ANALOG_REPORT_H
#define ANALOG_REPORT_H


//****************************************************************************************************************************************************
/// \brief Report structure.
//****************************************************************************************************************************************************
struct Report {
    QDateTime startDate;
    QDateTime endDate;

    QString toString() const;
};


#endif //ANALOG_REPORT_H
