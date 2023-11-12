/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of log entry class.


#include "LogEntry.h"
#include "Exception.h"


namespace {


QChar const doubleQuote('"');
QChar const backslash('\\');
QChar const equal('=');
QString const keyTime = "time";
QString const keyLevel = "level";
QString const keyPackage = "pkg";
QString const keyService = "service";
QString const keyMessage = "msg";


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
QString LogEntry::level() const {
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
                level_ = value;
                continue;
            }

            if ((key == keyPackage) || (key == keyService)){
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


