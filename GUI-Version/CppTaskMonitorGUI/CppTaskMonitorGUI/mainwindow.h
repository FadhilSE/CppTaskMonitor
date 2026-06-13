#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <sqlite3.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddTaskClicked();
    void onDeleteTaskClicked();
    void onCompleteTaskClicked();
    void onSystemMonitorClicked();
    void onSearchTextChanged(const QString &text);
    void updateStats();
    void applyPriorityColor(int row);
    void applyStatusColor(int row);
    void applyDueDateColor(int row);

private:
    Ui::MainWindow *ui;
    sqlite3* db;

    void initializeDatabase();
    void loadTasksFromDatabase();
    void addTaskToTable(int id, const QString &title, const QString &priority,
                        const QString &dueDate, const QString &category, const QString &status);
    int saveTaskToDatabase(const QString &title, const QString &priority,
                           const QString &dueDate, const QString &category);
    void deleteTaskFromDatabase(int id);
    void completeTaskInDatabase(int id);
};

#endif