/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of program entry-point.


#include "MainWindow.h"
#include <QtWidgets>


//****************************************************************************************************************************************************
/// \param[in] argc The number of command-line arguments.
/// \param[in] argv The list of command-line arguments.
//****************************************************************************************************************************************************
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
