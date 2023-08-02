#ifndef FLIGHTOCCUPANCY_H
#define FLIGHTOCCUPANCY_H

#include <QDialog>

namespace Ui {
class flightoccupancy;
}

class flightoccupancy : public QDialog
{
    Q_OBJECT

public:
    explicit flightoccupancy(QWidget *parent = nullptr);
    ~flightoccupancy();
    void setReportContent(const QString& content);

private:
    Ui::flightoccupancy *ui;

};

#endif // FLIGHTOCCUPANCY_H
