/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of program entry-point.


#include "MainWindow.h"
#include "AnalogApp.h"
#include "Exception.h"


//****************************************************************************************************************************************************
/// \param[in] argc The number of command-line arguments.
/// \param[in] argv The list of command-line arguments.
//****************************************************************************************************************************************************
int main(int argc, char *argv[]) {
    AnalogApp a(argc, argv);
    MainWindow w;
    try {
        w.show();
        if (argc > 1) {
            QTimer::singleShot(0, &w, [argv, &w]() { w.openFile(QString(argv[1])); });
        }
        return QApplication::exec();
    } catch (Exception const &e) {
        qCritical() << e.message() << "\n";
    } catch (std::exception const& e) {
        qCritical() << e.what() << "\n";
    } catch (...) {
        qCritical() << "A fatal error occurred.\n";
    }
    return EXIT_FAILURE;
}
