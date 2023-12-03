/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of filter model for the log table view.


#include "FilterModel.h"


//****************************************************************************************************************************************************
/// \param[in] log The log.
//****************************************************************************************************************************************************
FilterModel::FilterModel(SPLog const &log)
    : log_(log) {
    this->QSortFilterProxyModel::setSourceModel(log_.get());
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
void FilterModel::setLog(SPLog const &log) {
    log_ = log;
    this->setSourceModel(log.get());
}


//****************************************************************************************************************************************************
/// \return The level.
//****************************************************************************************************************************************************
LogEntry::Level FilterModel::level() const {
    return level_;
}


//****************************************************************************************************************************************************
/// \param[in] level The level.
//****************************************************************************************************************************************************
void FilterModel::setLevel(LogEntry::Level level) {
    if (level == level_) {
        return;
    }
    level_ = level;
    this->invalidate();
}

//****************************************************************************************************************************************************
/// \param[in] sourceRow The row index.
/// \return true iff the row should be displayed.
//****************************************************************************************************************************************************
bool FilterModel::filterAcceptsRow(int sourceRow, QModelIndex const &) const {
    if (!log_) {
        return false;
    }
    LogEntry const &entry = log_->entries_[sourceRow];
    if ((useStrictLevelFilter_) && (static_cast<int>(entry.level()) != static_cast<int>(level_))) {
        return false;
    }
    if ((!useStrictLevelFilter_) && (static_cast<int>(entry.level()) < static_cast<int>(level_))) {
        return false;
    }

    if ((!packageFilter_.isEmpty()) && !entry.package().contains(packageFilter_, Qt::CaseInsensitive)) {
        return false;
    }

    if (textFilter_.isEmpty()) {
        return true;
    }

    if (entry.message().contains(textFilter_, Qt::CaseInsensitive)) {
        return true;
    }
    QMap<QString, QString> const &fields = entry.fields();
    for (QMap<QString, QString>::const_iterator it = fields.begin(); it != fields.end(); ++it) {
        if (it.key().contains(textFilter_, Qt::CaseInsensitive) || it.value().contains(textFilter_, Qt::CaseInsensitive)) {
            return true;
        }
    }

    return false;
}


//****************************************************************************************************************************************************
/// \return true iff the level filter is strict.
//****************************************************************************************************************************************************
bool FilterModel::useStrictLevelFilter() const {
    return useStrictLevelFilter_;
}


//****************************************************************************************************************************************************
/// \param[in] strict the new strictness of the level filter.
//****************************************************************************************************************************************************
void FilterModel::setUseStrictLevelFilter(bool strict) {
    if (strict == useStrictLevelFilter_) {
        return;
    }

    useStrictLevelFilter_ = strict;
    this->invalidate();
}


//****************************************************************************************************************************************************
/// \return The package filter.
//****************************************************************************************************************************************************
QString FilterModel::packageFilter() const {
    return packageFilter_;
}


//****************************************************************************************************************************************************
/// \param[in] filter The package filter.
//****************************************************************************************************************************************************
void FilterModel::setPackageFilter(QString const &filter) {
    if (filter == packageFilter_) {
        return;
    }
    packageFilter_ = filter;
    this->invalidate();
}


//****************************************************************************************************************************************************
/// \return The text filter.
//****************************************************************************************************************************************************
QString FilterModel::textFilter() {
    return textFilter_;
}


//****************************************************************************************************************************************************
/// \param[in] filter The text filter.
//****************************************************************************************************************************************************
void FilterModel::setTextFilter(QString const &filter) {
    if (textFilter_ == filter) {
        return;
    }
    textFilter_ = filter;
    this->invalidate();
}
