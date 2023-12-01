/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of filename info structure.


#include "FilenameInfo.h"
#include "Exception.h"


namespace {
QRegularExpression rx(R"(^(?<sessionID>\d{8}_\d{9})_(?<exe>lau|bri|gui)_(?<fileIndex>\d{3,})_v(?<version>.*)_(?<tag>.*)\.log$)"); ///< The regular expression for log file names.
}


//****************************************************************************************************************************************************
/// \param[in] exe The 3-letter executable identifier string.
/// \return The executable identifier.
//****************************************************************************************************************************************************
FilenameInfo::Executable parseExecutableIdentifier(QString const &exe) {
    if (exe.compare("bri", Qt::CaseInsensitive) == 0) {
        return FilenameInfo::Executable::Bridge;
    }
    if (exe.compare("gui", Qt::CaseInsensitive) == 0) {
        return FilenameInfo::Executable::BridgeGUI;
    }
    if (exe.compare("lau", Qt::CaseInsensitive) == 0) {
        return FilenameInfo::Executable::Launcher;
    }

    throw Exception(QString("Unexpected executable identifier found in file name: %1").arg(exe));
}


//****************************************************************************************************************************************************
/// \param[in] filename The filename.
/// \return an optional FileNameInfo containing the info parsed from the filename if it was correctly formed.
//****************************************************************************************************************************************************
std::optional<FilenameInfo> FilenameInfo::parseFilename(QString const &filename) {
    QRegularExpressionMatch match = rx.match(filename);
    if (!match.hasMatch())
        return {};
    return FilenameInfo{
        .sessionID = match.captured("sessionID"),
        .executable = parseExecutableIdentifier(match.captured("exe")),
        .fileIndex = QString(match.captured("fileIndex")).toInt(),
        .version = match.captured("version"),
        .tag = match.captured("tag"),
    };
}
