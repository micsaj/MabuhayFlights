#ifndef BOOKFLIGHTS_H
#define BOOKFLIGHTS_H

#include <QDialog>

namespace Ui {
class BookFlights;
}

class CustomMessageBox;

class BookFlights : public QDialog
{
    Q_OBJECT

public:
    explicit BookFlights(QWidget *parent = nullptr);
    ~BookFlights();

    int getTicketsSoldForFlight(const QString& flightNumber);

private slots:
    void on_plus_clicked();

    void on_minus_clicked();

    void on_home_clicked();

    void updateReturnDate();

    void on_checkout_clicked();

    void on_reset_clicked();

    void on_reserve_clicked();

    void on_domestic_clicked();

    void on_international_clicked();

    void on_earlyflight_clicked();

    void on_morningflight_clicked();

    void on_afternoon_clicked();

    void on_nightflight_clicked();

    void on_oneway_clicked();

    void on_roundtrip_clicked();

    void on_economy_clicked();

    void on_business_clicked();

    void on_searchflight_clicked();


    void on_clear_clicked();

private:
    Ui::BookFlights *ui;
};

#endif // BOOKFLIGHTS_H
