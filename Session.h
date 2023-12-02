/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of session class.


#ifndef ANALOG_SESSION_H
#define ANALOG_SESSION_H


//****************************************************************************************************************************************************
/// \brief Session class
//****************************************************************************************************************************************************
class Session {
public: // member functions.
    Session(QDir const &dir, QStringList const &filenames); ///< Default constructor.
    Session(Session const &) = delete; ///< Disabled copy-constructor.
    Session(Session &&) = delete; ///< Disabled assignment copy-constructor.
    ~Session() = default; ///< Destructor.
    Session& operator=(Session const &) = delete; ///< Disabled assignment operator.
    Session& operator=(Session &&) = delete; ///< Disabled move assignment operator.

private: // data members.
    QDir dir_; ///< The folder containing the session.
    QString sessionID_; ///< The sessionID.
    QStringList guiFiles_; ///< The GUI log files.
    QStringList launcherFiles_; ///< The launcher log files.
    QStringList bridgeFiles_; ///< The bridge log files.
};


#endif // ANALOG_SESSION_H
