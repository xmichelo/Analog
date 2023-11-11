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
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
    Log.cpp
    Log.h
    Exception.cpp
    Exception.h)
target_link_libraries(Analog
    Qt::Core
    Qt::Gui
    Qt::Widgets
)
