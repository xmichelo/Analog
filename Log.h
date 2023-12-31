/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of log class.


#ifndef ANALOG_LOG_H
#define ANALOG_LOG_H


#include "FilenameInfo.h"
#include "LogEntry.h"
#include "Report.h"


//****************************************************************************************************************************************************
/// \brief Log class
//****************************************************************************************************************************************************
class Log : public QAbstractTableModel {
Q_OBJECT

public: // member functions.
    explicit Log(QStringList const& filePaths = {}); ///< Return a log read from a set of files.
    explicit Log(QString const &filePath); ///< Return a log read from a single file.
    Log(Log const &) = delete; ///< Disabled copy-constructor.
    Log(Log &&) = delete; ///< Disabled assignment copy-constructor.
    ~Log() override = default; ///< Destructor.
    Log &operator=(Log const &) = delete; ///< Disabled assignment operator.
    Log &operator=(Log &&) = delete; ///< Disabled move assignment operator.

    void clear(bool resetModel = true); ///< Clear the content of the log.
    void open(QString const &filePath); ///< Open a log from file.
    void open(QStringList const &filePaths); ///< Open a log from an ordered list of files.
    int rowCount(QModelIndex const &parent) const override; ///< Get the number of rows in the model.
    int columnCount(QModelIndex const &parent) const override; ///< Get the number of columns in the model.
    QVariant data(QModelIndex const &index, int role) const override; ///< Get the data at an index in the model.
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override; ///< Get the header data for a row/column.

    QList<LogEntry> const &entries() const; ///< Returns a constant reference to the log entries.
    bool isEmpty() const; ///< Check if the log is empty.
    Report generateReport() const; ///< Generates a report from the log.
    bool hasErrors() const; ///< Returns true iff errors where encountered while parsing the log.
    QStringList errors() const; ///< Returns the error encountered while parsing the log.

signals:
    void logErrorsOccurred(QStringList const& list); ///< Signal emitted when errors occured while opening a log.

private: // static member functions.
    static LogEntry::Format getLogFormat(QString const &file); ///< Determines the log file format.

private: // member functions.
    void appendFileContent(QString const &filePath); ///< Append the content of a file to the log.

public: // data members
    LogEntry::Format format_ { LogEntry::Format::Unknown }; ///< The log format.
    QStringList errors_; ///< The errors encountered while passing the log.
    QList<LogEntry> entries_; ///< The log entries.
};


typedef std::shared_ptr<Log> SPLog; ///< Type definition for shared pointer to log.


#endif //ANALOG_LOG_H
