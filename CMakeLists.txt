cmake_minimum_required(VERSION 3.10)

project(CppTaskMonitor)

set(CMAKE_CXX_STANDARD 17)

find_package(unofficial-sqlite3 CONFIG REQUIRED)

add_executable(CppTaskMonitor
    src/main.cpp
    src/Task.cpp
    src/TaskManager.cpp
)

target_link_libraries(CppTaskMonitor PRIVATE unofficial::sqlite3::sqlite3)