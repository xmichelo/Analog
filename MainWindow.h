/// \file
/// \author Xavier Michelon
///
/// \brief Main window class.


#ifndef ANALOG_MAIN_WINDOW_H
#define ANALOG_MAIN_WINDOW_H


#include "Log.h"
#include "SessionList.h"
#include "ui_MainWindow.h"


//****************************************************************************************************************************************************
/// \brief Main window class
//****************************************************************************************************************************************************
class MainWindow : public QMainWindow {
    Q_OBJECT

public: // member functions.
    MainWindow(); ///< Default constructor.
    MainWindow(MainWindow const &) = delete; ///< Disabled copy-constructor.
    MainWindow(MainWindow &&) = delete; ///< Disabled assignment copy-constructor.
    ~MainWindow() override = default; ///< Destructor.
    MainWindow& operator=(MainWindow const &) = delete; ///< Disabled assignment operator.
    MainWindow& operator=(MainWindow &&) = delete; ///< Disabled move assignment operator.

private: // members functions
    /// \name Drag and drop functions
    ///\{
    void dragEnterEvent(QDragEnterEvent *event) override; ///< Drag enter event handler
    void dragMoveEvent(QDragMoveEvent *event) override; ///< Drag move event handler
    void dragLeaveEvent(QDragLeaveEvent *event) override; ///< Drag leave event handler
    void dropEvent(QDropEvent *event) override; ///< Drop event handler
    ///\}

public slots:
    /// \name Actions
    ///\{
    void onActionOpenFile(); ///< Slot for the 'Open File' action.
    void onActionShowReport(); ///< Slot for the 'Show Report' action.
    void onAbout(); ///< Slot for showing the about dialog.
    ///\}

    void open(QStringList const &filePaths); ///< Slot for opening a given file.
    void onSelectedSessionChanged(const QItemSelection &selected, const QItemSelection &deselected) const; ///< Slot for the selection of a session.
    void onLogStatusMessageChanged(QString const &message) const; ///< Slot for the change of the log status message.
    void onLogErrors(QStringList const &errors); ///< Slot for errors opening log.

private: // data members.
    Ui::MainWindow ui_ {}; ///< The GUI for the window.
    SessionList sessionList_; ///< The session list.
    SPLog log_; ///< The log
};


#endif //ANALOG_MAIN_WINDOW_H
