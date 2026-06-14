#include <windows.h>
#include <QInputDialog>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMessageBox>
#include <QCoreApplication>
#include <QColor>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDate>
#include "addtaskdialog.h"
#include <QAbstractItemView>
#include "dashboarddialog.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db(nullptr)
{
    ui->setupUi(this);
    QSettings settings("FadhilSE", "SmartTaskManager");
    darkModeEnabled = settings.value("darkModeEnabled", false).toBool();

    if (darkModeEnabled) {
        applyDarkTheme();
        ui->darkModeButton->setText("Light Mode");
    } else {
        applyLightTheme();
        ui->darkModeButton->setText("Dark Mode");
    }
    ui->taskTable->setSortingEnabled(true);
    //darkModeEnabled = false;
    ui->taskTable->horizontalHeader()->setStretchLastSection(true);
    ui->taskTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setWindowTitle("Smart Task Manager Pro");
   // applyLightTheme();
    //this->setStyleSheet(
      //  "QMainWindow { background-color: #f4f6f8; }"
        //"QLabel { font-size: 14px; font-weight: bold; }"
       // "QLabel#titleLabel { font-size: 22px; font-weight: bold; color: #1e293b; }"
        //"QPushButton { background-color: #2563eb; color: white; border-radius: 6px; "
        //"padding: 4px 10px; font-weight: bold; font-size: 10pt; min-width: 90px; min-height: 24px; }"
        //"QPushButton:hover { background-color: #1d4ed8; }"
        //"QLineEdit { border: 1px solid #cbd5e1; border-radius: 6px; padding: 6px; }"
        //"QTableWidget { background-color: white; border: 1px solid #cbd5e1; }"
      //  );


    // Hidden ID column + visible task columns
    ui->taskTable->setColumnCount(6);
    ui->taskTable->setHorizontalHeaderLabels({"ID", "Title", "Priority", "Due Date", "Category", "Status"});
    ui->taskTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->taskTable->horizontalHeader()->setStretchLastSection(true);
    ui->taskTable->verticalHeader()->setVisible(false);
    ui->taskTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->taskTable->setAlternatingRowColors(true);
    ui->taskTable->setSortingEnabled(true);
    ui->taskTable->setColumnWidth(0, 60);
    ui->taskTable->hideColumn(0);
    ui->taskTable->horizontalHeader()->setStretchLastSection(true);
    ui->taskTable->setAlternatingRowColors(true);
    ui->taskTable->horizontalHeader()->setStretchLastSection(true);
    ui->taskTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    initializeDatabase();
    loadTasksFromDatabase();

    connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::onAddTaskClicked);
    connect(ui->deleteTaskButton, &QPushButton::clicked, this, &MainWindow::onDeleteTaskClicked);
    connect(ui->completeTaskButton, &QPushButton::clicked, this, &MainWindow::onCompleteTaskClicked);
    connect(ui->systemMonitorButton, &QPushButton::clicked, this, &MainWindow::onSystemMonitorClicked);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    connect(ui->taskTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::onEditTaskClicked);
    connect(ui->dashboardButton, &QPushButton::clicked, this, &MainWindow::onDashboardClicked);
    connect(ui->exportCsvButton, &QPushButton::clicked, this, &MainWindow::onExportCsvClicked);
    connect(ui->darkModeButton, &QPushButton::clicked, this, &MainWindow::onDarkModeClicked);
    connect(ui->importCsvButton, &QPushButton::clicked, this, &MainWindow::onImportCsvClicked);
    connect(ui->dashboardButton, &QPushButton::clicked, this, &MainWindow::onDashboardClicked);
    updateStats();
}

MainWindow::~MainWindow()
{
    if (db) {
        sqlite3_close(db);
    }

    delete ui;
}

