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
public: // data types
    enum class Level {
        Trace = 0,
        Debug = 1,
        Info = 2,
        Warn = 3,
        Error = 4,
        Fatal = 5,
        Panic = 6,
    }; ///< Log levels

    enum class Format {
        BridgeGUI_3_4_0, ///< bridge-gui version 3.4.0 and above.
        Bridge_3_4_0, ///< bridge version 3.4.0 and above.
        Unknown ///< Unknown format
    }; ///< Enumeration for log file formats.

public: // member functions.
    explicit LogEntry(QString const &str, Format format); ///< Default constructor for log entry from string
    LogEntry(LogEntry const &) = default; ///< Disabled copy-constructor.
    LogEntry(LogEntry &&) = default; ///< Disabled assignment copy-constructor.
    ~LogEntry() = default; ///< Destructor.
    LogEntry &operator=(LogEntry const &) = default; ///< Disabled assignment operator.
    LogEntry &operator=(LogEntry &&) = default; ///< Disabled move assignment operator.

    bool isValid() const; ///< Return true iff the log entry is valid.
    QString time() const; ///< Return the entry time.
    QDateTime dateTime() const; ///< Return the date/time of the entry.
    Level level() const; ///< Return the entry level.
    QString package() const; ///< Return the entry package.
    QString message() const; ///< Return the entry message.
    QMap<QString, QString> const &fields() const; ///< Returns a reference to the entry fields.
    QString fieldsString() const; ///< Return the log entry as a string.
    QString error() const; ///< Return the description of the problem encountered while parsing the entry.

public: // static members
    static Level levelFromBridge34String(QString const &str); ///< convert a string from a bridge 3.4 log to a log level.
    static Level levelFromBridgeGUI34String(QString const &str); ///< convert a string from a bridge-gui 3.4 log to a log level.
    static QString levelToString(Level level); ///< Return the string for a level.
    static QColor levelColor(Level level); ///< Return the color for a level.

private: // member functions
    void parse(QString const &str, Format format); ///< Parse the log entry from a string.
    void parseBridgeGUI34Entry(QString const& str); ///< Parse a log entry in bridge-gui 3.4 format.
    void parseBridge34Entry(QString const& str); ///< Parse a log entry in bridge 3.4 format.

private: // member functions
    QString time_; ///< The entry date/time.
    Level level_{Level::Trace}; ///< The entry level.
    QString package_; ///< The entry package.
    QString message_; ///< The entry message.
    QMap<QString, QString> fields_; ///< The other entry fieldsString.
    QString error_;  ///< The error that make the line invalid.
};


#endif //ANALOG_LOG_ENTRY_H
