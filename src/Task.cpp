#include "Task.h"

// Constructor initializes all task data
Task::Task(int id, string title, string priority, string dueDate, string category, bool completed) {
    this->id = id;
    this->title = title;
    this->priority = priority;
    this->dueDate = dueDate;
    this->category = category;
    this->completed = completed;
}

// Return task ID
int Task::getId() const {
    return id;
}

// Return task title
string Task::getTitle() const {
    return title;
}

// Return task priority
string Task::getPriority() const {
    return priority;
}

// Return task due date
string Task::getDueDate() const {
    return dueDate;
}

// Return task category
string Task::getCategory() const {
    return category;
}

// Return true if task is completed
bool Task::isCompleted() const {
    return completed;
}

// Change task status to completed
void Task::markCompleted() {
    completed = true;
}

// Save format: id|title|priority|dueDate|category|completed
string Task::toFileString() const {
    return to_string(id) + "|" + title + "|" + priority + "|" + dueDate + "|" + category + "|" + to_string(completed);
}