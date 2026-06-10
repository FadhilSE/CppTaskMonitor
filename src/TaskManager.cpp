#include "TaskManager.h"
#include <iostream>
#include <sstream>

using namespace std;

// Constructor
TaskManager::TaskManager() {
    nextId = 1;

    // Open SQLite database
    if (sqlite3_open("data/tasks.db", &db)) {
        cerr << "Error opening database.\n";
    }
    else {
        cout << "Database connected successfully.\n";
    }

    initializeDatabase();
    loadTasksFromDatabase();
}

// Destructor closes database connection
TaskManager::~TaskManager() {
    sqlite3_close(db);
}

// Create tasks table if it does not exist
void TaskManager::initializeDatabase() {

    string sql =
        "CREATE TABLE IF NOT EXISTS tasks ("
        "id INTEGER PRIMARY KEY, "
        "title TEXT, "
        "priority TEXT, "
        "dueDate TEXT, "
        "category TEXT, "
        "completed INTEGER);";

    char* errorMessage = nullptr;

    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        cerr << "Database table creation failed: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
}

// Add new task
void TaskManager::addTask(string title, string priority, string dueDate, string category) {

    Task newTask(nextId, title, priority, dueDate, category);

    tasks.push_back(newTask);

    saveTaskToDatabase(newTask);

    nextId++;

    cout << "Task added successfully.\n";
}

// Save task into SQLite database
void TaskManager::saveTaskToDatabase(const Task& task) {

    string sql =
        "INSERT INTO tasks (id, title, priority, dueDate, category, completed) VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, task.getId());

    sqlite3_bind_text(stmt, 2, task.getTitle().c_str(), -1, SQLITE_STATIC);

    sqlite3_bind_text(stmt, 3, task.getPriority().c_str(), -1, SQLITE_STATIC);

    sqlite3_bind_text(stmt, 4, task.getDueDate().c_str(), -1, SQLITE_STATIC);

    sqlite3_bind_text(stmt, 5, task.getCategory().c_str(), -1, SQLITE_STATIC);

    sqlite3_bind_int(stmt, 6, task.isCompleted());

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);
}

// Load tasks from database
void TaskManager::loadTasksFromDatabase() {

    string sql = "SELECT * FROM tasks;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        int id = sqlite3_column_int(stmt, 0);

        string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        string priority = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        string dueDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        bool completed = sqlite3_column_int(stmt, 5);

        tasks.push_back(Task(id, title, priority, dueDate, category, completed));

        if (id >= nextId) {
            nextId = id + 1;
        }
    }

    sqlite3_finalize(stmt);
}

// Display all tasks
void TaskManager::viewTasks() const {

    if (tasks.empty()) {
        cout << "No tasks found.\n";
        return;
    }

    cout << "\n===== All Tasks =====\n";

    for (const Task& task : tasks) {

        cout << "ID: " << task.getId()
             << " | Title: " << task.getTitle()
             << " | Priority: " << task.getPriority()
             << " | Due: " << task.getDueDate()
             << " | Category: " << task.getCategory()
             << " | Status: "
             << (task.isCompleted() ? "Completed" : "Pending")
             << endl;
    }
}

// Mark task as completed
void TaskManager::completeTask(int id) {

    for (Task& task : tasks) {

        if (task.getId() == id) {

            task.markCompleted();

            string sql =
                "UPDATE tasks SET completed = 1 WHERE id = ?;";

            sqlite3_stmt* stmt;

            sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

            sqlite3_bind_int(stmt, 1, id);

            sqlite3_step(stmt);

            sqlite3_finalize(stmt);

            cout << "Task marked as completed.\n";

            return;
        }
    }

    cout << "Task not found.\n";
}

// Delete task
void TaskManager::deleteTask(int id) {

    for (auto it = tasks.begin(); it != tasks.end(); ++it) {

        if (it->getId() == id) {

            tasks.erase(it);

            string sql = "DELETE FROM tasks WHERE id = ?;";

            sqlite3_stmt* stmt;

            sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

            sqlite3_bind_int(stmt, 1, id);

            sqlite3_step(stmt);

            sqlite3_finalize(stmt);

            cout << "Task deleted successfully.\n";

            return;
        }
    }

    cout << "Task not found.\n";
}

// Search tasks
void TaskManager::searchTasks(string keyword) const {

    bool found = false;

    cout << "\n===== Search Results =====\n";

    for (const Task& task : tasks) {

        if (task.getTitle().find(keyword) != string::npos ||
            task.getPriority().find(keyword) != string::npos ||
            task.getDueDate().find(keyword) != string::npos ||
            task.getCategory().find(keyword) != string::npos) {

            cout << "ID: " << task.getId()
                 << " | Title: " << task.getTitle()
                 << " | Priority: " << task.getPriority()
                 << " | Due: " << task.getDueDate()
                 << " | Category: " << task.getCategory()
                 << " | Status: "
                 << (task.isCompleted() ? "Completed" : "Pending")
                 << endl;

            found = true;
        }
    }

    if (!found) {
        cout << "No matching tasks found.\n";
    }
}