void MainWindow::initializeDatabase()
{
    QString dbPath = QCoreApplication::applicationDirPath() + "/tasks_gui.db";

    if (sqlite3_open(dbPath.toUtf8().constData(), &db) != SQLITE_OK) {
        QMessageBox::critical(this, "Database Error", "Could not open SQLite database.");
        return;
    }

    const char* sql =
        "CREATE TABLE IF NOT EXISTS tasks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT NOT NULL,"
        "priority TEXT NOT NULL,"
        "dueDate TEXT NOT NULL,"
        "category TEXT NOT NULL,"
        "status TEXT NOT NULL);";

    char* errorMessage = nullptr;

    if (sqlite3_exec(db, sql, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        QMessageBox::critical(this, "Database Error", errorMessage);
        sqlite3_free(errorMessage);
    }
}

void MainWindow::loadTasksFromDatabase()
{
    const char* sql = "SELECT id, title, priority, dueDate, category, status FROM tasks;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);

        QString title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        QString priority = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        QString dueDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        QString category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        QString status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

        addTaskToTable(id, title, priority, dueDate, category, status);
    }

    sqlite3_finalize(stmt);
}

void MainWindow::addTaskToTable(int id, const QString &title, const QString &priority,
                                const QString &dueDate, const QString &category, const QString &status)
{
    int row = ui->taskTable->rowCount();
    ui->taskTable->insertRow(row);

    ui->taskTable->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
    ui->taskTable->setItem(row, 1, new QTableWidgetItem(title));
    ui->taskTable->setItem(row, 2, new QTableWidgetItem(priority));
    ui->taskTable->setItem(row, 3, new QTableWidgetItem(dueDate));
    ui->taskTable->setItem(row, 4, new QTableWidgetItem(category));
    ui->taskTable->setItem(row, 5, new QTableWidgetItem(status));
    applyPriorityColor(row);
    applyStatusColor(row);
    applyDueDateColor(row);
}

int MainWindow::saveTaskToDatabase(const QString &title, const QString &priority,
                                   const QString &dueDate, const QString &category)
{
    const char* sql =
        "INSERT INTO tasks (title, priority, dueDate, category, status) "
        "VALUES (?, ?, ?, ?, 'Pending');";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return -1;
    }

    sqlite3_bind_text(stmt, 1, title.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, priority.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, dueDate.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, category.toUtf8().constData(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);

    return static_cast<int>(sqlite3_last_insert_rowid(db));
}

void MainWindow::deleteTaskFromDatabase(int id)
{
    const char* sql = "DELETE FROM tasks WHERE id = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void MainWindow::completeTaskInDatabase(int id)
{
    const char* sql = "UPDATE tasks SET status = 'Completed' WHERE id = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void MainWindow::onAddTaskClicked()
{
    AddTaskDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString title = dialog.getTitle();

        if (title.isEmpty()) {
            QMessageBox::warning(this, "Add Task", "Task title cannot be empty.");
            return;
        }

        QString priority = dialog.getPriority();
        QString dueDate = dialog.getDueDate();
        QString category = dialog.getCategory();

        int id = saveTaskToDatabase(title, priority, dueDate, category);

        if (id == -1) {
            QMessageBox::critical(this, "Database Error", "Task could not be saved.");
            return;
        }

        addTaskToTable(id, title, priority, dueDate, category, "Pending");
        updateStats();
    }
}

void MainWindow::onDeleteTaskClicked()
{
    int row = ui->taskTable->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Delete Task", "Please select a task first.");
        return;
    }

    QString taskTitle = ui->taskTable->item(row, 1)->text();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirm Delete",
        "Are you sure you want to delete this task?\n\nTask: " + taskTitle,
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::No) {
        return;
    }

    int id = ui->taskTable->item(row, 0)->text().toInt();

    deleteTaskFromDatabase(id);
    ui->taskTable->removeRow(row);
    updateStats();
}
void MainWindow::onCompleteTaskClicked()
{
    int row = ui->taskTable->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Complete Task", "Please select a task first.");
        return;
    }

    int id = ui->taskTable->item(row, 0)->text().toInt();

    completeTaskInDatabase(id);
    ui->taskTable->setItem(row, 5, new QTableWidgetItem("Completed"));
    updateStats();
}

