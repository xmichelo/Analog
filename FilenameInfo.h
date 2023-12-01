/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of filename info structure.


#ifndef ANALOG_FILE_NAME_INFO_H
#define ANALOG_FILE_NAME_INFO_H


//****************************************************************************************************************************************************
/// \brief A structure holding data contained in the log file name
//****************************************************************************************************************************************************
struct FilenameInfo {
    enum class Executable {
        Bridge,
        BridgeGUI,
        Launcher
    }; ///< Enumeration for b

    QString sessionID; ///< The sessionID.
    Executable executable; ///< The executable.
    int fileIndex { 0 }; ///< The zero-based file index.
    QString version; ///< The version.
    QString tag; ///< The tag.

    static std::optional<FilenameInfo> parseFilename(QString const &filename); ///< Parse a log filename.
};


#endif //ANALOG_FILE_NAME_INFO_H
