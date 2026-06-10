#ifndef TASK_H
#define TASK_H

#include <string>
using namespace std;

// Task class represents one task in the application
class Task {
private:
    int id;              // Unique task ID
    string title;        // Task title/name
    string priority;     // Task priority: Low, Medium, High
    string dueDate;      // Task due date
    string category;     // Task category: School, Work, Personal
    bool completed;      // Task status: completed or pending

public:
    // Constructor to create a task object
    Task(int id, string title, string priority, string dueDate, string category, bool completed = false);

    // Getter functions
    int getId() const;
    string getTitle() const;
    string getPriority() const;
    string getDueDate() const;
    string getCategory() const;
    bool isCompleted() const;

    // Mark task as completed
    void markCompleted();

    // Convert task data to text format for file saving
    string toFileString() const;
};

#endif