cmake_minimum_required(VERSION 3.26)
project(Analog)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED)

add_executable(Analog main.cpp
    AnalogApp.cpp
    AnalogApp.h
    Exception.cpp
    Exception.h
    Log.cpp
    Log.h
    LogEntry.cpp
    LogEntry.h
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
)

target_precompile_headers(Analog PRIVATE PCH.h)

target_link_libraries(Analog
    Qt::Core
    Qt::Gui
    Qt::Widgets
)
