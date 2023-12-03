/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of log entry class.


#include "LogEntry.h"
#include "Exception.h"


namespace {
QChar constexpr doubleQuote('"'); ///< The double quote character.
QChar constexpr backslash('\\'); ///< The backslash character.
QChar constexpr equal('='); ///< The equal sign character.
QString const keyTime = "time"; ///< The field name for time.
QString const keyLevel = "level"; ///< The field name for level.
QString const keyPackage = "pkg"; ///< The field name for package.
QString const keyService = "service"; ///< the field name for service.
QString const keyMessage = "msg"; ///< The field name for message.
QString const traceColor("#fffffc"); ///< The color for the trace log level.
QString const debugColor("#9bf6ff"); ///< The color for the debug log level.
QString const infoColor("#caffbf"); ///< The color for the info log level.
QString const warnColor("#fdd6a5"); ///< The color for the warn log level.
QString const errorColor("#ffadad"); ///< The color for the error log level.
QString const fatalColor("#a0c4ff"); ///< The color for the fatal log level.
QString const panicColor("#bdb2ff"); ///< The color for the panic log level.
QString const yearStr = QDate::currentDate().toString("yyyy "); // Why is the year not in the log timestamps? We ignore year change for now...
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
QStringList tokenizeBridge34Entry(QString const &str) {
    QStringList result;
    if (str.isEmpty()) {
        return result;
    }
    QChar prevChar(0);
    bool inQuotes = false;
    QString acc;
    for (QChar c: str) {
        if ((c == doubleQuote) && (prevChar != backslash)) {
            if (!acc.isEmpty()) {
                result.append(acc);
                acc = QString();
            }
            inQuotes = !inQuotes;
            prevChar = c;
            continue;
        }

        if ((c == equal) && (!inQuotes)) {
            if (!acc.isEmpty()) {
                result.push_back(acc);
                acc = QString();
            }
            result.push_back(c);
            prevChar = c;
            continue;
        }

        if (c.isSpace() && !inQuotes) {
            if (!acc.isEmpty()) {
                result.append(acc);
                acc = QString();
            }
            prevChar = c;
            continue;
        }

        acc.append(c);
        prevChar = c;
    }

    if (!acc.isEmpty()) {
        result.append(acc);
    }

    return result;
}


//****************************************************************************************************************************************************
/// \param[in] str The log entry string.
/// \param[in] format The log file format.
//****************************************************************************************************************************************************
LogEntry::LogEntry(QString const &str, Format format) {
    this->parse(str, format);
}


//****************************************************************************************************************************************************
/// \return true iff the log entry is valid.
//****************************************************************************************************************************************************
bool LogEntry::isValid() const {
    return error_.isEmpty();
}


//****************************************************************************************************************************************************
/// \return The entry time.
//****************************************************************************************************************************************************
QString LogEntry::time() const {
    return time_;
}

//****************************************************************************************************************************************************
/// \return The entry level.
//****************************************************************************************************************************************************
LogEntry::Level LogEntry::level() const {
    return level_;
}


//****************************************************************************************************************************************************
/// \return The entry package.
//****************************************************************************************************************************************************
QString LogEntry::package() const {
    return package_;
}


//****************************************************************************************************************************************************
/// \return The entry message.
//****************************************************************************************************************************************************
QString LogEntry::message() const {
    return message_;
}


//****************************************************************************************************************************************************
/// \return The error encountered while parsing the entry.
//****************************************************************************************************************************************************
QString LogEntry::error() const {
    return error_;
}


//****************************************************************************************************************************************************
/// \return a constant reference to the entry fields.
//****************************************************************************************************************************************************
QMap<QString, QString> const& LogEntry::fields() const {
    return fields_;
}

//****************************************************************************************************************************************************
/// \return The log entry fieldsString.
//****************************************************************************************************************************************************
QString LogEntry::fieldsString() const {
    QStringList result;
    for (auto it = fields_.begin(); it != fields_.end(); ++it) {
        result.append(QString(R"(%1=%2)").arg(it.key(), it.value()));
    }
    return result.join(" - ");
}


//****************************************************************************************************************************************************
/// \param[in] str The string.
/// \param[in] format The log format.
//****************************************************************************************************************************************************
void LogEntry::parse(QString const &str, Format format) {
    switch (format) {
    case Format::BridgeGUI_3_4_0:
        this->parseBridgeGUI34Entry(str);
        break;
    case Format::Bridge_3_4_0:
        this->parseBridge34Entry(str);
        break;
    case Format::Unknown:
        throw Exception("Failed parsing of log entry of unknown format.");
    }
}


//****************************************************************************************************************************************************
/// \param[in] str The string.
//****************************************************************************************************************************************************
void LogEntry::parseBridgeGUI34Entry(QString const &str) {
    try {
        QRegularExpression const rx(R"(^(?<level>PANI|FATA|ERRO|WARN|INFO|DEBU|TRAC)\[(?<time>.{19})]\s+(?<message>.*)$)");
        QRegularExpressionMatch const match = rx.match(str);
        if (!match.hasMatch()) {
            throw Exception("Invalid log entry");
        }
        level_ = levelFromBridgeGUI34String(match.captured("level"));
        time_ = match.captured("time");
        message_ = match.captured("message");
    } catch (Exception const &e) {
        error_ = e.message();
    }
}


//****************************************************************************************************************************************************
/// \param[in] str The string.
//****************************************************************************************************************************************************
void LogEntry::parseBridge34Entry(QString const &str) {
    try {
        QStringList tokens = tokenizeBridge34Entry(str);
        qsizetype count = tokens.size();
        if (count % 3 != 0) {
            // fix for issue where a logrus field key contains a space.
            if ((count % 3 == 1) && (tokens[9] == "message") && (tokens[10] == "updated")) {
                tokens[9] = "message updated";
                tokens.removeAt(10);
                --count;
            } else {
                throw Exception("Invalid number of elements after tokenization.");
            }
        }
        for (int i = 0; i < count; i += 3) {
            QString const &expectedEqual = tokens[i + 1];
            if (expectedEqual != equal) {
                throw Exception(QString("expected equal sign but encountered '%1'")
                    .arg(expectedEqual.size() < 10 ? expectedEqual : expectedEqual.left(10) + "..."));
            }
            QString const &key = tokens[i];
            QString const &value = tokens[i + 2];
            if (fields_.contains(key)) {
                throw Exception(QString("Duplicate field \"%1\"").arg(key));
            }
            if (key == keyTime) {
                time_ = value;
                continue;
            }
            if (key == keyLevel) {
                level_ = LogEntry::levelFromBridge34String(value);
                continue;
            }

            if ((key == keyPackage) || (key == keyService)) {
                package_ = value;
                continue;
            }

            if (key == keyMessage) {
                message_ = value;
                continue;
            }

            fields_[key] = value;
        }
    } catch (Exception const &e) {
        QString const msg = e.message();
        error_ = msg.isEmpty() ? "Unknown error" : msg;
        fields_.clear();
    }
}


//****************************************************************************************************************************************************
/// \param[in] str The string.
/// \return The level parsed from the string
//****************************************************************************************************************************************************
LogEntry::Level LogEntry::levelFromBridge34String(QString const &str) {
    if (str.compare("trace", Qt::CaseInsensitive) == 0)
        return Level::Trace;
    if (str.compare("debug", Qt::CaseInsensitive) == 0)
        return Level::Debug;
    if (str.compare("info", Qt::CaseInsensitive) == 0)
        return Level::Info;
    if (str.compare("warning", Qt::CaseInsensitive) == 0)
        return Level::Warn;
    if (str.compare("error", Qt::CaseInsensitive) == 0)
        return Level::Error;
    if (str.compare("fatal", Qt::CaseInsensitive) == 0)
        return Level::Fatal;
    if (str.compare("panic", Qt::CaseInsensitive) == 0)
        return Level::Panic;
    qCritical() << QString("Unknown log level '%1'").arg(str);
    return Level::Trace;
}


//****************************************************************************************************************************************************
/// \param[in] str The string.
/// \return The level parsed from the string
//****************************************************************************************************************************************************LogEntry::Level LogEntry::levelFromBridge34String(QString const &str) {
LogEntry::Level LogEntry::levelFromBridgeGUI34String(QString const &str) {
    if (str.compare("TRAC", Qt::CaseInsensitive) == 0)
        return Level::Trace;
    if (str.compare("DEBU", Qt::CaseInsensitive) == 0)
        return Level::Debug;
    if (str.compare("INFO", Qt::CaseInsensitive) == 0)
        return Level::Info;
    if (str.compare("WARN", Qt::CaseInsensitive) == 0)
        return Level::Warn;
    if (str.compare("ERRO", Qt::CaseInsensitive) == 0)
        return Level::Error;
    if (str.compare("FATA", Qt::CaseInsensitive) == 0)
        return Level::Fatal;
    if (str.compare("PANI", Qt::CaseInsensitive) == 0)
        return Level::Panic;
    qCritical() << QString("Unknown log level '%1'").arg(str);
    return Level::Trace;
}


//****************************************************************************************************************************************************
/// \param[in] level The level.
/// \return The string for the level.
//****************************************************************************************************************************************************
QString LogEntry::levelToString(LogEntry::Level level) {
    switch (level) {
    case Level::Trace:
        return "trace";
    case Level::Debug:
        return "debug";
    case Level::Info:
        return "info";
    case Level::Warn:
        return "warning";
    case Level::Error:
        return "error";
    case Level::Fatal:
        return "fatal";
    case Level::Panic:
        return "panic";
    default:
        qCritical() << QString("Unknown log level '%1'").arg(static_cast<qint64>(level));
        return "unknown";
    }
}


//****************************************************************************************************************************************************
/// \param[in] level The level.
/// \return The color associated with a level.
//****************************************************************************************************************************************************
QColor LogEntry::levelColor(LogEntry::Level level) {
    switch (level) {
    case Level::Trace:
        return traceColor;
    case Level::Debug:
        return debugColor;
    case Level::Info:
        return infoColor;
    case Level::Warn:
        return warnColor;
    case Level::Error:
        return errorColor;
    case Level::Fatal:
        return fatalColor;
    case Level::Panic:
        return panicColor;
    default:
        qCritical() << QString("Unknown log level '%1'").arg(static_cast<qint64>(level));
        return traceColor;
    }
}


//****************************************************************************************************************************************************
/// \return The date/time for the entry.
//****************************************************************************************************************************************************
QDateTime LogEntry::dateTime() const {
    return QDateTime::fromString(yearStr + time_, "yyyy MMM dd HH:mm:ss.zzz");
}
