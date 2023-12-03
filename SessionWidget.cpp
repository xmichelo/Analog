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
    connect(&filter_, &Log::modelReset, this, &SessionWidget::onLogLoaded);
    connect(&filter_, &Log::layoutChanged, this, &SessionWidget::onLayoutChanged);

    ui_.editFilter->setText(filter_.textFilter());
    ui_.editPackage->setText(filter_.packageFilter());
    ui_.comboLevel->setCurrentIndex(static_cast<int>(filter_.level()));
    ui_.checkAndAbove->setChecked(!filter_.useStrictLevelFilter());
}


//****************************************************************************************************************************************************
/// \param[in] log The log.
//****************************************************************************************************************************************************
void SessionWidget::setLog(SPLog const &log) {
    filter_.setLog(log);
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
