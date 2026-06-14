#ifndef ADDTASKDIALOG_H
#define ADDTASKDIALOG_H

#include <QDialog>

namespace Ui {
class AddTaskDialog;
}

class AddTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTaskDialog(QWidget *parent = nullptr);
    ~AddTaskDialog();

    QString getTitle() const;
    QString getPriority() const;
    QString getDueDate() const;
    QString getCategory() const;

    void setTaskData(const QString &title,
                     const QString &priority,
                     const QString &dueDate,
                     const QString &category);

private:
    Ui::AddTaskDialog *ui;
};

#endif