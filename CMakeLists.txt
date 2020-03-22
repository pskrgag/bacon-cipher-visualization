cmake_minimum_required(VERSION 2.8)

project(crypto)
add_executable(${PROJECT_NAME} "main.cpp" mainwindow.cpp mainwindow.h mainwindow.ui)
set_target_properties(crypto PROPERTIES AUTOMOC ON AUTOUIC ON)
find_package(Qt5 COMPONENTS Gui Core Widgets REQUIRED)
target_link_libraries(crypto Qt5::Core Qt5::Widgets Qt5::Gui)
