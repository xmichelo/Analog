cmake_minimum_required(VERSION 3.26)

set(CMAKE_OSX_DEPLOYMENT_TARGET 11.0)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")

project(Analog)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED)

qt_standard_project_setup()

qt_add_executable(Analog main.cpp
    AnalogApp.cpp
    AnalogApp.h
    Exception.cpp
    Exception.h
    FilenameInfo.cpp
    FilenameInfo.h
    Log.cpp
    Log.h
    LogEntry.cpp
    LogEntry.h
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
    FilterModel.cpp
    FilterModel.h
    Report.cpp
    Report.h
    ReportDialog.cpp
    ReportDialog.h
    ReportDialog.ui
    Session.cpp
    Session.h
    SessionList.cpp
    SessionList.h
    SessionWidget.cpp
    SessionWidget.h
    SessionWidget.ui
)

target_link_libraries(Analog PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

target_precompile_headers(Analog PRIVATE PCH.h)
target_include_directories(Analog PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# macOS specific settings
if (APPLE)
    set_target_properties(Analog PROPERTIES MACOSX_BUNDLE TRUE)
    set(MACOSX_BUNDLE_ICON_FILE Bridge.icns)
    set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Bridge.icns")
    set_source_files_properties(${app_icon_macos} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")
    target_sources(Analog PUBLIC ${app_icon_macos})
endif (APPLE)

# Windows specific settings
if (WIN32)
    set_target_properties(Analog PROPERTIES WIN32_EXECUTABLE TRUE)
endif (WIN32)