void MainWindow::onSystemMonitorClicked()
{
    SYSTEM_INFO sysInfo;
    MEMORYSTATUSEX memoryStatus;

    GetSystemInfo(&sysInfo);

    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);

    QString message;

    message += "Processor Count: " + QString::number(sysInfo.dwNumberOfProcessors) + "\n";
    message += "Memory Usage: " + QString::number(memoryStatus.dwMemoryLoad) + "%\n";
    message += "Total RAM: " + QString::number(memoryStatus.ullTotalPhys / (1024 * 1024 * 1024)) + " GB\n";
    message += "Available RAM: " + QString::number(memoryStatus.ullAvailPhys / (1024 * 1024 * 1024)) + " GB\n";

    QMessageBox::information(this, "System Monitor", message);
}

void MainWindow::onSearchTextChanged(const QString &text)
{
    for (int row = 0; row < ui->taskTable->rowCount(); ++row) {
        bool match = false;

        for (int col = 1; col < ui->taskTable->columnCount(); ++col) {
            QTableWidgetItem *item = ui->taskTable->item(row, col);

            if (item && item->text().contains(text, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }

        ui->taskTable->setRowHidden(row, !match);
    }
}

void MainWindow::updateStats()
{
    int total = ui->taskTable->rowCount();
    int completed = 0;

    for (int row = 0; row < total; ++row) {
        QTableWidgetItem *statusItem = ui->taskTable->item(row, 5);

        if (statusItem && statusItem->text() == "Completed") {
            completed++;
        }
    }

    int pending = total - completed;

    ui->statsLabel->setText(
        "Total Tasks: " + QString::number(total) +
        " | Completed: " + QString::number(completed) +
        " | Pending: " + QString::number(pending)
        );
}
void MainWindow::applyPriorityColor(int row)
{
    QTableWidgetItem *priorityItem = ui->taskTable->item(row, 2);

    if (!priorityItem) {
        return;
    }

    QString priority = priorityItem->text();

    if (priority == "High") {
        priorityItem->setBackground(QColor("#fee2e2"));
        priorityItem->setForeground(QColor("#991b1b"));
    }
    else if (priority == "Medium") {
        priorityItem->setBackground(QColor("#fef3c7"));
        priorityItem->setForeground(QColor("#92400e"));
    }
    else if (priority == "Low") {
        priorityItem->setBackground(QColor("#dcfce7"));
        priorityItem->setForeground(QColor("#166534"));
    }
}

void MainWindow::applyStatusColor(int row)
{
    QTableWidgetItem *statusItem = ui->taskTable->item(row, 5);

    if (!statusItem)
        return;

    QString status = statusItem->text();

    if (status == "Completed") {
        statusItem->setBackground(QColor("#dcfce7"));
        statusItem->setForeground(QColor("#166534"));
    }
    else {
        statusItem->setBackground(QColor("#fef3c7"));
        statusItem->setForeground(QColor("#92400e"));
    }
}



void MainWindow::applyDueDateColor(int row)
{
    QTableWidgetItem *dateItem = ui->taskTable->item(row, 3);
    QTableWidgetItem *statusItem = ui->taskTable->item(row, 5);

    if (!dateItem || !statusItem) {
        return;
    }

    if (statusItem->text() == "Completed") {
        dateItem->setBackground(QColor("#dcfce7"));
        dateItem->setForeground(QColor("#166534"));
        return;
    }

    QDate dueDate = QDate::fromString(dateItem->text(), "yyyy-MM-dd");
    QDate today = QDate::currentDate();

    int daysRemaining = today.daysTo(dueDate);

    if (daysRemaining < 0) {
        dateItem->setBackground(QColor("#fee2e2"));
        dateItem->setForeground(QColor("#991b1b"));
    }
    else if (daysRemaining <= 3) {
        dateItem->setBackground(QColor("#fef3c7"));
        dateItem->setForeground(QColor("#92400e"));
    }
    else {
        dateItem->setBackground(QColor("#dcfce7"));
        dateItem->setForeground(QColor("#166534"));
    }
}

void MainWindow::updateTaskInDatabase(int id,
                                      const QString &title,
                                      const QString &priority,
                                      const QString &dueDate,
                                      const QString &category)
{
    const char* sql =
        "UPDATE tasks SET title = ?, priority = ?, dueDate = ?, category = ? WHERE id = ?;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, title.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, priority.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, dueDate.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, category.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, id);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void MainWindow::onEditTaskClicked()
{
    int row = ui->taskTable->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Edit Task", "Please select a task first.");
        return;
    }

    int id = ui->taskTable->item(row, 0)->text().toInt();

    QString title = ui->taskTable->item(row, 1)->text();
    QString priority = ui->taskTable->item(row, 2)->text();
    QString dueDate = ui->taskTable->item(row, 3)->text();
    QString category = ui->taskTable->item(row, 4)->text();

    AddTaskDialog dialog(this);
    dialog.setWindowTitle("Edit Task");
    dialog.setTaskData(title, priority, dueDate, category);

    if (dialog.exec() == QDialog::Accepted) {
        QString newTitle = dialog.getTitle();

        if (newTitle.isEmpty()) {
            QMessageBox::warning(this, "Edit Task", "Task title cannot be empty.");
            return;
        }

        QString newPriority = dialog.getPriority();
        QString newDueDate = dialog.getDueDate();
        QString newCategory = dialog.getCategory();

        updateTaskInDatabase(id, newTitle, newPriority, newDueDate, newCategory);

        ui->taskTable->setItem(row, 1, new QTableWidgetItem(newTitle));
        ui->taskTable->setItem(row, 2, new QTableWidgetItem(newPriority));
        ui->taskTable->setItem(row, 3, new QTableWidgetItem(newDueDate));
        ui->taskTable->setItem(row, 4, new QTableWidgetItem(newCategory));

        applyPriorityColor(row);
        applyDueDateColor(row);
        applyStatusColor(row);
        updateStats();
    }
}

void MainWindow::onDashboardClicked()
{
    int total = ui->taskTable->rowCount();
    int completed = 0;
    int pending = 0;
    int highPriority = 0;
    int mediumPriority = 0;
    int lowPriority = 0;
    int schoolTasks = 0;
    int workTasks = 0;
    int personalTasks = 0;

    for (int row = 0; row < total; ++row) {
        QString priority = ui->taskTable->item(row, 2)->text();
        QString status = ui->taskTable->item(row, 5)->text();
        QString category = ui->taskTable->item(row, 4)->text();

        if (category == "School") {
            schoolTasks++;
        }
        else if (category == "Work") {
            workTasks++;
        }
        else if (category == "Personal") {
            personalTasks++;
        }

        if (priority == "High") {
            highPriority++;
        }
        else if (priority == "Medium") {
            mediumPriority++;
        }
        else if (priority == "Low") {
            lowPriority++;
        }

        if (status == "Completed") {
            completed++;
        } else {
            pending++;
        }
    }

    double completionRate = 0.0;

    if (total > 0) {
        completionRate = (static_cast<double>(completed) / total) * 100.0;
    }

    DashboardDialog dialog(this);

    dialog.setDashboardData(
        total,
        completed,
        pending,
        highPriority,
        mediumPriority,
        lowPriority,
        schoolTasks,
        workTasks,
        personalTasks,
        completionRate
        );
    dialog.exec();
}

void MainWindow::onExportCsvClicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Export Tasks to CSV",
        "tasks_export.csv",
        "CSV Files (*.csv)"
        );

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Export Error", "Could not create CSV file.");
        return;
    }

    QTextStream out(&file);

    out << "ID,Title,Priority,Due Date,Category,Status\n";

    for (int row = 0; row < ui->taskTable->rowCount(); ++row) {
        if (ui->taskTable->isRowHidden(row)) {
            continue;
        }

        for (int col = 0; col < ui->taskTable->columnCount(); ++col) {
            QTableWidgetItem *item = ui->taskTable->item(row, col);

            QString text = item ? item->text() : "";

            text.replace("\"", "\"\"");

            out << "\"" << text << "\"";

            if (col < ui->taskTable->columnCount() - 1) {
                out << ",";
            }
        }

        out << "\n";
    }

    file.close();

    QMessageBox::information(this, "Export Complete", "Tasks exported successfully.");
}

