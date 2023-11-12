/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of filter model for the log table view.


#include "FilterModel.h"


//****************************************************************************************************************************************************
/// \param[in] log The log.
//****************************************************************************************************************************************************
FilterModel::FilterModel(Log& log)
    : QSortFilterProxyModel()
    , log_(log){
    this->setSourceModel(&log_);
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
    if (useStrictLevelFilter_) {
        return static_cast<int>(log_.entries_[sourceRow].level()) == static_cast<int>(level_);
    } else {
        return static_cast<int>(log_.entries_[sourceRow].level()) >= static_cast<int>(level_);
    }
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
