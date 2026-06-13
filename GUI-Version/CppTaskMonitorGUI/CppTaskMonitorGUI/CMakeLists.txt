cmake_minimum_required(VERSION 3.19)
project(CppTaskMonitorGUI LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
qt_standard_project_setup()

qt_add_executable(CppTaskMonitorGUI
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    addtaskdialog.h addtaskdialog.cpp addtaskdialog.ui
)

target_link_libraries(CppTaskMonitorGUI
    PRIVATE
        Qt::Core
        Qt::Widgets
        unofficial::sqlite3::sqlite3
)

install(TARGETS CppTaskMonitorGUI
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET CppTaskMonitorGUI
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
