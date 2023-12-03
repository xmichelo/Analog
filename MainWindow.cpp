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
MainWindow::MainWindow() {
    ui_.setupUi(this);

    ui_.sessionList->setModel(&sessionList_);
    //ui_.sessionList->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    ui_.sessionList->setMinimumWidth(250);
    connect(ui_.sessionList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onSelectedSessionChanged);
    connect(ui_.actionOpenFile, &QAction::triggered, this, &MainWindow::onActionOpenFile);
    connect(ui_.actionShowReport, &QAction::triggered, this, &MainWindow::onActionShowReport);
    connect(ui_.sessionWidget, &SessionWidget::logStatusMessageChanged, this, &MainWindow::onLogStatusMessageChanged);
    connect(ui_.sessionWidget, &SessionWidget::logErrorsOccurred, this, &MainWindow::onLogErrors);
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
        if (sessionList_.count() == 0) {
            ui_.sessionWidget->setSession({});
        } else {
            ui_.sessionList->setCurrentIndex(ui_.sessionList->model()->index(0, 0, QModelIndex()));
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
/// \param[in] selected The new selection.
//****************************************************************************************************************************************************
void MainWindow::onSelectedSessionChanged(const QItemSelection &selected, const QItemSelection &) const {
    QList<QModelIndex> const indexes = selected.indexes();
    ui_.sessionWidget->setSession(indexes.isEmpty() ? std::optional<Session> {} : sessionList_[indexes.first().row()]);
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


//****************************************************************************************************************************************************
/// \param[in] errors The list of errors.
//****************************************************************************************************************************************************
void MainWindow::onLogErrors(QStringList const &errors) {
    QStringList errs = errors;
    if (errs.count() > 5) {
        errs.resize(5);
        errs.append("...");
    }
    QMessageBox::critical(this, "Error", errs.join("\n"));
}
