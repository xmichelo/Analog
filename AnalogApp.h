/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of custom Qt application class


#ifndef ANALOG_ANALOG_APP_H
#define ANALOG_ANALOG_APP_H


//****************************************************************************************************************************************************
/// \brief Custom Qt application class.
//****************************************************************************************************************************************************
class AnalogApp: public QApplication {
    Q_OBJECT
public: // member functions.
    AnalogApp(int &argc, char *argv[]); ///< Default constructor.
    AnalogApp(AnalogApp const&) = delete; ///< Disabled copy-constructor.
    AnalogApp(AnalogApp&&) = delete; ///< Disabled assignment copy-constructor.
    ~AnalogApp() override = default; ///< Destructor.
    AnalogApp& operator=(AnalogApp const&) = delete; ///< Disabled assignment operator.
    AnalogApp& operator=(AnalogApp&&) = delete; ///< Disabled move assignment operator.
    bool notify(QObject *object, QEvent *event) override;
};


#endif //ANALOG_ANALOG_APP_H
