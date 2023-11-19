/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of main window class.


#include "MainWindow.h"
#include "ReportDialog.h"
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
    connect(ui_.actionShowReport, &QAction::triggered, this, &MainWindow::onActionShowReport);
    connect(&log_, &Log::modelReset, this, &MainWindow::onLogLoaded);
    connect(ui_.editFilter, &QLineEdit::textChanged, this, &MainWindow::onTextFilterChanged);
    connect(ui_.editPackage, &QLineEdit::textChanged, this, &MainWindow::onPackageFilterChanged);
    connect(ui_.comboLevel, &QComboBox::currentIndexChanged, this, &MainWindow::onLevelComboChanged);
    connect(ui_.checkAndAbove, &QCheckBox::stateChanged, this, &MainWindow::onLevelStrictnessChanged);

    ui_.editFilter->setText(filter_.textFilter());
    ui_.editPackage->setText(filter_.packageFilter());
    ui_.comboLevel->setCurrentIndex(static_cast<int>(filter_.level()));
    ui_.checkAndAbove->setChecked(!filter_.useStrictLevelFilter());
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::onActionOpenFile() {
    QString const filePath = QFileDialog::getOpenFileName(this, tr("Select log file"), QString(), tr("Log files (*.log);;All files (*.*)"));
    if (filePath.isEmpty()) {
        return;
    }
    this->openFile(filePath);
}


//****************************************************************************************************************************************************
/// \param[in] value The text filter.
//****************************************************************************************************************************************************
void MainWindow::onTextFilterChanged(QString const &value) {
    filter_.setTextFilter(value);
}


//****************************************************************************************************************************************************
/// \param[in] index The index of the current item.
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
/// \param[in] value The new value for the package filter.
//****************************************************************************************************************************************************
void MainWindow::onPackageFilterChanged(QString const &value) {
    filter_.setPackageFilter(value);
}


//****************************************************************************************************************************************************
/// \param[in] filePath The path of the file to open.
//****************************************************************************************************************************************************
void MainWindow::openFile(QString const &filePath) {
    try {
        log_.open(filePath);
        if (log_.hasErrors()) {
            QStringList errors = log_.errors();
            if (errors.size() > 10) {
                errors = errors.first(10);
                errors.append("...");
            }
            QMessageBox::critical(this, "Error", errors.join("\n"));
        }
    } catch (Exception const &e) {
        QMessageBox::critical(this, tr("Error"), e.message());
    }
}

//****************************************************************************************************************************************************
/// \param[in] event The event.
//****************************************************************************************************************************************************
void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

//****************************************************************************************************************************************************
/// \param[in] event The event.
//****************************************************************************************************************************************************
void MainWindow::dragMoveEvent(QDragMoveEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

//****************************************************************************************************************************************************
/// \param[in] event The event.
//****************************************************************************************************************************************************
void MainWindow::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}

//****************************************************************************************************************************************************
/// \param[in] event The event.
//****************************************************************************************************************************************************
void MainWindow::dropEvent(QDropEvent *event) {
    QMimeData const *mimeData = event->mimeData();
    if (!mimeData->hasUrls())
        return;
    QList<QUrl> urls = mimeData->urls();
    if (urls.empty()) {
        return;
    }
    QString const &path = urls[0].toLocalFile();
    this->openFile(path);
    event->acceptProposedAction();
}

//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::onActionShowReport() {
    try {
        ReportDialog dlg(this, log_.generateReport());
        dlg.exec();
    } catch (Exception const &e) {
        QMessageBox::critical(this, "Error", e.message());
    }
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::onLogLoaded() {
    ui_.tableView->resizeColumnsToContents();
    ui_.tableView->setColumnWidth(3, qMin(ui_.tableView->columnWidth(3), 600));
}
