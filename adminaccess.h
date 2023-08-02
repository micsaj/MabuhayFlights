#ifndef ADMINACCESS_H
#define ADMINACCESS_H

#include <QDialog>

namespace Ui {
class AdminAccess;
}

class AdminAccess : public QDialog
{
    Q_OBJECT

public:
    explicit AdminAccess(QWidget *parent = nullptr);
    ~AdminAccess();

private slots:
    void on_flights_clicked();

    void on_pushButton_2_clicked();

    void on_summaryreports_clicked();

    void on_home_clicked();

private:
    Ui::AdminAccess *ui;
};

#endif // ADMINACCESS_H
