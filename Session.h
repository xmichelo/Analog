/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of session class.


#ifndef ANALOG_SESSION_H
#define ANALOG_SESSION_H


#include "Log.h"

//****************************************************************************************************************************************************
/// \brief Session class
//****************************************************************************************************************************************************
class Session {
public: // member functions.
    Session(QDir const &dir, QStringList const &filenames); ///< Default constructor.
    Session(Session const &) = default; ///< Disabled copy-constructor.
    Session(Session &&) = default; ///< Disabled assignment copy-constructor.
    ~Session() = default; ///< Destructor.
    Session& operator=(Session const &) = default; ///< Disabled assignment operator.
    Session& operator=(Session &&) = default; ///< Disabled move assignment operator.
    QString sessionID() const; ///< Return the sessionID.
    QDateTime dateTime() const; ///< Return the session date/time (based on the sessionID).
    bool hasBridgeLog() const; ///< Checks if the session has a bridge log file.
    bool hasGUILog() const;  ///< Checks if the session has a bridge-gui log file.
    bool hasLauncherLog() const; ///< Checks if the session has a launcher log file.
    SPLog bridgeLog() const; ///< Returns the bridge log.
    SPLog guiLog() const; ///< Returns the bridge-gui log.
    SPLog launcherLog() const; ///< Returns the launcher log.
    QStringList bridgeFilePaths() const; ///< Return the full paths of the bridge log files
    QStringList guiFilePaths() const; ///< Return the full paths of the bridge-gui log files
    QStringList launcherFilePaths() const; ///< Return the full paths of the launcher log files

private: // data members.
    QDir dir_; ///< The folder containing the session.
    QString sessionID_; ///< The sessionID.
    QStringList bridgeFiles_; ///< The bridge log files.
    QStringList guiFiles_; ///< The GUI log files.
    QStringList launcherFiles_; ///< The launcher log files.
};


#endif // ANALOG_SESSION_H
