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
class FilterModel : public QSortFilterProxyModel {
    Q_OBJECT

public: // member functions.
    explicit FilterModel(SPLog const& log = {}); ///< Default constructor.
    FilterModel(FilterModel const &) = delete; ///< Disabled copy-constructor.
    FilterModel(FilterModel &&) = delete; ///< Disabled assignment copy-constructor.
    ~FilterModel() override = default; ///< Destructor.
    FilterModel& operator=(FilterModel const &) = delete; ///< Disabled assignment operator.
    FilterModel& operator=(FilterModel &&) = delete; ///< Disabled move assignment operator.
    void setLog(SPLog const &log); ///< Set the log.
    LogEntry::Level level() const; ///< Get the level of the filer.
    void setLevel(LogEntry::Level); ///< Set the level of the filter.
    bool useStrictLevelFilter() const; ///< Check if the level filter is strict.
    QString packageFilter() const; ///< Get the package filter string.
    void setUseStrictLevelFilter(bool strict); ///< Set the strictness of the level filter.
    void setPackageFilter(QString const &filter); ///< Set the package filter string.
    QString textFilter(); ///< Get the text filter.
    void setTextFilter(QString const &filter); ///< Set The text filter.

signals:
    void logErrorsOccurred(QStringList const& list); ///< Signal emitted when errors occured while opening a log.

private: // member functions.
    bool filterAcceptsRow(int sourceRow, QModelIndex const &) const override; ///< check if a row show be accepted.

private: // data members.
    SPLog log_; ///< The log
    LogEntry::Level level_ { LogEntry::Level::Trace }; ///< The minimum level to show.
    bool useStrictLevelFilter_ { false }; ///< Set if the level_ filtering should exclude entries above the selected level.
    QString packageFilter_; ///< The filter to apply to the package.
    QString textFilter_; ///< The text filter.
};


#endif //ANALOG_FILTER_MODEL_H
