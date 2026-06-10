#include "TaskManager.h"
#include "SystemMonitor.h"
#include <iostream>

using namespace std;

int main() {
    // Create TaskManager object
    // SQLite database loads automatically inside the constructor
    TaskManager manager;

    // Create SystemMonitor object
    SystemMonitor monitor;

    int choice;

    do {
        cout << "\n===== C++ Smart Task Manager =====\n";
        cout << "1. Add Task\n";
        cout << "2. View Tasks\n";
        cout << "3. Mark Task Complete\n";
        cout << "4. Delete Task\n";
        cout << "5. Search Tasks\n";
        cout << "6. View System Info\n";
        cout << "7. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        // Clear input buffer before using getline
        cin.ignore();

        if (choice == 1) {
            string title, priority, dueDate, category;

            cout << "Enter task title: ";
            getline(cin, title);

            cout << "Enter priority Low/Medium/High: ";
            getline(cin, priority);

            cout << "Enter due date YYYY-MM-DD: ";
            getline(cin, dueDate);

            cout << "Enter category School/Work/Personal: ";
            getline(cin, category);

            manager.addTask(title, priority, dueDate, category);
        }
        else if (choice == 2) {
            manager.viewTasks();
        }
        else if (choice == 3) {
            int id;

            cout << "Enter task ID to complete: ";
            cin >> id;

            manager.completeTask(id);
        }
        else if (choice == 4) {
            int id;

            cout << "Enter task ID to delete: ";
            cin >> id;

            manager.deleteTask(id);
        }
        else if (choice == 5) {
            string keyword;

            cout << "Enter search keyword: ";
            getline(cin, keyword);

            manager.searchTasks(keyword);
        }
        else if (choice == 6) {
            monitor.displaySystemInfo();
        }
        else if (choice == 7) {
            cout << "Goodbye. Database saved automatically.\n";
        }
        else {
            cout << "Invalid option. Try again.\n";
        }

    } while (choice != 7);

    return 0;
}