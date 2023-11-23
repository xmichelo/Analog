/// \file
/// \author Xavier Michelon
///
/// \brief Main window class.


#ifndef ANALOG_MAIN_WINDOW_H
#define ANALOG_MAIN_WINDOW_H


#include "Log.h"
#include "FilterModel.h"
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
    MainWindow &operator=(MainWindow const &) = delete; ///< Disabled assignment operator.
    MainWindow &operator=(MainWindow &&) = delete; ///< Disabled move assignment operator.

private: // members functions
    bool validateFilesForOpening(QStringList &filePaths); ///< Validate the files paths before opening.

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
    ///\}

    void onTextFilterChanged(QString const &value); ///< Slot for the change of the text filter edit.
    void onPackageFilterChanged(QString const &value); ///< Slot for the change of the packet filter edit.
    void onLevelComboChanged(int index); ///< Slot the the change of the level combo.
    void onLevelStrictnessChanged(bool nonStrict); ///< Slot for the change of the level strictness check.
    void open(QStringList const &filePaths); ///< Slot for opening a given file.
    void onLogLoaded(); ///< Slot for the loading of a log.
    void onLayoutChanged(); ///< Slot for the changing of the filtering.

private: // data members.
    Ui::MainWindow ui_{}; ///< The GUI for the window.
    Log log_; ///< The log
    FilterModel filter_; ///< The filter model for the log.
};


#endif //ANALOG_MAIN_WINDOW_H
