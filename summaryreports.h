#ifndef SUMMARYREPORTS_H
#define SUMMARYREPORTS_H

#include <QDialog>

namespace Ui {
class reports;
}

class reports : public QDialog
{
    Q_OBJECT

public:
    explicit reports(QWidget *parent = nullptr);
    ~reports();

private slots:
    void on_pmr_clicked();

    void on_flightoccupancy_clicked();

    void on_schedulereport_clicked();

    void on_salesreport_clicked();

    void on_pass_reports_clicked();

    void on_schedule_clicked();

private:
    Ui::reports *ui;
    QMap<QString, QList<QStringList>> flightPassengersMap;
};

#endif // SUMMARYREPORTS_H
