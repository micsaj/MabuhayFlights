#ifndef SHOWFLIGHTS_H
#define SHOWFLIGHTS_H

#include <QDialog>

namespace Ui {
class ShowFlights;
}

class ShowFlights : public QDialog
{
    Q_OBJECT

public:
    explicit ShowFlights(QWidget *parent = nullptr);
    ~ShowFlights();

private slots:
    void on_home_clicked();

    void on_domestic_clicked();

    void on_international_clicked();

private:
    Ui::ShowFlights *ui;
};

#endif // SHOWFLIGHTS_H