void MainWindow::applyLightTheme()
{
    this->setStyleSheet(
        "QMainWindow { background-color: #f4f6f8; }"
        "QLabel { font-size: 14px; font-weight: bold; color: #1e293b; }"
        "QLabel#titleLabel { font-size: 22px; font-weight: bold; color: #1e293b; }"
        "QPushButton { background-color: #2563eb; color: white; border-radius: 6px; "
        "padding: 4px 10px; font-weight: bold; font-size: 10pt; min-width: 90px; min-height: 24px; }"
        "QPushButton:hover { background-color: #1d4ed8; }"
        "QLineEdit { border: 1px solid #cbd5e1; border-radius: 6px; padding: 6px; background-color: white; color: black; }"
        "QTableWidget { background-color: white; color: black; border: 1px solid #cbd5e1; }"
        "QHeaderView::section { background-color: #e2e8f0; color: #1e293b; font-weight: bold; padding: 5px; }"
        );
}

void MainWindow::applyDarkTheme()
{
    this->setStyleSheet(
        "QMainWindow { background-color: #0f172a; }"
        "QLabel { font-size: 14px; font-weight: bold; color: #e5e7eb; }"
        "QLabel#titleLabel { font-size: 22px; font-weight: bold; color: #f8fafc; }"
        "QPushButton { background-color: #3b82f6; color: white; border-radius: 6px; "
        "padding: 4px 10px; font-weight: bold; font-size: 10pt; min-width: 90px; min-height: 24px; }"
        "QPushButton:hover { background-color: #60a5fa; }"
        "QLineEdit { border: 1px solid #334155; border-radius: 6px; padding: 6px; background-color: #1e293b; color: #f8fafc; }"
        "QTableWidget { background-color: #1e293b; color: #f8fafc; border: 1px solid #334155; gridline-color: #334155; }"
        "QHeaderView::section { background-color: #334155; color: #f8fafc; font-weight: bold; padding: 5px; }"
        );
}

