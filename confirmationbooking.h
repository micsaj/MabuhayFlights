#ifndef CONFIRMATIONBOOKING_H
#define CONFIRMATIONBOOKING_H

#include <QDialog>

namespace Ui {
class confirmationbooking;
}

class confirmationbooking : public QDialog
{
    Q_OBJECT

public:
    explicit confirmationbooking(QWidget *parent = nullptr);
    ~confirmationbooking();
    void setTicketSummary(const QString& summary);

private slots:
    void on_download_clicked();

private:
    Ui::confirmationbooking *ui;

};

#endif // CONFIRMATIONBOOKING_H
