/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of main window class.


#include "MainWindow.h"
#include "ReportDialog.h"
#include "Exception.h"
#include "cmake-build-release/CMakeFiles/Analog.dir/cmake_pch_arm64.hxx"


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
MainWindow::MainWindow() {
    ui_.setupUi(this);

    ui_.sessionTree->setModel(&sessionList_);
    ui_.tableView->setModel(&filter_);
    connect(ui_.sessionTree, &QTreeView::activated, this, &MainWindow::onSessionSelected);
    connect(ui_.actionOpenFile, &QAction::triggered, this, &MainWindow::onActionOpenFile);
    connect(ui_.actionShowReport, &QAction::triggered, this, &MainWindow::onActionShowReport);
    connect(&filter_, &Log::modelReset, this, &MainWindow::onLogLoaded);
    connect(&filter_, &Log::layoutChanged, this, &MainWindow::onLayoutChanged);
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
    QStringList const filePaths = QFileDialog::getOpenFileNames(this, tr("Select log file"), QString(), tr("Log files (*.log);;All files (*.*)"));
    if (!filePaths.isEmpty()) {
        this->open(filePaths);
    }
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
/// \param[in] filePaths The list of file paths to open
//****************************************************************************************************************************************************
void MainWindow::open(QStringList const &filePaths) {
    try {
        sessionList_.open(filePaths);
        // QStringList paths = filePaths;
        // if (!validateFilesForOpening(paths)) {
        //     return;
        // }
        // log_.open(paths);
        // if (log_.hasErrors()) {
        //     QStringList errors = log_.errors();
        //     if (errors.size() > 10) {
        //         errors = errors.first(10);
        //         errors.append("...");
        //     }
        //     QMessageBox::critical(this, "Error", errors.join("\n"));
        // }
        ui_.sessionTree->expandAll();
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
    QStringList paths;
    std::ranges::transform(urls, std::back_inserter(paths), [](QUrl const &url) -> QString { return url.toLocalFile(); });
    this->open(paths);
    event->acceptProposedAction();
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::onActionShowReport() {
    try {
        if (!log_) {
            return;
        }
        ReportDialog dlg(this, log_->generateReport());
        dlg.exec();
    } catch (Exception const &e) {
        QMessageBox::critical(this, "Error", e.message());
    }
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void MainWindow::onLogLoaded() const {
    ui_.tableView->resizeColumnsToContents();
    ui_.tableView->setColumnWidth(3, qMin(ui_.tableView->columnWidth(3), 600));
    this->onLayoutChanged();
}


//****************************************************************************************************************************************************
/// This slot is called when the filtering of the log change.
//****************************************************************************************************************************************************
void MainWindow::onLayoutChanged() const {
    qsizetype const entryCount = filter_.rowCount();
    if (entryCount != 0) {
        ui_.statusbar->showMessage(entryCount > 1 ? QString("%1 entries").arg(entryCount) : "1 entry");
    } else {
        ui_.statusbar->clearMessage();
    }
}


//****************************************************************************************************************************************************
/// \param[in] index The index of the selected session.
//****************************************************************************************************************************************************
void MainWindow::onSessionSelected(QModelIndex const &index) {
    filter_.setLog(index.isValid() ? sessionList_.log(index) : SPLog {});
}
