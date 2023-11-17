/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of report dialog class.


#ifndef ANALOG_REPORT_DIALOG_H
#define ANALOG_REPORT_DIALOG_H


#include "ui_ReportDialog.h"
#include "Report.h"

//****************************************************************************************************************************************************
/// \brief Report window class.
//****************************************************************************************************************************************************
class ReportDialog : public QDialog {
Q_OBJECT

public: // member functions.
    ReportDialog(QWidget *parent, Report const &report); ///< Default constructor.
    ReportDialog(ReportDialog const &) = delete; ///< Disabled copy-constructor.
    ReportDialog(ReportDialog &&) = delete; ///< Disabled assignment copy-constructor.
    ~ReportDialog() override = default; ///< Destructor.
    ReportDialog &operator=(ReportDialog const &) = delete; ///< Disabled assignment operator.
    ReportDialog &operator=(ReportDialog &&) = delete; ///< Disabled move assignment operator.

public slots:
    void onSave(); ///< Slot for the 'Save' button.

private: // data members
    Ui::ReportDialog ui_ {}; ///< The UI for the dialog.
    Report const report_; ///< The report.
};


#endif //ANALOG_REPORT_DIALOG_H
