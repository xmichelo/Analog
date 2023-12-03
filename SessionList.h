/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of session list.


#ifndef ANALOG_SESSION_LIST_H
#define ANALOG_SESSION_LIST_H


#include "Session.h"
#include "Log.h"


//****************************************************************************************************************************************************
/// \brief Session list class.
//****************************************************************************************************************************************************
class SessionList : public QAbstractListModel {
    Q_OBJECT

public: // member functions.
    explicit SessionList(QStringList const &filePaths = {}); ///< Default constructor.
    SessionList(SessionList const &) = delete; ///< Disabled copy-constructor.
    SessionList(SessionList &&) = delete; ///< Disabled assignment copy-constructor.
    ~SessionList() override = default; ///< Destructor.
    SessionList& operator=(SessionList const &) = delete; ///< Disabled assignment operator.
    SessionList& operator=(SessionList &&) = delete; ///< Disabled move assignment operator.
    Session & operator[](int index);
    Session const& operator[](int index) const;
    void open(QStringList const &filePaths); ///< Open a session list.
    Session const & session(QModelIndex const &index) const; ///< Get an optional reference to the session at the given index.
    qsizetype count() const; ///< return the number of sessions.

    /// \name Tree view model functions.
    ///\{
    int rowCount(QModelIndex const &parent) const override;
    QVariant data(QModelIndex const &index, int role) const override;
    ///\}

private:
    QList<Session> sessions_;
};


#endif //  ANALOG_SESSION_LIST_H
