/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of a session widget class.
///
///
#include "SessionWidget.h"


//****************************************************************************************************************************************************
/// \param[in] parent The parent widget of the widget.
//****************************************************************************************************************************************************
SessionWidget::SessionWidget(QWidget *parent)
    : QWidget(parent) {
    ui_.setupUi(this);
    ui_.tableView->setModel(&filter_);

    connect(ui_.editFilter, &QLineEdit::textChanged, this, &SessionWidget::onTextFilterChanged);
    connect(ui_.editPackage, &QLineEdit::textChanged, this, &SessionWidget::onPackageFilterChanged);
    connect(ui_.comboLevel, &QComboBox::currentIndexChanged, this, &SessionWidget::onLevelComboChanged);
    connect(ui_.checkAndAbove, &QCheckBox::stateChanged, this, &SessionWidget::onLevelStrictnessChanged);
    connect(ui_.buttonBridge, &QPushButton::clicked, this, &SessionWidget::onShowBridgeLog);
    connect(ui_.buttonGUI, &QPushButton::clicked, this, &SessionWidget::onShowGUILog);
    connect(ui_.buttonLauncher, &QPushButton::clicked, this, &SessionWidget::onShowLauncherLog);
    connect(&filter_, &Log::modelReset, this, &SessionWidget::onLogLoaded);
    connect(&filter_, &Log::layoutChanged, this, &SessionWidget::onLayoutChanged);

    ui_.editFilter->setText(filter_.textFilter());
    ui_.editPackage->setText(filter_.packageFilter());
    ui_.comboLevel->setCurrentIndex(static_cast<int>(filter_.level()));
    ui_.checkAndAbove->setChecked(!filter_.useStrictLevelFilter());
}


//****************************************************************************************************************************************************
/// \param[in] session The session.
//****************************************************************************************************************************************************
void SessionWidget::setSession(std::optional<Session> const &session) {
    session_ = session;

    this->updateGUI();

    bool const hasBridgeLog = session->hasBridgeLog();
    bool const hasGUILog = session->hasGUILog();
    bool const hasLauncherLog = session->hasLauncherLog();

    if (hasBridgeLog) {
        filter_.setLog(session->bridgeLog());
        ui_.buttonBridge->setChecked(true);
        return;
    }
    if (hasGUILog) {
        filter_.setLog(session->guiLog());
        ui_.buttonGUI->setChecked(true);
        return;
    }
    if (hasLauncherLog) {
        filter_.setLog(session_->launcherLog());
        ui_.buttonLauncher->setChecked(true);
        return;
    }

}

//****************************************************************************************************************************************************
/// \param[in] value The text filter.
//****************************************************************************************************************************************************
void SessionWidget::onTextFilterChanged(QString const &value) {
    filter_.setTextFilter(value);
}


//****************************************************************************************************************************************************
/// \param[in] index The index of the current item.
//****************************************************************************************************************************************************
void SessionWidget::onLevelComboChanged(int index) {
    filter_.setLevel(static_cast<LogEntry::Level>(index));
}


//****************************************************************************************************************************************************
/// \param[in] nonStrict The strictness of the level filter.
//****************************************************************************************************************************************************
void SessionWidget::onLevelStrictnessChanged(bool nonStrict) {
    filter_.setUseStrictLevelFilter(!nonStrict);
}


//****************************************************************************************************************************************************
/// \param[in] value The new value for the package filter.
//****************************************************************************************************************************************************
void SessionWidget::onPackageFilterChanged(QString const &value) {
    filter_.setPackageFilter(value);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void SessionWidget::onLogLoaded() {
    ui_.tableView->resizeColumnsToContents();
    ui_.tableView->setColumnWidth(3, qMin(ui_.tableView->columnWidth(3), 600));
    this->onLayoutChanged();
}


//****************************************************************************************************************************************************
/// This slot is called when the filtering of the log change.
//****************************************************************************************************************************************************
void SessionWidget::onLayoutChanged() {
    QString message;
    qsizetype const entryCount = filter_.rowCount();
    if (entryCount != 0) {
        message = entryCount > 1 ? QString("%1 entries").arg(entryCount) : "1 entry";
    }
    emit logStatusMessageChanged(message);
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void SessionWidget::onShowBridgeLog() {
    filter_.setLog(session_.has_value() ? session_->bridgeLog() : SPLog {});
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void SessionWidget::onShowGUILog() {
    filter_.setLog(session_.has_value() ? session_->guiLog() : SPLog {});
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void SessionWidget::onShowLauncherLog() {
    filter_.setLog(session_.has_value() ? session_->launcherLog() : SPLog {});
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void SessionWidget::updateGUI() {
    bool const hasSession = session_.has_value();
    ui_.buttonBridge->setEnabled(hasSession && session_->hasBridgeLog());
    ui_.buttonGUI->setEnabled(hasSession && session_->hasGUILog());
    ui_.buttonLauncher->setEnabled(hasSession && session_->hasLauncherLog());

}
