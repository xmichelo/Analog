#include "Log.h"
#include "Exception.h"


//****************************************************************************************************************************************************
/// \param[in] filePath The path of the log file.
//****************************************************************************************************************************************************
void Log::open(QString const &filePath) {
    this->open(QStringList{filePath});
}


//****************************************************************************************************************************************************
/// \param[in] filePaths The ordered list of files forming the log.
//****************************************************************************************************************************************************
void Log::open(QStringList const &filePaths) {
    this->beginResetModel();
    entries_.clear();
    for (QString const &filePath: filePaths) {
        this->appendFileContent(filePath);
    }
    this->endResetModel();
}


//****************************************************************************************************************************************************
/// \param[in] filePath The path of the file.
//****************************************************************************************************************************************************
void Log::appendFileContent(QString const &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw Exception(QString("The file '%1' could not be opened.").arg(QDir::toNativeSeparators(filePath)));
    }

    qint64 lineNumber = 0;
    while (!file.atEnd()) {
        LogEntry const entry(QString::fromUtf8(file.readLine()));
        lineNumber++;
        if (entry.isValid()) {
            entries_.append(entry);
        } else {
            qWarning() << QString("Invalid log entry at line %1: %2").arg(lineNumber).arg(entry.error());
        }
    }
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
            return entry.time();
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
QList<LogEntry> const &Log::entries() {
    return entries_;
}
