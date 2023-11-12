/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of log entry class.


#ifndef ANALOG_LOG_ENTRY_H
#define ANALOG_LOG_ENTRY_H


//****************************************************************************************************************************************************
/// \brief Log entry class.
//****************************************************************************************************************************************************
class LogEntry {
public: // member functions.
    explicit LogEntry(QString const& str); ///< Default constructor for log entry from string
    LogEntry(LogEntry const &) = default; ///< Disabled copy-constructor.
    LogEntry(LogEntry &&) = default; ///< Disabled assignment copy-constructor.
    ~LogEntry() = default; ///< Destructor.
    LogEntry &operator=(LogEntry const &) = default; ///< Disabled assignment operator.
    LogEntry &operator=(LogEntry &&) = default; ///< Disabled move assignment operator.

    bool isValid() const; ///< Return true iff the log entry is valid.
    QString value() const; ///< Return the log entry value.

private: // member functions
    QString entry_; ///< The entry
};



#endif //ANALOG_LOG_ENTRY_H
