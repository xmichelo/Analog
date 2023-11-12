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
    QString time() const; ///< Return the entry time.
    QString level() const; ///< Return the entry level.
    QString package() const; ///< Return the entry package.
    QString message() const; ///< Return the entry message.
    QString fields() const; ///< Return the log entry fields.
    QString error() const; ///< Return the description of the problem encountered while parsing the entry.

private:
    void parse(QString const& str); ///< Parse the log entry from a string.

private: // member functions
    QString time_; ///< The entry time.
    QString level_; ///< The entry level.
    QString package_; ///< The entry package.
    QString message_; ///< The entry message.
    QMap<QString, QString> fields_; ///< The other entry fields.
    QString error_;  ///< The error that make the line invalid.
};



#endif //ANALOG_LOG_ENTRY_H
