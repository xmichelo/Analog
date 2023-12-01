/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of report dialog class.


#include "ReportDialog.h"


//****************************************************************************************************************************************************
/// \param[in] parent The parent widget of the dialog.
/// \param[in] report The report.
//****************************************************************************************************************************************************
ReportDialog::ReportDialog(QWidget *parent, Report const &report)
    : QDialog(parent),
      report_(report) {
    ui_.setupUi(this);
    ui_.edit->setPlainText(report_.toString());
    connect(ui_.buttonClose, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui_.buttonSave, &QPushButton::clicked, this, &ReportDialog::onSave);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void ReportDialog::onSave() {
    QMessageBox::critical(this, "Error", "Not yet implemented.");
}
