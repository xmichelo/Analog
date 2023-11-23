#include "Log.h"
#include "Exception.h"


//****************************************************************************************************************************************************
/// \param[in] filePath The path of the log file.
///***************************************************************************************************************************************************
void Log::open(QString const &filePath) {
    this->open(QStringList{filePath});
}


//****************************************************************************************************************************************************
/// No check is perform on the ordering of the file.
///
/// \param[in] filePaths The ordered list of files forming the log.
//****************************************************************************************************************************************************
void Log::open(QStringList const &filePaths) {
    entries_.clear();
    errors_.clear();
    this->beginResetModel();
    try {
        for (QString const &filePath: filePaths) {
            this->appendFileContent(filePath);
        }
    } catch (Exception const &e) {
        errors_ = {e.message()};
    }
    this->endResetModel();
}


//****************************************************************************************************************************************************
/// \return the number of rows in the model.
//****************************************************************************************************************************************************
int Log::rowCount(QModelIndex const &) const {
    return int(entries_.count());
}


//****************************************************************************************************************************************************
/// \return The number of columns in the model.
//****************************************************************************************************************************************************
int Log::columnCount(QModelIndex const &) const {
    return 5;
}


//****************************************************************************************************************************************************
/// \param[in] index The index of the data to retrieve.
/// \param[in] role The role of the data to retrieve.
/// \return The data for a given role at a model index.
//****************************************************************************************************************************************************
QVariant Log::data(QModelIndex const &index, int role) const {
    LogEntry const &entry = entries_[index.row()];
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0:
                return entry.dateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
            case 1:
                return LogEntry::levelToString(entry.level());
            case 2:
                return entry.package();
            case 3:
                return entry.message();
            case 4:
                return entry.fieldsString();
            default:
                return {};
        }
    }
    if (role == Qt::ForegroundRole) {
        return LogEntry::levelColor(entry.level());
    }
    if (role == Qt::BackgroundRole) {
        return QColor("#2b2d30");
    }
    return {};
}

//****************************************************************************************************************************************************
/// \param[in] section The section (i.e. index of the row of column)
/// \param[in] orientation The orientation of the header.
/// \param[in] role The role of the data to retrieve.
/// \return The data for the header.
//****************************************************************************************************************************************************
QVariant Log::headerData(int section, Qt::Orientation orientation, int role) const {
    if ((role != Qt::DisplayRole) || (Qt::Horizontal != orientation)) {
        return QAbstractItemModel::headerData(section, orientation, role);
    }
    switch (section) {
        case 0:
            return tr("Time");
        case 1:
            return tr("Level");
        case 2:
            return tr("Package");
        case 3:
            return tr("Message");
        case 4:
            return tr("Fields");
        default:
            return {};
    }
}


//****************************************************************************************************************************************************
/// \return A constant reference to the log entries.
//****************************************************************************************************************************************************
QList<LogEntry> const &Log::entries() const {
    return entries_;
}


//****************************************************************************************************************************************************
/// \return true iff the log is empty.
//****************************************************************************************************************************************************
bool Log::isEmpty() const {
    return entries_.isEmpty();
}


//****************************************************************************************************************************************************
/// \return the report.
//****************************************************************************************************************************************************
Report Log::generateReport() const {
    if (entries_.isEmpty()) {
        throw Exception("Empty log");
    }

    Report report;
    report.startDate = entries_.front().dateTime();
    report.endDate = entries_.back().dateTime();

    return report;
}


//****************************************************************************************************************************************************
/// \return true iff errors where encountered while parsing the log.
//****************************************************************************************************************************************************
bool Log::hasErrors() const {
    return !errors_.isEmpty();
}


//****************************************************************************************************************************************************
/// \return The description of the errors encountered while parsing.
//****************************************************************************************************************************************************
QStringList Log::errors() const {
    return errors_;
}


//****************************************************************************************************************************************************
/// \param[in] line A log line
//****************************************************************************************************************************************************
LogEntry::Format Log::getLogFormat(QString const &line) {
    if (line.startsWith("time=")) {
        return LogEntry::Format::Bridge_3_4_0;
    }

    if (line.contains(QRegularExpression(R"(^(PANI|FATA|ERRO|WARN|INFO|DEBU|TRAC)\[)"))) {
        return LogEntry::Format::BridgeGUI_3_4_0;
    }

    return LogEntry::Format::Unknown;
}


//****************************************************************************************************************************************************
/// \param[in] filePath The path of the file.
//****************************************************************************************************************************************************
void Log::appendFileContent(QString const &filePath) {
    try {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw Exception(QString("The file '%1' could not be opened.").arg(QDir::toNativeSeparators(filePath)));
        }

        if (file.atEnd()) {
            throw Exception(QString("The file '%1' is empty.").arg(QDir::toNativeSeparators(filePath)));
        }

        QString const line = QString::fromUtf8(file.readLine());
        LogEntry::Format format = Log::getLogFormat(line);
        if (format == LogEntry::Format::Unknown) {
            throw Exception(QString("The file '%1' is not of a known log format.").arg(QDir::toNativeSeparators(filePath)));
        }
        if ((format_ != LogEntry::Format::Unknown) && (format_ != format)) {
            throw Exception(QString("The file '%1' is not of the same format as the beginning of the log.").arg(QDir::toNativeSeparators(filePath)));
        }
        format_ = format;

        LogEntry entry(line, format_);
        int errCount = 0;
        do {
            if (entry.isValid()) {
                entries_.append(entry);
            } else {
                errors_.append(
                    QString("%1: Invalid log entry at line %2: %3").arg(
                        QFileInfo(filePath).fileName()).arg(entries_.count() + errCount + 1).arg(entry.error()
                    )
                );
                ++errCount;
            }
            entry = LogEntry(QString::fromUtf8(file.readLine()), format_);
        } while (!file.atEnd());
    } catch (Exception const &e) {
        errors_.append(e.message());
    }

}
