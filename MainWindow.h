/// \file
/// \author Xavier Michelon
///
/// \brief Main window class.


#ifndef ANALOG_MAIN_WINDOW_H
#define ANALOG_MAIN_WINDOW_H


#include "Log.h"
#include "ui_MainWindow.h"


//****************************************************************************************************************************************************
/// \brief Main window class
//****************************************************************************************************************************************************
class MainWindow: public QMainWindow {
    Q_OBJECT

public: // member functions.
    MainWindow(); ///< Default constructor.
    MainWindow(MainWindow const&) = delete; ///< Disabled copy-constructor.
    MainWindow(MainWindow&&) = delete; ///< Disabled assignment copy-constructor.
    ~MainWindow() override = default; ///< Destructor.
    MainWindow& operator=(MainWindow const&) = delete; ///< Disabled assignment operator.
    MainWindow& operator=(MainWindow&&) = delete; ///< Disabled move assignment operator.

private slots:
    void onActionOpenFile(); ///< Slot for the 'Open File' action.

private: // data members.
    Ui::MainWindow ui_ {}; ///< The GUI for the window.
    Log log_; ///< The log
};



#endif //ANALOG_MAIN_WINDOW_H
