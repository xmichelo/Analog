/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of a session widget class.


#ifndef ANALOG_SESSION_WIDGET_H
#define ANALOG_SESSION_WIDGET_H


#include "ui_SessionWidget.h"
#include "FilterModel.h"


//****************************************************************************************************************************************************
/// \brief Session widget class.
//****************************************************************************************************************************************************
class SessionWidget: public QWidget {
    Q_OBJECT
public: // member functions.
    explicit SessionWidget(QWidget *parent = nullptr); ///< Default constructor.
    SessionWidget(SessionWidget const &) = delete; ///< Disabled copy-constructor.
    SessionWidget(SessionWidget &&) = delete; ///< Disabled assignment copy-constructor.
    ~SessionWidget() override = default; ///< Destructor.
    SessionWidget& operator=(SessionWidget const &) = delete; ///< Disabled assignment operator.
    SessionWidget& operator=(SessionWidget &&) = delete; ///< Disabled move assignment operator.

    void setLog(SPLog const& log); ///< Set the log

public slots:
    void onTextFilterChanged(QString const &value); ///< Slot for the change of the text filter edit.
    void onPackageFilterChanged(QString const &value); ///< Slot for the change of the packet filter edit.
    void onLevelComboChanged(int index); ///< Slot the the change of the level combo.
    void onLevelStrictnessChanged(bool nonStrict); ///< Slot for the change of the level strictness check.
    void onLogLoaded(); ///< Slot for the loading of a log.
    void onLayoutChanged(); ///< Slot for the changing of the filtering.

signals:
    void logStatusMessageChanged(QString const &statusMessages); ///< emit a signal for change of the log status message.

private:
    Ui::SessionWidget ui_ {}; ///< The UI for the widget.
    FilterModel filter_; ///< The filter model for the log.
};


#endif // ANALOG_SESSION_WIDGET_H
