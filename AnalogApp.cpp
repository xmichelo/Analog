/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of custom Qt application class


#include "AnalogApp.h"
#include "Exception.h"


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
AnalogApp::AnalogApp(int &argc, char *argv[])
    : QApplication(argc, argv) {
}


//****************************************************************************************************************************************************
//
//****************************************************************************************************************************************************
bool AnalogApp::notify(QObject *object, QEvent *event) {
    try {
        return QApplication::notify(object, event);
    } catch (Exception const &e) {
        QMessageBox::critical(nullptr, "Error", e.message());
        qCritical() << e.message() << "\n";
    } catch (std::exception const& e) {
        qCritical() << QString::fromLocal8Bit(e.what()) << "\n";
    } catch (...) {
        qCritical() << "A fatal error occurred.\n";
    }
    return false;
}
