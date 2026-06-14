#ifndef DASHBOARDDIALOG_H
#define DASHBOARDDIALOG_H

#include <QDialog>
#include <QtCharts/QChartView>

namespace Ui {
class DashboardDialog;
}

class DashboardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DashboardDialog(QWidget *parent = nullptr);
    ~DashboardDialog();

    void setDashboardData(int totalTasks,
                          int completedTasks,
                          int pendingTasks,
                          int highPriorityTasks,
                          int mediumPriorityTasks,
                          int lowPriorityTasks,
                          double completionRate);

private:
    Ui::DashboardDialog *ui;
    QChartView *chartView;
    QChartView *priorityChartView;
};

#endif