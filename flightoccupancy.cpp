#include "flightoccupancy.h"
#include "ui_flightoccupancy.h"

flightoccupancy::flightoccupancy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::flightoccupancy)
{
    ui->setupUi(this);
    setStyleSheet("flightoccupancy { background-image: url(:/pix/MABUHAY FLIGHTS/bg); }");

}

flightoccupancy::~flightoccupancy()
{
    delete ui;
}

void flightoccupancy::setReportContent(const QString& content)
{
    ui->occupancyreport->setPlainText(content);
}
