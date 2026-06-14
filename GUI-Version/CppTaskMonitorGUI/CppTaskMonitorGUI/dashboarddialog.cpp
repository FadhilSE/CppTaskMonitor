#include "dashboarddialog.h"
#include "ui_dashboarddialog.h"

#include <QDialogButtonBox>
#include <QPainter>
#include <algorithm>

#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

DashboardDialog::DashboardDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DashboardDialog)
{
    ui->setupUi(this);
    setFixedSize(650, 520);
    chartView = new QChartView(this);
    chartView->setGeometry(380, 40, 220, 180);
    chartView->setRenderHint(QPainter::Antialiasing);

    priorityChartView = new QChartView(this);
    priorityChartView->setGeometry(320, 250, 300, 200);
    priorityChartView->setRenderHint(QPainter::Antialiasing);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    setWindowTitle("Analytics Dashboard");

    this->setStyleSheet(
        "QDialog { background-color: #f4f6f8; }"
        "QLabel { font-size: 14px; font-weight: bold; color: #1e293b; }"
        "QPushButton {"
        " background-color: #2563eb;"
        " color: white;"
        " border-radius: 6px;"
        " padding: 6px 18px;"
        " font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #1d4ed8; }"
        );
}

DashboardDialog::~DashboardDialog()
{
    delete ui;
}

void DashboardDialog::setDashboardData(int totalTasks,
                                       int completedTasks,
                                       int pendingTasks,
                                       int highPriorityTasks,
                                       int mediumPriorityTasks,
                                       int lowPriorityTasks,
                                       double completionRate)
{
    ui->totalTasksLabel->setText("Total Tasks: " + QString::number(totalTasks));
    ui->completedTasksLabel->setText("Completed Tasks: " + QString::number(completedTasks));
    ui->pendingTasksLabel->setText("Pending Tasks: " + QString::number(pendingTasks));
    ui->highPriorityLabel->setText("High Priority: " + QString::number(highPriorityTasks));
    ui->completionRateLabel->setText("Completion Rate: " + QString::number(completionRate, 'f', 1) + "%");

    QPieSeries *series = new QPieSeries();
    series->append("Completed", completedTasks);
    series->append("Pending", pendingTasks);

    QChart *pieChart = new QChart();
    pieChart->addSeries(series);
    pieChart->setTitle("Task Status");
    pieChart->legend()->setVisible(true);
    chartView->setChart(pieChart);

    QBarSet *prioritySet = new QBarSet("Tasks");
    *prioritySet << highPriorityTasks << mediumPriorityTasks << lowPriorityTasks;

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(prioritySet);

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("Priority Levels");

    QStringList categories;
    categories << "High" << "Medium" << "Low";

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);

    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    int maxValue = std::max({highPriorityTasks, mediumPriorityTasks, lowPriorityTasks, 1});

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, maxValue);
    axisY->setLabelFormat("%d");

    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    barChart->legend()->setVisible(false);
    priorityChartView->setChart(barChart);
}