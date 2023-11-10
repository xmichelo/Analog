/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of main window class.


#include "MainWindow.h"


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
MainWindow::MainWindow()
    : QMainWindow() {
    ui_.setupUi(this);

    connect(ui_.actionOpenFile, &QAction::triggered, this, &MainWindow::onActionOpenFile);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::onActionOpenFile() {
    if (QFileDialog::getOpenFileName(this, tr("Select log file"), QString(), tr("Log files (*.log);;All files (*.*)")).isEmpty()) {
        return;
    }
}

