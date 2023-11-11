/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of exception class.



#ifndef ANALOG_EXCEPTION_H
#define ANALOG_EXCEPTION_H


#include <stdexcept>


//****************************************************************************************************************************************************
/// \brief Exception class.
///
/// The class does not inherit from std::exception to avoid having to deal with C-style string and their lifetime.
//****************************************************************************************************************************************************
class Exception: std::exception {
public: // member functions.
    explicit Exception(QString message); ///< Default constructor.
    Exception(Exception const&) = delete; ///< Disabled copy-constructor.
    Exception(Exception&&) = delete; ///< Disabled assignment copy-constructor.
    ~Exception() override = default; ///< Destructor.
    Exception& operator=(Exception const&) = delete; ///< Disabled assignment operator.
    Exception& operator=(Exception&&) = delete; ///< Disabled move assignment operator.
    QString message() const; ///< Return the exception description as a QString.
    char const *what() const noexcept override; ///< Implementation of required what() function  from std::exception.

private: // data members
    QString const message_; ///< The description of the exception.
    mutable QByteArray what_; ///< The cached C-style string version of the exception description.
};



#endif //ANALOG_EXCEPTION_H
