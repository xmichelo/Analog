/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of log class



#ifndef ANALOG_LOG_H
#define ANALOG_LOG_H


#include <QtCore>


//****************************************************************************************************************************************************
/// \brief Log class
//****************************************************************************************************************************************************
class Log: public QAbstractTableModel {
    Q_OBJECT
public: // member functions.
    Log() = default; ///< Default constructor.
    Log(Log const &) = delete; ///< Disabled copy-constructor.
    Log(Log &&) = delete; ///< Disabled assignment copy-constructor.
    ~Log() override = default; ///< Destructor.
    Log &operator=(Log const &) = delete; ///< Disabled assignment operator.
    Log &operator=(Log &&) = delete; ///< Disabled move assignment operator.

    void open(QString const &filePath); ///< Open a log from file.
    void open(QStringList const& filePaths); ///< Open a log from an ordered list of files.

    int rowCount(QModelIndex const &parent) const override;
    int columnCount(QModelIndex const &parent) const override;
    QVariant data(QModelIndex const &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private: // member functions
    void appendFileContent(QString const& filePath); ///< Append the content of a file to the log.

public: // data members
    QStringList entries_; ///< The log entries
};


#endif //ANALOG_LOG_H