void MainWindow::onDarkModeClicked()
{
    darkModeEnabled = !darkModeEnabled;

    if (darkModeEnabled) {
        applyDarkTheme();
        ui->darkModeButton->setText("Light Mode");
    } else {
        applyLightTheme();
        ui->darkModeButton->setText("Dark Mode");
    }

    QSettings settings("FadhilSE", "SmartTaskManager");
    settings.setValue("darkModeEnabled", darkModeEnabled);
}

void MainWindow::onImportCsvClicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Import Tasks from CSV",
        "",
        "CSV Files (*.csv)"
        );

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Import Error", "Could not open CSV file.");
        return;
    }

    QTextStream in(&file);

    // Skip header line
    if (!in.atEnd()) {
        in.readLine();
    }

    int importedCount = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line.trimmed().isEmpty()) {
            continue;
        }

        QStringList columns = line.split(",");

        if (columns.size() < 5) {
            continue;
        }

        QString title = columns[0].remove("\"").trimmed();
        QString priority = columns[1].remove("\"").trimmed();
        QString dueDate = columns[2].remove("\"").trimmed();
        QString category = columns[3].remove("\"").trimmed();
        QString status = columns[4].remove("\"").trimmed();

        int id = saveTaskToDatabase(title, priority, dueDate, category);

        if (id != -1) {
            addTaskToTable(id, title, priority, dueDate, category, status);

            if (status == "Completed") {
                completeTaskInDatabase(id);
            }

            importedCount++;
        }
    }

    file.close();
    updateStats();

    QMessageBox::information(
        this,
        "Import Complete",
        QString::number(importedCount) + " tasks imported successfully."
        );
}