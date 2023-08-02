#ifndef SEATSRESERVATION_H
#define SEATSRESERVATION_H

#include <QDialog>

namespace Ui {
class seatsreservation;
}

class seatsreservation : public QDialog
{
    Q_OBJECT

public:
    explicit seatsreservation(QWidget *parent = nullptr);
    ~seatsreservation();
    void onSeatButtonClicked();

private slots:
    void on_a1_clicked();

    void on_b1_clicked();

    void on_c1_clicked();

    void on_a2_clicked();

    void on_b2_clicked();

    void on_c2_clicked();

    void on_a3_clicked();

    void on_b3_clicked();

    void on_c3_clicked();

    void on_a4_clicked();

    void on_b4_clicked();

    void on_c4_clicked();

    void on_a5_clicked();

    void on_b5_clicked();

    void on_c5_clicked();

    void on_d1_clicked();

    void on_e1_clicked();

    void on_f1_clicked();

    void on_d2_clicked();

    void on_e2_clicked();

    void on_f2_clicked();

    void on_d3_clicked();

    void on_e3_clicked();

    void on_f3_clicked();

    void on_d4_clicked();

    void on_e4_clicked();

    void on_f4_clicked();

    void on_d5_clicked();

    void on_e5_clicked();

    void on_next_96_clicked();

    void on_f5_clicked();

private:
    Ui::seatsreservation *ui;
    void connectAllSeatButtons();
};

#endif // SEATSRESERVATION_H
