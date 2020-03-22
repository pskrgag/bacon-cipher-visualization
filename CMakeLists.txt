cmake_minimum_required(VERSION 2.8)

project(bacon)
add_executable(${PROJECT_NAME} main.cpp mainwindow.cpp mainwindow.h mainwindow.ui)
set_target_properties(bacon PROPERTIES CXX_STANDARD 20)
set_target_properties(bacon  PROPERTIES AUTOMOC ON AUTOUIC ON)
find_package(Qt5 COMPONENTS Gui Core Widgets REQUIRED)
target_link_libraries(bacon  Qt5::Core Qt5::Widgets Qt5::Gui)
