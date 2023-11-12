/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of filter model for the log table view.



#ifndef ANALOG_FILTER_MODEL_H
#define ANALOG_FILTER_MODEL_H


#include "Log.h"


//****************************************************************************************************************************************************
/// \brief The filter model for the log.
//****************************************************************************************************************************************************
class FilterModel: public QSortFilterProxyModel {
    Q_OBJECT
public: // member functions.
    FilterModel(Log& log); ///< Default constructor.
    FilterModel(FilterModel const&) = delete; ///< Disabled copy-constructor.
    FilterModel(FilterModel&&) = delete; ///< Disabled assignment copy-constructor.
    ~FilterModel() override = default; ///< Destructor.
    FilterModel& operator=(FilterModel const&) = delete; ///< Disabled assignment operator.
    FilterModel& operator=(FilterModel&&) = delete; ///< Disabled move assignment operator.
    LogEntry::Level level() const; ///< Get the level of the filer.
    void setLevel(LogEntry::Level); ///< Set the level of the filter.
    bool useStrictLevelFilter() const; ///< Check if the level filter is strict.
    void  setUseStrictLevelFilter(bool strict); ///< Set the strictness of the level filter.

private: // member functions.
    bool filterAcceptsRow(int sourceRow, QModelIndex const &source_parent) const override; ///< check if a row show be accepted.

private: // data members.
    Log& log_; ///< The log
    LogEntry::Level level_ { LogEntry::Level::Trace }; ///< The minimum level to show.
    bool useStrictLevelFilter_ { false }; ///< Set if the level_ filtering should exclude entries above the selected level.
};


#endif //ANALOG_FILTER_MODEL_H
