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
    : QAbstractItemModel(nullptr) {
    this->open(filePaths);
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
/// \param[in] index The index.
/// \return The log at the given index.
//****************************************************************************************************************************************************
SPLog SessionList::log(QModelIndex const &index) const {
    Session const &session = this->session(index);
    int const i = index.parent().isValid() ? index.row() : 0;
    bool const hasBridgeLog = session.hasBridgeLog();
    bool const hasGUILog = session.hasGUILog();

    switch (i) {
    case 0:
        if (hasBridgeLog) {
            return session.bridgeLog();
        }
        return hasGUILog ? session.guiLog() : session.launcherLog();
    case 1:
        if (hasBridgeLog) {
            return hasGUILog ? session.guiLog() : session.launcherLog();
        }
        return session.launcherLog();
    case 2:
        return session.launcherLog();
    default:
        return SPLog {};
    }
}

//****************************************************************************************************************************************************
/// \return an optional reference to the selected session.
//****************************************************************************************************************************************************
Session const& SessionList::session(QModelIndex const &index) const {
    QModelIndex const parent = index.parent();
    return sessions_[parent.isValid() ? parent.row() : index.row()];
}


//****************************************************************************************************************************************************
/// \param[in] row The row.
/// \param[in] column The column.
/// \param[in] parent The parent.
/// \return The model index of the item.
//****************************************************************************************************************************************************
QModelIndex SessionList::index(int row, int column, QModelIndex const &parent) const {
    if (parent.isValid()) {
        return createIndex(row, column, static_cast<quintptr>(parent.row()));
    }
    return createIndex(row, column, -1);
}


//****************************************************************************************************************************************************
/// \param[in] child the model index of the child.
/// \return The model index of the parent.
//****************************************************************************************************************************************************
QModelIndex SessionList::parent(QModelIndex const &child) const {
    if (!child.isValid()) {
        return {};
    }

    int const parentID = static_cast<int>(child.internalId());
    if (parentID == -1) {
        return {};
    }

    return createIndex(parentID, 0, -1);
}


//****************************************************************************************************************************************************
/// \param[in] parent The parent item index.
/// \return The number of rows.
//****************************************************************************************************************************************************
int SessionList::rowCount(QModelIndex const &parent) const {
    if (!parent.isValid()) {
        return static_cast<int>(sessions_.count());
    }

    if (!parent.parent().isValid()) {
        Session const &session = sessions_[parent.row()];
        return (session.hasBridgeLog() ? 1 : 0) + (session.hasGUILog() ? 1 : 0) + (session.hasLauncherLog() ? 1 : 0);
    }

    return 0;
}


//****************************************************************************************************************************************************
/// \param[in] parent The parent item index.
/// \return 1
//****************************************************************************************************************************************************
int SessionList::columnCount(QModelIndex const &parent) const {
    return 1;
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
    QModelIndex const parent = index.parent();
    if (parent.isValid()) {
        Session const &session = sessions_[parent.row()];
        bool const hasBridgeLog = session.hasBridgeLog();
        bool const hasGUILog = session.hasGUILog();

        switch (index.row()) {
        case 0:
            if (hasBridgeLog) {
                return bridgeStr;
            }
            return hasGUILog ? guiStr : launcherStr;
        case 1:
            if (hasBridgeLog) {
                return hasGUILog ? guiStr : launcherStr;
            }
            return launcherStr;
        case 2:
            return launcherStr;
        default:
            return {};
        }
    }

    return sessions_[index.row()].dateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
}
