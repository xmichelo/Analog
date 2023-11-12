/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of log entry class.


#include "LogEntry.h"
#include "Exception.h"


namespace {


QChar const doubleQuote('"'); ///< The double quote character.
QChar const backslash('\\'); ///< The backslash character.
QChar const equal('='); ///< The equal sign character.
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


}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
QStringList tokenize(QString const &str) {
    QStringList result;
    if (str.isEmpty()) {
        return result;
    }
    QChar prevChar('\n');
    bool inQuotes = false;
    QString acc;
    int n = -1;
    for (QChar c: str) {
        ++n;
        if ((c == doubleQuote) && (prevChar != backslash)) {
            if (inQuotes) {
                result.append(acc);
                inQuotes = false;
                acc = QString();
            } else {
                if (!acc.isEmpty()) {
                    throw Exception(QString("invalid quote found at position %1").arg(n));
                }
                inQuotes = true;
            }
            prevChar = c;
            continue;
        }

        if ((c == equal) && (!inQuotes)) {
            if (acc.isEmpty()) {
                throw Exception(QString("invalid equal sign found at position %1").arg(n));
            } else {
                if (result.size() % 2 != 0) {
                    throw Exception(QString("invalid equal sign found at position %1").arg(n));
                }
                result.append(acc);
                acc = QString();
            }
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
//****************************************************************************************************************************************************
LogEntry::LogEntry(QString const &str) {
    this->parse(str);
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
/// \return The log entry fields.
//****************************************************************************************************************************************************
QString LogEntry::fields() const {
    QStringList result;
    for (auto it = fields_.begin(); it != fields_.end(); ++it) {
        result.append(QString(R"(%1=%2)").arg(it.key(), it.value()));
    }
    return result.join(" - ");
}


//****************************************************************************************************************************************************
/// \param[in] str The string.
//****************************************************************************************************************************************************
void LogEntry::parse(QString const &str) {
    try {
        QStringList const tokens = tokenize(str);
        qsizetype const count = tokens.size();
        if (count % 2 != 0) {
            throw Exception("Invalid number of elements after tokenization.");
        }
        for (int i = 0; i < count; i += 2) {
            QString const &key = tokens[i];
            QString const &value = tokens[i + 1];
            if (fields_.contains(key)) {
                throw Exception(QString("Duplicate field \"%1\"").arg(key));
            }
            if (key == keyTime) {
                time_ = value;
                continue;
            }
            if (key == keyLevel) {
                level_ = LogEntry::levelFromString(value);
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
//
//****************************************************************************************************************************************************
LogEntry::Level LogEntry::levelFromString(QString const &str) {
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
//
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
        qCritical() << QString("Unknown log level '%1'").arg(qint64(level));
        return "unknown";
    }
}


//****************************************************************************************************************************************************
//
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
        qCritical() << QString("Unknown log level '%1'").arg(qint64(level));
        return traceColor;
    }
}


