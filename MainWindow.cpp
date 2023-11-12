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
    : QMainWindow() {
    ui_.setupUi(this);
    ui_.tableView->setModel(&log_);

    connect(ui_.actionOpenFile, &QAction::triggered, this, &MainWindow::onActionOpenFile);
    connect(&log_, &Log::modelReset, ui_.tableView, &QTableView::resizeColumnsToContents);
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
/// \param[in] filePath The path of the file to open.
//****************************************************************************************************************************************************
void MainWindow::openFile(QString const &filePath) {
    try {
        log_.open(filePath);
    } catch (Exception const &e) {
        QMessageBox::critical(this, tr("Error"), e.message());
    }
}

