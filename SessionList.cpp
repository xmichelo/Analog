/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of session list.


#include "SessionList.h"
#include "Exception.h"
#include "FilenameInfo.h"

namespace {
QString const bridgeStr = "Bridge"; ///< The string for bridge.
QString const guiStr = "GUI"; ///< The string for bridge-gui.
QString const launcherStr = "Launcher"; ///< The string for launcher.
}

//****************************************************************************************************************************************************
/// \param[in] filePaths the list of file paths
//****************************************************************************************************************************************************
SessionList::SessionList(QStringList const &filePaths)
    : QAbstractListModel(nullptr) {
    this->open(filePaths);
}


//****************************************************************************************************************************************************
/// \param[in] index The index.
/// \return The session at the given index.
//****************************************************************************************************************************************************
Session& SessionList::operator[](int index) {
    return sessions_[index];
}


//****************************************************************************************************************************************************
/// \param[in] index The index.
/// \return The session at the given index.
//****************************************************************************************************************************************************
Session const& SessionList::operator[](int index) const {
    return sessions_[index];
}


//****************************************************************************************************************************************************
/// \param[in] filePaths The paths of the files to add the session.
//****************************************************************************************************************************************************
void SessionList::open(QStringList const &filePaths) {
    this->beginResetModel();
    sessions_.clear();
    if (filePaths.isEmpty()) {
        return;
    }
    QMap<QString, QStringList> sessionMap;
    QDir const dir(QFileInfo(filePaths.front()).canonicalPath());
    for (QString const &path: filePaths) {
        QFileInfo const info(path);
        QString const filename = info.fileName();
        if (QDir(QFileInfo(path).canonicalPath()) != dir) {
            throw Exception("Cannot open files from different directories.");
        }
        if (!info.exists()) {
            throw Exception(QString("The file '%1' does not exists.").arg(QDir::toNativeSeparators(info.canonicalFilePath())));
        }
        if (!info.isFile()) {
            throw Exception(QString("'%1' is a directory.").arg(QDir::toNativeSeparators(info.canonicalFilePath())));
        }
        std::optional<FilenameInfo> const filenameInfo = FilenameInfo::parseFilename(filename);
        if (!filenameInfo) {
            throw Exception(QString("'%1' is not a valid log file name.").arg(filename));
        }

        sessionMap[filenameInfo->sessionID].append(filename);
    }

    for (QStringList const &files: sessionMap) {
        sessions_.append(Session(dir, files));
    }
    this->endResetModel();
}


//****************************************************************************************************************************************************
/// \return an optional reference to the selected session.
//****************************************************************************************************************************************************
Session const& SessionList::session(QModelIndex const &index) const {
    QModelIndex const parent = index.parent();
    return sessions_[parent.isValid() ? parent.row() : index.row()];
}


//****************************************************************************************************************************************************
/// \return The number of sessions.
//****************************************************************************************************************************************************
qsizetype SessionList::count() const {
    return sessions_.count();
}


//****************************************************************************************************************************************************
/// \param[in] parent The parent item index.
/// \return The number of rows.
//****************************************************************************************************************************************************
int SessionList::rowCount(QModelIndex const &parent) const {
    if (!parent.isValid()) {
        return static_cast<int>(sessions_.count());
    }
    return 0;
}


//****************************************************************************************************************************************************
/// \param[in] index The model index.
/// \param[in] role The role to retrieve data for.
/// \return The data.
//****************************************************************************************************************************************************
QVariant SessionList::data(QModelIndex const &index, int role) const {
    if (role != Qt::DisplayRole) {
        return {};
    }
    Session const &session = sessions_[index.row()];
    QString result = session.dateTime().toString("yyyy-MM-dd HH:mm:ss.zzz - ");
    if (session.hasBridgeLog()) {
        result += "B";
    }
    if (session.hasGUILog()) {
        result += "G";
    }
    if (session.hasLauncherLog()) {
        result += "L";
    }

    return result;
}
