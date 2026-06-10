#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Task.h"
#include <vector>
#include <sqlite3.h>

using namespace std;

// TaskManager handles all task/database operations
class TaskManager {
private:
    vector<Task> tasks;
    int nextId;

    // SQLite database connection
    sqlite3* db;

public:
    // Constructor and destructor
    TaskManager();
    ~TaskManager();

    // Database setup
    void initializeDatabase();

    // Task operations
    void addTask(string title, string priority, string dueDate, string category);
    void viewTasks() const;
    void completeTask(int id);
    void deleteTask(int id);
    void searchTasks(string keyword) const;

    // Database operations
    void saveTaskToDatabase(const Task& task);
    void loadTasksFromDatabase();
};

#endif