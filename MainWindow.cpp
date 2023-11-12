/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of main window class.


#include "MainWindow.h"
#include "Exception.h"


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
MainWindow::MainWindow()
    : QMainWindow()
    , log_()
    , filter_(log_) {
    ui_.setupUi(this);
    ui_.tableView->setModel(&filter_);

    connect(ui_.actionOpenFile, &QAction::triggered, this, &MainWindow::onActionOpenFile);
    connect(&log_, &Log::modelReset, ui_.tableView, &QTableView::resizeColumnsToContents);
    connect(ui_.comboLevel, &QComboBox::currentIndexChanged, this, &MainWindow::onLevelComboChanged);
    connect(ui_.checkAndAbove, &QCheckBox::stateChanged, this, &MainWindow::onLevelStrictnessChanged);
    ui_.comboLevel->setCurrentIndex(static_cast<int>(filter_.level()));
    ui_.checkAndAbove->setChecked(!filter_.useStrictLevelFilter());
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::onActionOpenFile() {
    QString const filePath = QFileDialog::getOpenFileName(this, tr("Select log file"), QString(), tr("Log files (*.log);;All files (*.*)"));
    if (!filePath.isEmpty()) {
        this->openFile(filePath);
    }
}


//****************************************************************************************************************************************************
/// \param[in]
//****************************************************************************************************************************************************
void MainWindow::onLevelComboChanged(int index) {
    filter_.setLevel(static_cast<LogEntry::Level>(index));
}


//****************************************************************************************************************************************************
/// \param[in] nonStrict The strictness of the level filter.
//****************************************************************************************************************************************************
void MainWindow::onLevelStrictnessChanged(bool nonStrict) {
    filter_.setUseStrictLevelFilter(!nonStrict);
}


//****************************************************************************************************************************************************
/// \param[in] filePath The path of the file to open.
//****************************************************************************************************************************************************
void MainWindow::openFile(QString const &filePath) {
    try {
        log_.open(filePath);
    } catch (Exception const &e) {
        QMessageBox::critical(this, tr("Error"), e.message());
    }
}
