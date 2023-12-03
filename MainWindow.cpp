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
    ui_.sessionTree->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    ui_.sessionTree->setMinimumWidth(250);
    connect(ui_.sessionTree, &QTreeView::activated, this, &MainWindow::onSessionSelected);
    connect(ui_.actionOpenFile, &QAction::triggered, this, &MainWindow::onActionOpenFile);
    connect(ui_.actionShowReport, &QAction::triggered, this, &MainWindow::onActionShowReport);
    connect(ui_.sessionWidget, &SessionWidget::logStatusMessageChanged, this, &MainWindow::onLogStatusMessageChanged);
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
/// \param[in] index The index of the selected session.
//****************************************************************************************************************************************************
void MainWindow::onSessionSelected(QModelIndex const &index) const {
    ui_.sessionWidget->setSession(index.isValid() ? sessionList_[index.row()] : std::optional<Session> {});
}


//****************************************************************************************************************************************************
/// \param[in] message The message.
//****************************************************************************************************************************************************
void MainWindow::onLogStatusMessageChanged(QString const &message) const {
    if (message.isEmpty()) {
        ui_.statusbar->clearMessage();
    } else {
        ui_.statusbar->showMessage(message);
    }
}
