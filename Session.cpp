/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of session class.


#include "Session.h"
#include "Exception.h"
#include "FilenameInfo.h"


//****************************************************************************************************************************************************
/// \param[in] dir The folder containing the session.
/// \param[in] filenames The name of the session files.
//****************************************************************************************************************************************************
Session::Session(QDir const &dir, QStringList const &filenames)
    : dir_(dir) {
    if (filenames.isEmpty()) {
        throw Exception("Session cannot be empty");
    }
    QStringList files = filenames;
    files.sort();

    for (QString const &filename: files) {
        std::optional<FilenameInfo> const info = FilenameInfo::parseFilename(filename);
        if (!info) {
            throw Exception(QString("File '%1' is does not have a valid log file name.").arg(filename));
        }

        if (sessionID_.isEmpty()) {
            sessionID_ = info->sessionID;
        } else {
            if (sessionID_ != info->sessionID) {
                throw Exception(QString("File '%1' does not belong to session %2.").arg(filename, sessionID_));
            }
        }

        QString const path = dir.absoluteFilePath(filename);
        if (!QFile(path).exists()) {
            throw Exception(QString("The file '%1' does not exist.").arg(path));
        }

        switch (info->executable) {
        case FilenameInfo::Executable::Bridge:
            if (bridgeFiles_.count() != info->fileIndex) {
                throw Exception(QString("At least one Bridge log file is missing for session %1").arg(sessionID_));
            }
            bridgeFiles_.append(filename);
            break;
        case FilenameInfo::Executable::BridgeGUI:
            if (guiFiles_.count() != info->fileIndex) {
                throw Exception(QString("At least one GUI log file is missing for session %1").arg(sessionID_));
            }
            guiFiles_.append(filename);
            break;
        case FilenameInfo::Executable::Launcher:
            if (launcherFiles_.count() != info->fileIndex) {
                throw Exception(QString("At least one Launcher log file is missing for session %1").arg(sessionID_));
            }
            launcherFiles_.append(filename);
            break;
        }
    }
}


//****************************************************************************************************************************************************
/// \return The sessionID
//****************************************************************************************************************************************************
QString Session::sessionID() const {
    return sessionID_;
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
QDateTime Session::dateTime() const {
    return QDateTime::fromString(sessionID_, "yyyyMMdd_HHmmsszzz");
}


//****************************************************************************************************************************************************
/// \return true iff the session contains a bridge log file.
//****************************************************************************************************************************************************
bool Session::hasBridgeLog() const {
    return !bridgeFiles_.isEmpty();
}

//****************************************************************************************************************************************************
/// \return true iff the session contains a bridge-gui log file.
//****************************************************************************************************************************************************
bool Session::hasGUILog() const {
    return !guiFiles_.isEmpty();
}

//****************************************************************************************************************************************************
/// \return true iff the session contains a launcher log file.
//****************************************************************************************************************************************************
bool Session::hasLauncherLog() const {
    return !launcherFiles_.isEmpty();
}
