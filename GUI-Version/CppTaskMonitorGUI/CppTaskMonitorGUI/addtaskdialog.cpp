#include "addtaskdialog.h"
#include "ui_addtaskdialog.h"

AddTaskDialog::AddTaskDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddTaskDialog)
{
    ui->setupUi(this);

    this->setStyleSheet(
        "QDialog { background-color: #f4f6f8; }"
        "QLabel { font-size: 12px; font-weight: bold; color: #1e293b; }"
        "QLineEdit, QComboBox, QDateEdit {"
        " border: 1px solid #cbd5e1;"
        " border-radius: 6px;"
        " padding: 5px;"
        " background-color: white;"
        "}"
        "QPushButton {"
        " background-color: #2563eb;"
        " color: white;"
        " border-radius: 6px;"
        " padding: 6px 16px;"
        " font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #1d4ed8; }"
        );

    ui->priorityComboBox->clear();
    ui->priorityComboBox->addItems({"Low", "Medium", "High"});

    ui->categoryComboBox->clear();
    ui->categoryComboBox->addItems({"School", "Work", "Personal"});

    ui->dueDateEdit->setCalendarPopup(true);
}

AddTaskDialog::~AddTaskDialog()
{
    delete ui;
}

QString AddTaskDialog::getTitle() const
{
    return ui->titleLineEdit->text();
}

QString AddTaskDialog::getPriority() const
{
    return ui->priorityComboBox->currentText();
}

QString AddTaskDialog::getDueDate() const
{
    return ui->dueDateEdit->date().toString("yyyy-MM-dd");
}

QString AddTaskDialog::getCategory() const
{
    return ui->categoryComboBox->currentText();
}