#ifndef FLIGHTSUMMARY_H
#define FLIGHTSUMMARY_H

#include <QDialog>

namespace Ui {
class flightsummary;
}

class flightsummary : public QDialog
{
    Q_OBJECT

public:
    explicit flightsummary(QWidget *parent = nullptr);
    ~flightsummary();

private slots:




    void on_showtable_clicked();


    void on_reset_clicked();

    void on_add_clicked();

    void on_edit_clicked();

    void on_save_newflight_clicked();

    void on_del_clicked();

    void on_save_changes_clicked();

private:
    Ui::flightsummary *ui;
};

#endif // FLIGHTSUMMARY_H
