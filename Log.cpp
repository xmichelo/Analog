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
        throw Exception(QString("The file '%1' could not be opened.\n").arg(QDir::toNativeSeparators(filePath)));
    }

    while (!file.atEnd()) {
        QString const line = QString::fromUtf8(file.readLine());
        if (!line.isEmpty())
            entries_.append(line);
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
    return 1;
}


//****************************************************************************************************************************************************
/// \param[in] index The index of the data to retrieve.
/// \param[in] role The role of the data to retrieve.
/// \return The data for a given role at a model index.
//****************************************************************************************************************************************************
QVariant Log::data(QModelIndex const &index, int role) const {
    if (role != Qt::DisplayRole) {
        return {};
    }
    return entries_[index.row()];
}

//****************************************************************************************************************************************************
/// \param[in] section The section (i.e. index of the row of column)
/// \param[in] orientation The orientation of the header.
/// \param[in] role The role of the data to retrieve.
/// \return The data for the header.
//****************************************************************************************************************************************************
QVariant Log::headerData(int section, Qt::Orientation orientation, int role) const {
    if ((role == Qt::DisplayRole) && (Qt::Horizontal == orientation)) {
        return tr("Entry");
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}
