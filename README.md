# CppTaskMonitor

A modern C++ task management application featuring SQLite database integration, search functionality, and object-oriented design.

---

## Features

* Add tasks
* View tasks
* Delete tasks
* Mark tasks as completed
* Search tasks
* Task categories
* Due dates
* SQLite database storage
* Persistent data saving
* Multi-file C++ architecture

---

## Technologies Used

* C++
* SQLite3
* CMake
* vcpkg
* Visual Studio Build Tools

---

## Project Structure

```bash
CppTaskMonitor/
│
├── src/
│   ├── main.cpp
│   ├── Task.cpp
│   ├── Task.h
│   ├── TaskManager.cpp
│   └── TaskManager.h
│
├── data/
│   └── tasks.db
│
├── CMakeLists.txt
└── README.md
```

---

## Build Instructions

### 1. Install Dependencies

Install SQLite using vcpkg:

```powershell
vcpkg install sqlite3:x64-windows
vcpkg integrate install
```

---

### 2. Build Project

```powershell
mkdir build
cd build

cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

cmake --build .
```

---

### 3. Run Application

```powershell
.\Debug\CppTaskMonitor.exe
```

---

## Example Features

### Add Task

* Task title
* Priority
* Due date
* Category

### Search Tasks

Search by:

* title
* priority
* category
* due date

---

## Future Improvements

* CPU usage monitoring
* RAM monitoring
* GUI desktop interface with Qt
* Task statistics dashboard
* Sorting by due date and priority
* Cloud synchronization

---

## Author

Fadhil Al Salihi
